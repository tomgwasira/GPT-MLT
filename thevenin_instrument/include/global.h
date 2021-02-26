/*********************************************************************************************
*
* File:         global.h 
* Written by:   Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Date:         February 2020
* Version:      1.00
*
* 
* Functionality:
* This header file contains global variables and constant definitions to be shared across all
* files. This global variables are marked extern here and are also defined and initialised in
* the source file where it is most appropriate to initialise them. The variables can then be
* used in any other source file.
*
*********************************************************************************************/

#ifndef THEVENIN_INSTRUMENT_H
#define	THEVENIN_INSTRUMENT_H

/* Constant definitions */
#define Version 100
#define BUFFER_SIZE 256
#define NUMBER_OF_BUFFERS_TO_TRANSMIT 7
#define HB_LED				LATFbits.LATF0
#define HB_LED_tris			TRISFbits.TRISF0
#define OUTPUT 0
#define INPUT 1


/* IO */
extern unsigned int sample_number_buffer[BUFFER_SIZE];
extern int V1_buffer[BUFFER_SIZE];
extern int V2_buffer[BUFFER_SIZE];
extern int V3_buffer[BUFFER_SIZE];
extern int I1_buffer[BUFFER_SIZE];
extern int I2_buffer[BUFFER_SIZE];
extern int I3_buffer[BUFFER_SIZE];

/* I2C */
#define SLAVE_I2C_ADDR 0x60
#define SSPSTAT_BIT_MASK 0b00111111 // mask for I2C status
extern unsigned char bytes_transmitted_during_single_i2c_request;
extern unsigned char i2c_transmitting_samples;
extern unsigned int i2c_transmit_index;

extern unsigned char i2c_state;
extern unsigned char i2c_status;
extern unsigned char i2cdebug;
extern unsigned char i2cdebug2;
extern unsigned char i2cdebug3;
extern unsigned char i2cdebug4;
extern unsigned char i2cdebug5;
extern unsigned char i2cError;
extern unsigned int i2c_timeout;
extern unsigned char ux_c;

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define s16 int
#define s32 long

/*-----------------------------------------------------------------
 Timers
-----------------------------------------------------------------*/
extern unsigned char seconds;
extern unsigned char prev_seconds;





extern char i2c_transmitting;
extern unsigned int buffer_index;
extern unsigned int i2c_transmit_buffer_index ; // index for adding to buffers when I2C started reading
extern unsigned int sample_count_since_i2c_start; // number of samples added to buffers since I2C started transmitting. Used to pause adding to buffer if number of samples exceeded buffer size.
extern unsigned int waveTxCounter; // index used by I2C to transmit buffers sample by sample
extern char pause_adc;

//extern char pause_adc = 0; // flag to pause writting to buffers when I2C is transmitting in order to avoid overwriting values before they are transmitted

#endif	/* THEVENIN_INSTRUMENT_H */

