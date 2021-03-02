/*********************************************************************************************
*
* File:                 timers.c 
* Author:               Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Original code by:     Stanley Adams (stanley@mltinverters.com), MLT Power
* Date:                 February 2020
* Version:              1.00
* 
* Functionality:
* This code implements the basic routines for timers used for this project: Timer 3 and 
* Timer 4. Timer 3 is used for an interrupt which causes measured ADC values to be added to
* buffers; therefore, the period of this specific timer determines the so-called 'sample rate'
* for the entire project.
*
*********************************************************************************************/

#include "p24hj128gp206a.h"

#include "global.h"
#include "timers.h"



/* Global variables */
unsigned char seconds;
unsigned char prev_seconds;

/* Local variables */
unsigned int PR3_50Hz;



/**
 * Timer 4 initialisation.
 * Timer 4 is used as a general purpose timer.
 */
void Timer4_Init(void) {
	
	PR4 = 0x9C40; // set up Timer 4 for 1 ms operation

	IPC6bits.T4IP = 1; // setup Timer 4 interrupt priority
	IFS1bits.T4IF = 0; // clear Timer 4 interrupt flag
	IEC1bits.T4IE = 1; // enable Timer 4 interrupts

	T4CONbits.TON = 1; // start Timer 4
}

/**
 * Timer 3 initialisation.
 * Timer 3 is used to trigger interrupts that cause measured ADC values to be added to buffers.
 */
void Timer3_Init(void) {
	unsigned long PR3_temp, PR3_temp2;
	
	PR3_temp = 400000000; // frequency on crystal. Multiplied by 10, because output frequency has 1 decimal
	PR3_temp2 = (unsigned long)(500); // approximate grid frequency
	PR3_temp2 *= (unsigned long)(128); // approximately 128 interrupts per cycle. Will not be exactly this because signal frequency is not exactly 50 Hz.
	PR3_temp /= PR3_temp2; // Period Register 3 value is 40000000/(128*Output_freq)
	PR3_50Hz = (unsigned int)(PR3_temp); // nominal frequency
	
	PR3 = PR3_50Hz; // initialise Period Register with nominal calculated value. This will be the period at which the timer will run
	
	IPC2bits.T3IP = 5; // setup Timer 3 interrupt priority
	IFS0bits.T3IF = 0; // clear Timer 3 interrupt flag
	IEC0bits.T3IE = 1; // enable Timer 3 interrupts

	T3CONbits.TON = 1; // start Timer 3
}

