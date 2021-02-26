/* 
 * File:   main.h
 * Author: Thomas
 *
 * Created on 22 February 2021, 10:58 AM
 */

#ifndef MAIN_H
#define	MAIN_H

/* Global variables */
unsigned char i2c_transmitting_samples = 0; // flag to indicate whether MASTER is currently reading I2C line
unsigned char ux_c;


/* Local variables */
unsigned char ux_index;
unsigned int Timer_1ms;
unsigned char Dbg_Start;
unsigned char Dbg_Len;
unsigned char Debug_Buf[32];
unsigned char Debug_cmd;

// states for use of device as I2C slave
typedef enum  {
    STATE_WAIT_FOR_ADDR,                            // waiting for MASTER to request data by sending address and R/W bit
    STATE_WAIT_FOR_BUFFER_READING_DONE_FLAG,        // waiting for MASTER to send a byte (value 0) which serves as a flag to show MASTER is done reading all buffers
    STATE_SEND_DATA_PACKET,                         // sending one sample from each of the buffers to be transmitted i.e. a data packet
    STATE_DONE_SENDING_DATA_PACKET                  // reset routines done after sending a data packet
} STATE;

volatile STATE current_i2c_state = STATE_WAIT_FOR_ADDR; // current I2C state

#endif	/* MAIN_H */

