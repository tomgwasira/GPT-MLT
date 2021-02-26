/*********************************************************************************************
*
* File:         main.c 
* Written by:   Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Date:         February 2020
* Version:      1.00
*
* 
* Functionality:
* This code is the entry point for the measurement of Thevenin voltage and current values
* using the PIC24HJ128GP206A and the transmission of these values over an I2C bus.
* Interrupt Service Routines (ISRs) -- for such things as timers, using the microcontroller as
* an I2C SLAVE device etc. -- are also implemented here.
*
* I2R ISR based on:
* https://engineering.purdue.edu/ece477/Archive/2009/Spring/S09-Grp06/Code/PIC/pic24_code
* _examples/docs/textbookexamples.html  [Accessed: Feb 2020]
*
*********************************************************************************************/


/*-----------------------------------------------------------------
 Project Configuration
-----------------------------------------------------------------*/
// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Mode (Primary Oscillator (XT, HS, EC) w/ PLL)
#pragma config IESO = OFF               // Two-speed Oscillator Start-Up Enable (Start up with user-selected oscillator)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>
#include "p24hj128gp206a.h"

#include "main.h"
#include "global.h"

#include "adc.h"
#include "DEE Emulation 16-bit.h"
#include "io.h"
#include "i2c.h"
#include "timers.h"
#include "uart.h"
#include "uart_debug.h"



/*-----------------------------------------------------------------
 Main function
-----------------------------------------------------------------*/
int main(void) {
	
	RCONbits.SWDTEN = 0;                        // disable Watch Dog Timer	

    CLKDIV = 0;
    PLLFBD = 0x0026;
    
	IO_Init();                                  // configure I/O pins
	
	/* Variable initialisations */
	Timer_1ms = 0;
	
    /* Peripheral initialisations */
	UART_Init();                                // initialise serial comms (115200 8-n-1)
	
    ux_index = 0;                               // ask Stanley what this is used for
	
    I2C1_Init();                                // initialise I2C
	ADC_Init();                                 // initialise ADC
	Timer4_Init();                              // initialise Timer 4
	Timer3_Init();                              // initialise Timer 3
	
	Timer_1ms = 1;
    
    debugWelcomeMessage();
	
    /* Infinite loop */
	while(1) {
		asm("clrwdt");
        debuggerDisplay();
        toggleHeartbeatLED();
	} 
    
	return 0;
} 



/*-------------------------------------------------------------------
 Interrupt Service Routines (ISR)
--------------------------------------------------------------------*/
/**
 * Timer 3 Interrupt Service Routine.
 * 128 interrupts per 50 Hz cycle.
 */
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void)
{
	IFS0bits.T3IF = 0; // clear TImer 3 interrupt flag
    addToBuffers(); // obtain values in DMA ADC buffer and place in buffers to be transmitted over I2C. (4 values every 128 interrupts)
}

/**
 * Timer 4 Interrupt Service Routine.
 * Interrupts every 1 ms.
 */
void __attribute__ ((interrupt,no_auto_psv)) _T4Interrupt(void)
{
	IFS1bits.T4IF = 0; // clear TImer 4 Interrupt flag
	Timer_1ms++;
	Timer_1ms %= 60000;

	i2c_timeout++;
	if(i2c_timeout > 1000) // no I2C interrupts for 1 second, reset state machine
	{
		i2c_timeout = 0;
		i2c_state = 0;
		ux_index = 0;
		i2cError ++;
	}	

	if((Timer_1ms % 1000) == 0) seconds++;
		
}

/**
 * UART 2 RX Interrupt Service Routine.
 * UART 2 is used as the debug port.
 */
void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
	unsigned char Debug_offset;
	IFS1bits.U2RXIF = 0; // clear Interrupt flag
	if(U2STAbits.OERR == 1)	U2STAbits.OERR = 0;	// overflow error, ignore it for now
	Debug_offset = Dbg_Start + Dbg_Len;
	Debug_offset &= 0x1F; // same as %= 32
	Debug_Buf[Debug_offset] = U2RXREG;
	if(Debug_Buf[Debug_offset] == 13) // <CR> was received
	{
		Debug_cmd ++;
		UART_printf("Got cmd.\r");
	}
	Dbg_Len++;
}

/**
 * I2C Interrupt Service Routine.
 */
void __attribute__ ((interrupt, no_auto_psv)) _SI2C1Interrupt(void) {
	
    i2cPrepare(); // prepares device for use as I2C slave by doing setup and reset routines.
    
    unsigned char u8_c;
    
    _SI2C1IF = 0;
    
    switch (current_i2c_state) {
        case STATE_WAIT_FOR_ADDR:
            u8_c = I2C1RCV; // clear RBF bit for address
       
            if (I2C1STATbits.R_W) { // check the R/W bit and see if read or write transaction
                i2c_transmitting_samples = 1; // set flag to indicate that I2C is transmitting
                
                I2C1TRN = 0; // transmit a 0 at the beginning of transmission of every set of samples for error checking
                I2C1CONbits.SCLREL = 1; // release clock line  
                
                current_i2c_state = STATE_SEND_DATA_PACKET; // read transaction
            } 
            
            else { current_i2c_state = STATE_WAIT_FOR_BUFFER_READING_DONE_FLAG; }
            break;
        
        case STATE_WAIT_FOR_BUFFER_READING_DONE_FLAG: // waiting for MASTER to send a byte of the value 0 which indicated it is done reading buffers
            doneReadingAllBuffersRoutine();
            current_i2c_state = STATE_WAIT_FOR_ADDR; // wait for next transaction
            break;
       
        case STATE_SEND_DATA_PACKET:            
            if (bytes_transmitted_during_single_i2c_request == (NUMBER_OF_BUFFERS_TO_TRANSMIT << 1)) { // if a 'packet' containing (NUMBER_OF_BUFFERS_TO_TRANSMIT*2) bytes has been transmitted, transmit a 0, reset and go back to initial state to avoid any serious errors in case byte got corrupted while transmitting.
                I2C1TRN = 1; // transmit a 1 at the end of transmission of every set of samples for error checking
                I2C1CONbits.SCLREL = 1; // release clock line
                
                current_i2c_state = STATE_DONE_SENDING_DATA_PACKET; 
            } 
            
            else {
                i2cTransmitBufferElement(); // keep placing reversed characters in buffer as MASTER reads I2C port
            }
            break;
     
        case STATE_DONE_SENDING_DATA_PACKET:  // this is interrupt for last character finished shifting out
            packetSentRoutine();
            current_i2c_state = STATE_WAIT_FOR_ADDR;
            break;
        default:
            current_i2c_state = STATE_WAIT_FOR_ADDR;
   }
}