/*********************************************************************************************
*
* File:                 global.h 
* Author:               Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Original code by:     Stanley Adams (stanley@mltinverters.com), MLT Power
* Date:                 February 2020
* Version:              1.00
*
* 
* Functionality:
* This header file contains global variables and constant definitions to be shared across all
* files. This global variables are marked extern here and are also defined and initialised in
* the source file where it is most appropriate to initialise them. The variables can then be
* used in any other source file.
*
*********************************************************************************************/

#ifndef GLOBAL_H
#define	GLOBAL_H

/* Constant definitions */
#define BUFFER_SIZE 256
#define NUMBER_OF_BUFFERS_TO_TRANSMIT 7
#define SLAVE_I2C_ADDR 0x60
#define SSPSTAT_BIT_MASK 0b00111111 // mask for I2C status
#define HB_LED LATFbits.LATF0
#define HB_LED_tris TRISFbits.TRISF0
#define OUTPUT 0
#define INPUT 1

/* ADC */
extern unsigned int buffer_index;
extern unsigned int sample_number_buffer[BUFFER_SIZE];
extern int V1_buffer[BUFFER_SIZE];
extern int V2_buffer[BUFFER_SIZE];
extern int V3_buffer[BUFFER_SIZE];
extern int I1_buffer[BUFFER_SIZE];
extern int I2_buffer[BUFFER_SIZE];
extern int I3_buffer[BUFFER_SIZE];

/* IO */


/* I2C */
extern unsigned char bytes_transmitted; // bytes transmitted so far over i2c in STATE_SEND_DATA_PACKET. Used to determine whether a data 'packet' has been transmitted.
extern unsigned int sample_count_since_i2c_start; // number of samples added to buffers since I2C started transmitting. Used to pause adding to buffer if number of samples exceeded buffer size.
extern unsigned char i2c_error;
extern unsigned char i2c_state;
extern unsigned char i2c_status;
extern unsigned int i2c_timeout;
extern unsigned char i2c_transmitting; // flag to indicate that MASTER is reading and has not sent a 'done' flag yet
extern unsigned int i2c_transmit_buffer_index ; // index for adding to buffers when I2C started reading
extern unsigned char ux_c; // variable to store address of device received from MASTER

/* Timers */
extern unsigned char seconds;
extern unsigned char prev_seconds;

#endif	/* GLOBAL_H */

