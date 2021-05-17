//-----------------------------------------------------------------------------------------------
// includes
//

#include "system.h"

#include "usb.h"
#include "usb_device_hid.h"

#include "app_device_custom_hid.h"


//-----------------------------------------------------------------------------------------------
// defines
//

// servo channel mappings
#define IRIS_CHANNEL          1
#define FOCUS_CHANNEL         2
#define AUTOFOCUS_CHANNEL     3
#define GAIN_CHANNEL          5
#define SHUTTER_CHANNEL       6
#define WHITE_BALANCE_CHANNEL 7

// number of f-stops supported by lens
#define NUM_FSTOPS 25

enum {
    USB_SUSPENDED = 0,
    USB_CONNECTED = 1,
    USB_CONFIGURED = 2
};


//-----------------------------------------------------------------------------------------------
// typedefs
//


//-----------------------------------------------------------------------------------------------
// prototypes
//

uint8_t GetDirection (uint8_t a, uint8_t b);
uint8_t ProcessButton (uint8_t which, uint8_t sw);
void ConvertServoChannelsToSbusData (void);


//-----------------------------------------------------------------------------------------------
// globals
//

// discrete f stop mappings to servo channel value mapping
const uint8_t fstops[NUM_FSTOPS] = {
	212, 205, 198, 191, 184,
	177, 170, 163, 156, 149,
	142, 135, 128, 121, 115,
	107, 100,  93,  86,  80,
     73,  66,  60,  56,  44
};

// flag from timer isr to main to execute 250 Hz / 4 ms tick
volatile uint8_t flag250 = 0;  

// 1.5 second period led timer counter
uint16_t ledTimer = 0;

// encoder states
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
uint8_t reportNeeded;
uint8_t thisUsbReportData[6];
uint8_t lastUsbReportData0 = 0;

// variables used to communicate report to USB ISR
volatile uint8_t usbReportNeeded = false;
volatile uint8_t usbReportData[6];

// button debounce states
uint8_t buttonStates[8];

// camera parameters
int8_t balance;
int8_t gain;
int8_t shutter;
uint8_t iris;
uint8_t focus;

// servo data sending phase
uint8_t servoPhase;

// servo channels
uint16_t servoChannels[18];

// S.BUS data
volatile uint8_t txByte;
volatile uint8_t sbusData[25];


//-----------------------------------------------------------------------------------------------
// main
//

MAIN_RETURN main(void)
{
    uint8_t i;
    uint8_t newUsbState;
    
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    // no analog I/O
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    
    // initialize user LED to off
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 1;

    // initialize SW1 LED to off
    TRISAbits.TRISA2 = 0;
    LATAbits.LATA2 = 1;

    // initialize SW2 LED to off
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 1;
    
    // initialize SW1 inputs
    TRISAbits.TRISA3 = 1;

    // initialize SW2 inputs
    TRISAbits.TRISA1 = 1;

    // initialize encoder 1
    TRISAbits.TRISA7 = 1;
    TRISAbits.TRISA6 = 1;
    TRISCbits.TRISC0 = 1;

    // initialize encoder 2
    TRISDbits.TRISD5 = 1;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD4 = 1;

    // initialize encoder 3
    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA4 = 1;
    TRISEbits.TRISE0 = 1;

    // initialize encoder 4
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB0 = 1;

    // initialize encoder 5
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB3 = 1;
        
	// turn off leds
    USER_LED = LED_OFF;
	SW1_LED = LED_OFF;
	SW2_LED = LED_OFF;
    
    // enable EUSART transmitter in nine bit mode and configure baud rate generator
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 1;
    IPR1bits.TX1IP = 0;
    PIE1bits.TX1IE = 0;
    BAUDCON1 = 0x08;
    RCSTA1 = 0x80;
    TXSTA1 = 0x64;
    SPBRG1 = 0x77;
    SPBRGH1 = 0x00;

    // enable priority interrupts
    RCONbits.IPEN = 1;

    // configure USB
    USBDeviceInit();
    USBDeviceAttach();
    
    // configure TMR2
    TMR2_Initialize ();
    
    // configure TMR1 -- used to time bytes out serial port
    T0CONbits.T08BIT = 0;
    TMR0H = TMR0_RELOAD_VALUE >> 8;
    TMR0L = TMR0_RELOAD_VALUE & 0xFF;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;
    T0CON = 0x93;
    
    // enable low priority interrupts
    INTCONbits.GIEL = 1;

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

	// initialize S.BUS data
	for (i = 0; i < 25; i++) {
		sbusData[i] = 0x00;
	}
    
    while(1)
    {
        SYSTEM_Tasks();

        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif

        //Application specific tasks
        APP_DeviceCustomHIDTasks();
        
        // run 200 Hz tasks
        if (flag250) {
            // clear flag
            flag250 = 0;
            
            // get current USB state
            if (USBIsDeviceSuspended() == true) {
                newUsbState = USB_SUSPENDED;
            } else if (USBGetDeviceState() == CONFIGURED_STATE) {
                newUsbState = USB_CONFIGURED;
            } else {
                newUsbState = USB_CONNECTED;
            }
            
            // blink led
            if (ledTimer == 0) {
                USER_LED = LED_ON;
            } else if (ledTimer == 36) {
                USER_LED = LED_OFF;
            } else if ((ledTimer == 72) && (newUsbState >= USB_CONNECTED)) {
                USER_LED = LED_ON;
            } else if (ledTimer == 108) {
                USER_LED = LED_OFF;
            } else if ((ledTimer == 144) && (newUsbState >= USB_CONFIGURED)) {
                USER_LED = LED_ON;
            } else if (ledTimer == 180) {
                USER_LED = LED_OFF;
            }
            
            // increment led timer counter, 1.5 second period
            if (++ledTimer >= 375) {
                ledTimer = 0;
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

			// send report to USB code
			if (reportNeeded) {                
				usbReportNeeded = true;
				for (i = 0; i < 6; i++) {
					usbReportData[i] = thisUsbReportData[i];
				}
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
                TMR0H = TMR0_RELOAD_VALUE >> 8;
                TMR0L = TMR0_RELOAD_VALUE & 0xFF;
                INTCONbits.TMR0IF = 0;
                INTCONbits.TMR0IE = 1;
			}

			if (++servoPhase >= 3) {
				servoPhase = 0;
			}
        }

    }//end while
}//end main


#if 0
void TMR0_Initialize (void)
{
    T0CONbits.T08BIT = 0;
    TMR0H = TMR0_RELOAD_VALUE >> 8;
    TMR0L = TMR0_RELOAD_VALUE & 0xFF;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    T0CON = 0x93;
}


void TMR0_InterruptHandler (void)
{
    INTCONbits.TMR0IF = 0;
    TMR0H = TMR0_RELOAD_VALUE >> 8;
    TMR0L = TMR0_RELOAD_VALUE & 0xFF;
    flag250 = 1;
}
#endif


void TMR2_Initialize (void)
{
    PR2 = TMR2_PERIOD;
    TMR2 = 0x00;
    IPR1bits.TMR2IP = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CON = 0x5E;
}


void TMR2_InterruptHandler (void)
{
    PIR1bits.TMR2IF = 0;
    flag250 = 1;
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
