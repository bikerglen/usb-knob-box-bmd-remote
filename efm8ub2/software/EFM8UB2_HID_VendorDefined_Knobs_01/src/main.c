//-----------------------------------------------------------------------------------------------
// EFM8UB2 USB Controller Software for Sony BKS-R3209 Panels
//

//-----------------------------------------------------------------------------------------------
// includes
//

#include "SI_EFM8UB2_Register_Enums.h"
#include "efm8_usb.h"
#include "descriptors.h"
#include "idle.h"
#include "InitDevice.h"
#include "config.h"


//-----------------------------------------------------------------------------------------------
// defines
//

// device pins
SI_SBIT(SW1_LED,  SFR_P2, 3); // SW1 LED -- top button on top of board
SI_SBIT(SW2_LED,  SFR_P2, 2); // SW2 LED -- bottom button on top of board
SI_SBIT(USER_LED, SFR_P0, 6); // USER LED -- on bottom of board

#define ENC1_A  ((P0 & 0x20) ? 1 : 0)
#define ENC1_B  ((P0 & 0x10) ? 1 : 0)
#define ENC1_SW ((P0 & 0x08) ? 0 : 1)

#define ENC2_A  ((P4 & 0x04) ? 1 : 0)
#define ENC2_B  ((P4 & 0x08) ? 1 : 0)
#define ENC2_SW ((P4 & 0x02) ? 0 : 1)

#define ENC3_A  ((P2 & 0x01) ? 1 : 0)
#define ENC3_B  ((P2 & 0x02) ? 1 : 0)
#define ENC3_SW ((P1 & 0x80) ? 0 : 1)

#define ENC4_A  ((P3 & 0x10) ? 1 : 0)
#define ENC4_B  ((P3 & 0x20) ? 1 : 0)
#define ENC4_SW ((P3 & 0x40) ? 0 : 1)

#define ENC5_A  ((P2 & 0x10) ? 1 : 0)
#define ENC5_B  ((P2 & 0x20) ? 1 : 0)
#define ENC5_SW ((P2 & 0x40) ? 0 : 1)

#define SW1     ((P3 & 0x80) ? 0 : 1)
#define SW2     ((P4 & 0x01) ? 0 : 1)

#define LED_ON  0
#define LED_OFF 1

#define SYSCLK       48000000      // SYSCLK frequency in Hz

#define IRIS_CHANNEL          1
#define FOCUS_CHANNEL         2
#define AUTOFOCUS_CHANNEL     3
#define GAIN_CHANNEL          5
#define SHUTTER_CHANNEL       6
#define WHITE_BALANCE_CHANNEL 7


//-----------------------------------------------------------------------------------------------
// typedefs
//

typedef uint8_t FSTOP_TABLE[25];


//-----------------------------------------------------------------------------------------------
// prototypes
//

void Timer2_Init (int counts);
uint16_t lfsr (void);
uint8_t GetDirection (uint8_t a, uint8_t b);
uint8_t ProcessButton (uint8_t which, uint8_t sw);
void ConvertServoChannelsToSbusData (void);


//-----------------------------------------------------------------------------------------------
// globals
//

SI_SEGMENT_VARIABLE(fstops, const FSTOP_TABLE, SI_SEG_CODE) = {
	212, 205, 198, 191, 184,
	177, 170, 163, 156, 149,
	142, 135, 128, 121, 115,
	107, 100,  93,  86,  80,
     73,  66,  60,  56,  44
};

// signals from ISRs to main loop
volatile uint8_t flag250 = 0;

// pseudo random lfsr state
uint16_t lfsr_state = 0x0001;

uint8_t enc1a;
uint8_t enc1b;
uint8_t enc2a;
uint8_t enc2b;
uint8_t enc3a;
uint8_t enc3b;
uint8_t enc4a;
uint8_t enc4b;
uint8_t enc5a;
uint8_t enc5b;

// local variables for making aUSB report
uint8_t xdata reportNeeded;
uint8_t xdata thisUsbReportData[6];
uint8_t lastUsbReportData0 = 0;

// variables used to communicate report to USB ISR
volatile uint8_t xdata usbReportNeeded = false;
volatile uint8_t xdata usbReportData[6];

uint8_t xdata buttonStates[8];

// camera parameters
int8_t xdata balance;
int8_t xdata gain;
int8_t xdata shutter;
uint8_t xdata iris;
uint8_t xdata focus;

// servo data sending phase
uint8_t xdata servoPhase;

// servo channels
uint16_t xdata servoChannels[18];

// sbus data
uint8_t txByte;
uint8_t xdata sbusData[25];


//-----------------------------------------------------------------------------------------------
// SiLabs_Startup() Routine
//

void SiLabs_Startup (void)
{
  // Disable the watchdog here
}
 

//-----------------------------------------------------------------------------------------------
// main() Routine
//

int16_t main(void)
{
	uint8_t SFRPAGE_save;
	uint8_t led_timer = 0;
    uint8_t newUsbState = 0;
    uint8_t oldUsbState = 0;
    uint8_t i;

    // set default pin and peripheral configuration
    enter_DefaultMode_from_RESET();

	// Init Timer2 to generate interrupts at 250 Hz
	Timer2_Init (16000); // SYSCLK / 12 / 250

	// initialize encoder 3-bit states to prevent spurious up/down signals at startup
	enc1a = ENC1_A ? 7 : 0;
	enc1b = ENC1_B ? 7 : 0;
	enc2a = ENC2_A ? 7 : 0;
	enc2b = ENC2_B ? 7 : 0;
	enc3a = ENC3_A ? 7 : 0;
	enc3b = ENC3_B ? 7 : 0;
	enc4a = ENC4_A ? 7 : 0;
	enc4b = ENC4_B ? 7 : 0;
	enc5a = ENC5_A ? 7 : 0;
	enc5b = ENC5_B ? 7 : 0;

	// zero button states
	for (i = 0; i < 8; i++) {
		buttonStates[i] = 0;
	}

	// turn off leds
	SW1_LED = LED_OFF;
	SW2_LED = LED_OFF;

	// initialize camera parameters
	balance = 0;
	gain = 0;
	shutter = 0;
	iris = 0;
	focus = 128;

	// initialize servo phase
	servoPhase = 0;

	// initialize servo channels
	for (i = 0; i < 18; i++) {
		servoChannels[i] = 0x400;
	}

	// initialize sbus data
	for (i = 0; i < 25; i++) {
		sbusData[i] = 0x00;
	}

	// loop forever
	while (1)
	{
		// check if 100 Hz timer expired
		if (flag250) {
			flag250 = false;

			// new USB state
			newUsbState = USBD_GetUsbState ();

			// blink LED based on USB state
			if (newUsbState == USBD_STATE_CONFIGURED) {
				// run one led pattern for configured usb state
				if (led_timer < 25) {
					USER_LED = LED_ON; // LED ON
				} else if (led_timer < 50) {
					USER_LED = LED_OFF; // LED OFF
				} else if (led_timer < 75) {
					USER_LED = LED_ON; // LED ON
				} else if (led_timer < 100) {
					USER_LED = LED_OFF; // LED OFF
				}

				// tasks to run on entering configured state
				if (newUsbState != oldUsbState) {
					oldUsbState = newUsbState;
					// TODO
				}
			} else {
				// and another for all other usb states
				if (led_timer < 50) {
					USER_LED = LED_ON; // LED ON
				} else {
					USER_LED = LED_OFF; // LED OFF
				}

				// tasks to run on leaving configured state
				if ((newUsbState != oldUsbState) && (oldUsbState == USBD_STATE_CONFIGURED)) {
					oldUsbState = newUsbState;
					// TODO
				}
			}

			// update led timer
			if (++led_timer == 100) {
				led_timer = 0;
			}

			// sample encoders
			enc1a = ((enc1a << 1) | ENC1_A) & 7;
			enc1b = ((enc1b << 1) | ENC1_B) & 7;
			enc2a = ((enc2a << 1) | ENC2_A) & 7;
			enc2b = ((enc2b << 1) | ENC2_B) & 7;
			enc3a = ((enc3a << 1) | ENC3_A) & 7;
			enc3b = ((enc3b << 1) | ENC3_B) & 7;
			enc4a = ((enc4a << 1) | ENC4_A) & 7;
			enc4b = ((enc4b << 1) | ENC4_B) & 7;
			enc5a = ((enc5a << 1) | ENC5_A) & 7;
			enc5b = ((enc5b << 1) | ENC5_B) & 7;

			// clear USB report data
			for (i = 0; i < 6; i++) {
				thisUsbReportData[i] = 0;
			}

			// sample and process buttons
			thisUsbReportData[0] |= ProcessButton (0, ENC1_SW);
			thisUsbReportData[0] |= ProcessButton (1, ENC2_SW);
			thisUsbReportData[0] |= ProcessButton (2, ENC3_SW);
			thisUsbReportData[0] |= ProcessButton (3, ENC4_SW);
			thisUsbReportData[0] |= ProcessButton (4, ENC5_SW);
			thisUsbReportData[0] |= ProcessButton (5, SW1);
			thisUsbReportData[0] |= ProcessButton (6, SW2);

			// process encoders
			thisUsbReportData[1] = GetDirection (enc1a, enc1b);
			thisUsbReportData[2] = GetDirection (enc2a, enc2b);
			thisUsbReportData[3] = GetDirection (enc3a, enc3b);
			thisUsbReportData[4] = GetDirection (enc4a, enc4b);
			thisUsbReportData[5] = GetDirection (enc5a, enc5b);

			// check if report needed
			reportNeeded = thisUsbReportData[1] | thisUsbReportData[2] | thisUsbReportData[3] | thisUsbReportData[4] | thisUsbReportData[5] |
					(thisUsbReportData[0] != lastUsbReportData0);

			// save key presses for the next time around
			lastUsbReportData0 = thisUsbReportData[0];

			// send report to IRQ-based USB code safely
			if (reportNeeded) {
				SFRPAGE_save = SFRPAGE;
				USB_DisableInts ();
				SFRPAGE = SFRPAGE_save;

				usbReportNeeded = true;
				for (i = 0; i < 6; i++) {
					usbReportData[i] = thisUsbReportData[i];
				}

				SFRPAGE_save = SFRPAGE;
				USB_EnableInts ();
				SFRPAGE = SFRPAGE_save;
			}

			// white balance is controlled by encoder 1
			if (thisUsbReportData[1] & 0x01) {
				balance++;
			} else if (thisUsbReportData[1] & 0x02) {
				balance--;
			}

			// gain -- encoder 2
			if (thisUsbReportData[2] & 0x01) {
				gain++;
			} else if (thisUsbReportData[2] & 0x02) {
				gain--;
			}

			// shutter -- encoder 3
			if (thisUsbReportData[3] & 0x01) {
				shutter++;
			} else if (thisUsbReportData[3] & 0x02) {
				shutter--;
			}

			// iris -- encoder 4
			if (thisUsbReportData[4] & 0x01) {
				if (iris < 24) iris++;
			} else if (thisUsbReportData[4] & 0x02) {
				if (iris > 0) iris--;
			}

			// focus is controlled by encoder 5
			if (thisUsbReportData[5] & 0x01) {
				// CW = focus farther away
				if (focus > 44) focus--;
			} else if (thisUsbReportData[5] & 0x02) {
				// CCW = focus closer
				if (focus < 212) focus++;
			}

			// push to auto focus -- encoder 5 knob
			// TODO

			if (servoPhase == 0) {
				// convert user actions / camera parameters to servo channel data

				// white balance
				if (servoChannels[WHITE_BALANCE_CHANNEL] == 0x400) {
					if (balance > 0) {
						balance--;
						servoChannels[WHITE_BALANCE_CHANNEL] = 8*212; // stick up
					} else if (balance < 0) {
						balance++;
						servoChannels[WHITE_BALANCE_CHANNEL] = 8*44;  // stick down
					}
				} else {
					servoChannels[WHITE_BALANCE_CHANNEL] = 0x400;     // back to stick neutral
				}

				// gain
				if (servoChannels[GAIN_CHANNEL] == 0x400) {
					if (gain > 0) {
						gain--;
						servoChannels[GAIN_CHANNEL] = 8*212; // stick up
					} else if (gain < 0) {
						gain++;
						servoChannels[GAIN_CHANNEL] = 8*44;  // stick down
					}
				} else {
					servoChannels[GAIN_CHANNEL] = 0x400;     // back to stick neutral
				}

				// shutter
				if (servoChannels[SHUTTER_CHANNEL] == 0x400) {
					if (shutter > 0) {
						shutter--;
						servoChannels[SHUTTER_CHANNEL] = 8*212; // stick up
					} else if (shutter < 0) {
						shutter++;
						servoChannels[SHUTTER_CHANNEL] = 8*44;  // stick down
					}
				} else {
					servoChannels[SHUTTER_CHANNEL] = 0x400;     // back to stick neutral
				}

				// iris
				servoChannels[IRIS_CHANNEL] = fstops[iris] << 3;

				// focus
				servoChannels[FOCUS_CHANNEL] = focus << 3;

			} else if (servoPhase == 1) {
				// convert servo channels to sbus data
				ConvertServoChannelsToSbusData ();
			} else if (servoPhase == 2) {
				// send sbus data to camera
				txByte = 0;
				SCON1 |= SCON1_TI__BMASK;
			}

			if (++servoPhase >= 3) {
				servoPhase = 0;
			}

		} // end if (flag250)
	}
}


void Timer2_Init (int counts)
{
   TMR2CN0 = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON0 &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN0_TR2 = 1;                    // Start Timer2
}


SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
    TMR2CN0_TF2H = 0;                   // Clear Timer2 interrupt flag
	flag250 = 1;
}


uint16_t lfsr (void)
{
    uint16_t newbit;

    newbit = ((lfsr_state >> 0) ^ (lfsr_state >> 2) ^ (lfsr_state >> 3) ^ (lfsr_state >> 5)) /* & 1u */;
    lfsr_state = (lfsr_state >> 1) | (newbit << 15);

    return lfsr_state;
}


uint8_t GetDirection (uint8_t a, uint8_t b)
{
	if (a == 4) {
		// 100 => falling edge
		return (b & 2) ? 0x01 : 0x02; // 0x01 = cw, 0x02 = ccw
	} else if (a == 3) {
		// 011 => rising edge
		return (b & 2) ? 0x02 : 0x01; // 0x02 = ccw, 0x01 = cw
	}

	return 0;
}


uint8_t ProcessButton (uint8_t which, uint8_t sw)
{
	uint8_t state;

	state = buttonStates[which];

	switch (state) {
		case 0: state = sw ? 1 : 0; break;
		case 1: state = sw ? 2 : 0; break;
		case 2: state = sw ? 2 : 3; break;
		case 3: state = sw ? 2 : 0; break;
	}

	buttonStates[which] = state;

	return (state & 2) ? (1 << which) : 0;
}


void ConvertServoChannelsToSbusData (void)
{
    uint8_t i, byte_in_sbus, bit_in_sbus, bit_in_servo, ch;

    // clear sbus data array
    for (i = 0; i < 25; i++)
    {
        sbusData[i] = 0;
    }

    // start code, flags, and end code
    sbusData[0] = 0x0F;
    sbusData[23] = 0x00;
    sbusData[24] = 0x00;

    //reset counters
    ch = 0;
    bit_in_servo = 0;
    byte_in_sbus = 1;
    bit_in_sbus = 0;

    for (i = 0; i < 176; i++)
    {
        if ((servoChannels[ch] & (1 << bit_in_servo)) != 0)
        {
            sbusData[byte_in_sbus] |= (1 << bit_in_sbus);
        }

        bit_in_sbus++;
        bit_in_servo++;

        if (bit_in_sbus == 8)
        {
            bit_in_sbus = 0;
            byte_in_sbus++;
        }

        if (bit_in_servo == 11)
        {
            bit_in_servo = 0;
            ch++;
        }
    }
}


SI_INTERRUPT(UART1_ISR, UART1_IRQn)
{
	if (SCON1 & SCON1_TI__BMASK) {
		SCON1 &= ~SCON1_TI__BMASK;
		if (txByte < 25) {
			SBUF1 = sbusData[txByte++];
		}
	}

	if (SCON1 & SCON1_RI__BMASK) {
		SCON1 &= ~SCON1_RI__BMASK;
	}
}
