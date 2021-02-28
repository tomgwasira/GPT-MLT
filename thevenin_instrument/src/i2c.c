/*********************************************************************************************
*
* File:                 i2c.c 
* Author:               Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Original code by:     Stanley Adams (stanley@mltinverters.com), MLT Power
* Date:                 February 2020
* Version:              1.00
* 
* Functionality:
* This code implements the basic functions for use of the PIC24HJ128GP206A as an I2C slave 
* device to transmit buffers containing values of line voltages measured by the ADC.
* All I2C functions are handled in an ISR implemented in main.c.
*
*********************************************************************************************/

#include "p24hj128gp206a.h"

#include "global.h"
#include "i2c.h"



/* Global variables */
unsigned char bytes_transmitted = 0;
unsigned char i2c_error;
unsigned char i2c_state;
unsigned char i2c_status;
unsigned int i2c_timeout;
unsigned char i2c_transmitting = 0;
unsigned int i2c_transmit_buffer_index = 0;
unsigned int i2c_transmit_index = 0;
unsigned char ux_c;

/* Local variables */
volatile SAMPLE_TYPE current_i2c_sample = sample_number_lower_byte; // type of sample currently being transmitted
unsigned char first_transmission = 1; // flag to indicate whether the byte currently being transmitted is the first byte since program started running or since a previous 'done' flag from the MASTER. Used to extract the current index to which the buffers are being written such that transmission of samples will startfrom that index.



/**
* Initialises I2C1.
*/
void I2C1_Init(void) {
	
	I2C1BRG = 49;
	I2C1CON = 0b1001000100000000;
	I2C1ADD = SLAVE_I2C_ADDR>>1; // initialize the address register	
	
	_SI2C1IF = 0;
	_SI2C1IP = 6; // high priority
	_SI2C1IE = 1; // enable ISR
}

/**
 * Prepares device for use as I2C slave by doing setup and reset routines.
 */
void i2cPrepare(void) {
    if(_I2COV)
	{
		ux_c = I2C1RCV; // clear RBF bit for address
		_I2COV = 0;
	}

	i2c_timeout = 0; // reset timeout
	i2c_error = 0;
    i2c_status = ( I2C1STAT & SSPSTAT_BIT_MASK); // mask the status bits out from the other unimportant register bits
}

/**
 * Transmits an element from a specific buffer (depending on what element was previously transmitted) over I2C.
 */
void i2cTransmitBufferElement(void) {
    if (first_transmission) {
       first_transmission = 0;
       i2c_transmit_buffer_index  = buffer_index;
    }
    
    
    switch(current_i2c_sample) {
        case sample_number_lower_byte:
            I2C1TRN = getLowerByte(sample_number_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = sample_number_upper_byte;
            break;
            
        case sample_number_upper_byte:
            I2C1TRN = getHigherByte(sample_number_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V1_lower_byte;
            break;
        
        case V1_lower_byte:
            I2C1TRN = getLowerByte(V1_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V1_upper_byte;
            break;
            
        case V1_upper_byte:
            I2C1TRN = getHigherByte(V1_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V2_lower_byte;
            break;

        case V2_lower_byte:
            I2C1TRN = getLowerByte(V2_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V2_upper_byte;
            break;
            
        case V2_upper_byte:
            I2C1TRN = getHigherByte(V2_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V3_lower_byte;
            break;
           
        case V3_lower_byte:
            I2C1TRN = getLowerByte(V3_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = V3_upper_byte;
            break;
            
        case V3_upper_byte:
            I2C1TRN = getHigherByte(V3_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I1_lower_byte;
            break;
            
        case I1_lower_byte:
            I2C1TRN = getLowerByte(I1_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I1_upper_byte;
            break;
            
        case I1_upper_byte:
            I2C1TRN = getHigherByte(I1_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I2_lower_byte;
            break;
            
        case I2_lower_byte:
            I2C1TRN = getLowerByte(I2_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I2_upper_byte;
            break;
            
        case I2_upper_byte:
            I2C1TRN = getHigherByte(I2_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I3_lower_byte;
            break;
            
        case I3_lower_byte:
            I2C1TRN = getLowerByte(I3_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = I3_upper_byte;
            break;
        
        case I3_upper_byte:
            I2C1TRN = getHigherByte(I3_buffer[i2c_transmit_buffer_index ]);
            current_i2c_sample = sample_number_lower_byte;
            break;  
          
        default:
            break;
    }
    
    I2C1CONbits.SCLREL = 1; // release clock line so MASTER can drive it
    bytes_transmitted++; // increment counter for bytes transmitted during a single i2c request
}

/**
 * Returns the most significant byte in a 16-bit int.
 */
char getHigherByte(int x) {
    return ((x >> 8) & 0xFF);
}

/**
 * Returns the least significant byte in a 16-bit int.
 */
char getLowerByte(int x) {
    return (x & 0xFF);
}

/**
 * Resets variables and other things to state they were in when the device was waiting for MASTER to transmit address.
 */
void packetSentRoutine(void) {
    I2C1CONbits.SCLREL = 1; // release clock line 
    i2c_transmit_buffer_index ++; // increase index of samples being transmitted
    if (i2c_transmit_buffer_index  >= BUFFER_SIZE) { // wrap index around back to 0 so that it does not try to index array out of bounds
        i2c_transmit_buffer_index  = 0;
    }
    
    current_i2c_sample = sample_number_lower_byte;
    bytes_transmitted = 0;
}

/**
 * Does reset routines for variables when MASTER sends flag to indicate that it is done reading sets of samples for all buffers
 */
void doneReadingAllBuffersRoutine(void) {
    i2c_transmitting = I2C1RCV;  // when MASTER is done reading I2C port, it will send a 0 byte which should reset i2c_transmitting variable
    
    if (!i2c_transmitting) {
        first_transmission = 1; // reset flag used to indicate that a transmission is the first of a request for a set of samples
    }
}



/* The following functions are not being used. Not sure why they were included. */
/**
 * Performs an I2C start operation.
 */
void startI2C1(void) {
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1; //enable WDT
  I2C1CONbits.SEN = 1;   // initiate start
  // wait until start finished
  while (I2C1CONbits.SEN);
  _SWDTEN = u8_wdtState;  //restore WDT

}

/**
 * Performs an I2C repeated start operation.
 */
void rstartI2C1(void) {// repeated start
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1;  //enable WDT
  I2C1CONbits.RSEN = 1;   // initiate start
  // wait until start finished
  while (I2C1CONbits.RSEN);
  _SWDTEN = u8_wdtState;  //restore WDT

}

/**
 * Performs an I2C stop operation.
 */
void stopI2C1(void) {
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1;  //enable WDT
  I2C1CONbits.PEN=1;     // initiate stop, PEN=1
  //wait until stop finished
  while (I2C1CONbits.PEN);
  _SWDTEN = u8_wdtState;  //restore WDT
}