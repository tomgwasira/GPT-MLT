/*********************************************************************************************
*
* File:                 io.c 
* Author:               Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Original code by:     Stanley Adams (stanley@mltinverters.com), MLT Power
* Date:                 February 2020
* Version:              1.00
* 
* Functionality:
* This code implements functions related to I/O on the PIC24HJ128GP206A including I/o pin
* initialisations and other special routines such as toggling the 'heartbeat' LED.
*
*********************************************************************************************/

#include "p24hj128gp206a.h"
#include <xc.h>

#include "global.h"
#include "io.h"

/**
 * Initialises I/O pins. 
 * Sets the directions of all used I/O pins and initialises outputs to correct startup state.
 */
void IO_Init(void) {
	
	HB_LED_tris = OUTPUT;
	
	/* Analog channels */
	TRISBbits.TRISB0 = INPUT;
	TRISBbits.TRISB1 = INPUT;
	TRISBbits.TRISB2 = INPUT;
	TRISBbits.TRISB3 = INPUT;
	TRISBbits.TRISB4 = INPUT;
	TRISBbits.TRISB5 = INPUT;
	TRISBbits.TRISB6 = INPUT;
	TRISBbits.TRISB7 = INPUT;

	/* DAC Outputs */
	_TRISB8 = OUTPUT;
	_TRISB9 = OUTPUT;
	_TRISB10 = OUTPUT;
	_TRISB11 = OUTPUT;
	_TRISB12 = OUTPUT;
	_TRISB13 = OUTPUT;
	_TRISB14 = OUTPUT;
	_TRISB15 = OUTPUT;

	_LATB8 = 0;
	_LATB9 = 0;
	_LATB10 = 0;
	_LATB11 = 0;	
	_LATB12 = 0;
	_LATB13 = 0;
	_LATB14 = 0;
	_LATB15 = 0;	

	AD1PCFGH = 0xFFFF;
	AD1PCFGL = 0xFF00; // select which Analog inputs are in analog mode (0 = analog, 1 = digital)
}

/**
 * Toggles 'heartbeat' LED to indicate that program is running correctly.
 */
void toggleHeartbeatLED(void) {
    if(seconds != prev_seconds)
    {
        HB_LED ^= 1; // toggle 'heartbeat' LED every second
        prev_seconds = seconds;
    }
}