/*******************************************************************************
*
* Timer routines and configurations for Thevenin Instrument software. 
*
* File: timers.c
* Author: Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Date: 28 January 2020
*
*******************************************************************************/

#include "p24hj128gp206a.h"

#include "global.h"
#include "timers.h"
//#include "tables.h"
//#include "UART.h"

//unsigned int Timer_1ms;
unsigned int PR3_calc;
unsigned int PR3_50Hz;
unsigned int PR3_53Hz;


/*-----------------------------------------------------------------
 Global variables
-----------------------------------------------------------------*/
unsigned char seconds;
unsigned char prev_seconds;

//-----------------------------------------------------------------
//Timer 4 Initialisation
//Timer 4 will be used as a general purpose timer
//-----------------------------------------------------------------
void Timer4_Init(void)
{
	
	PR4 = 0x9C40;	//Set up Timer 4 for 1mS operation

	IPC6bits.T4IP = 1; // Setup Timer 4 interrupt priority
	IFS1bits.T4IF = 0; // Clear Timer 4 interrupt flag
	IEC1bits.T4IE = 1; // Enable Timer 4 interrupts

	T4CONbits.TON = 1; // Start Timer 4
}

//-----------------------------------------------------------------
//Timer 3 Initialisation
//-----------------------------------------------------------------
void Timer3_Init(void)
{
	unsigned long PR3_temp, PR3_temp2;
	
	PR3_temp = 400000000;							//Multiplied by 10, because Output_freq has 1 decimal
	PR3_temp2 = (unsigned long)(500);	
	PR3_temp2 *= (unsigned long)(128);				//128 interrupts per cycle
	PR3_temp /= PR3_temp2;							//PR3 value is 40000000/(128*Output_freq)
	PR3_calc = (unsigned int)(PR3_temp);
	PR3_50Hz = PR3_calc;							//Nominal frequency
	
	PR3 = PR3_50Hz;		//Initialise PR3 with nominal calculated value
	
	IPC2bits.T3IP = 5; // Setup Timer 3 interrupt priority
	IFS0bits.T3IF = 0; // Clear Timer 3 interrupt flag
	IEC0bits.T3IE = 1; // Enable Timer 3 interrupts

	T3CONbits.TON = 1; // Start Timer 3
}

