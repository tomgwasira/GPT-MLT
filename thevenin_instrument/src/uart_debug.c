/*********************************************************************************************
*
* File:                 uart_debug.c 
* Author:               Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Original code by:     Stanley Adams (stanley@mltinverters.com), MLT Power
* Date:                 February 2020
* Version:              1.00
* 
* Functionality:
* This code implements the basic initialisations, configurations and routines for use of the 
* UART serial port as a debug tool. This will, possibly, require the installation of Virtual
* COM Port drivers and use of a serial communication client such as PuTTY for Windows.
*
* This code contains incomplete functions because during the development of Version 1.00 of
* this project, the UART debug feature was never actually used.
*
*********************************************************************************************/


#include "p24hj128gp206a.h"

#include "DEE Emulation 16-bit.h"
#include "global.h"
#include "uart_debug.h"


/**
 * UART initialisation with RS-232
 */
void UART_Init(void) {

	U2MODE = 0x8000;		// Reset UART to 8-n-1, and enable
	U2BRG	= 86;			// Baud Rate Generator Prescaler       
	U2MODEbits.BRGH = 1;	// high speed UART (affects BRGx calculation, /4 if BRGH=1 /16 if BRGH=0)
	U2STA  = 0x0440;		// Reset status register and enable TX & RX
	IFS1bits.U2RXIF = 0;	// Clear UART RX Interrupt Flag
	U2STAbits.URXISEL = 0;	// interrupt when any character is recieved
	IEC1bits.U2RXIE = 1;	// Enable receive interrupt
}

/**
 * Displays some information on start up.
 */
void debugWelcomeMessage(void)
{
    uartPrintf("Thevenin Instrument Line Voltages and Currents");
}

/**
 * Displays useful information for development of Thevenin instrument through UART port.
 */
void debuggerDisplay(void) {

}

/**
 * Prints all data measured by ADC. Equivalent of case -AD in Parse_debug_buffer function.
 */
void printAllMeasuredData(void) {

}

/**
 * Transmits a single byte on the UART port.
 * @param byte Byte to be transmitted on UART port.
 */
void uartCharTx(unsigned char byte) {
	while(U2STAbits.TRMT==0); // wait till transmit shift register empty
	U2TXREG = byte;
}

/**
 * Transmits a string on the UART port.
 * @param input_string String to be transmitted on UART port.
 */
void uartPrintf(const char *input_string) {
  unsigned char count = 0;
  while (input_string[count])
    {uartCharTx(input_string[count]);
	   count++;
   	}
}