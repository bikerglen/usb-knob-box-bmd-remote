/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <xc.h>
#include <stdbool.h>

#include "fixed_address_memory.h"

#define MAIN_RETURN void

// 250 Hz timer 0 load / reload value
// dec2hex(65535-12e6/16/250) = 0xF447
// #define TMR0_RELOAD_VALUE 0xF447

// 8333 Hz timer 0 load / reload value
// dec2hex(round(65535-12e6/16/8333)) = 0xFFA5
#define TMR0_RELOAD_VALUE 0xFFA5

// 250 Hz timer 2 period value
// dec2hex(12e6/16/12/250-1)
#define TMR2_PERIOD 0xF9

// GPIO input pins
#define ENC1_A   (PORTAbits.RA7 ? 1 : 0)
#define ENC1_B   (PORTAbits.RA6 ? 1 : 0)
#define ENC1_SW  (PORTCbits.RC0 ? 0 : 1)
#define ENC2_A   (PORTDbits.RD5 ? 1 : 0)
#define ENC2_B   (PORTDbits.RD6 ? 1 : 0)
#define ENC2_SW  (PORTDbits.RD4 ? 0 : 1)
#define ENC3_A   (PORTAbits.RA5 ? 1 : 0)
#define ENC3_B   (PORTAbits.RA4 ? 1 : 0)
#define ENC3_SW  (PORTEbits.RE0 ? 0 : 1)
#define ENC4_A   (PORTBbits.RB2 ? 1 : 0)
#define ENC4_B   (PORTBbits.RB1 ? 1 : 0)
#define ENC4_SW  (PORTBbits.RB0 ? 0 : 1)
#define ENC5_A   (PORTBbits.RB5 ? 1 : 0)
#define ENC5_B   (PORTBbits.RB4 ? 1 : 0)
#define ENC5_SW  (PORTBbits.RB3 ? 0 : 1)
#define SW1      (PORTAbits.RA3 ? 0 : 1)
#define SW2      (PORTAbits.RA1 ? 0 : 1)

// GPIO output pins
#define USER_LED LATDbits.LATD7
#define SW1_LED  LATAbits.LATA2
#define SW2_LED  LATAbits.LATA0

// led states
#define LED_ON   0
#define LED_OFF  1


/*** System States **************************************************/
typedef enum {
    SYSTEM_STATE_USB_START,
    SYSTEM_STATE_USB_SUSPEND,
    SYSTEM_STATE_USB_RESUME
} SYSTEM_STATE;

/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state );


/*********************************************************************
* Function: void SYSTEM_Tasks(void)
*
* Overview: Runs system level tasks that keep the system running
*
* PreCondition: System has been initalized with SYSTEM_Initialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
//void SYSTEM_Tasks(void);
#define SYSTEM_Tasks()

void TMR0_Initialize (void);
void TMR0_InterruptHandler (void);

void TMR2_Initialize (void);
void TMR2_InterruptHandler (void);

#endif //SYSTEM_H
