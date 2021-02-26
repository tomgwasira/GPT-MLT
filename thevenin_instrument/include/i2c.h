/* 
 * File:   i2c.h
 * Author: Thomas
 *
 * Created on 11 February 2021, 10:53 AM
 */

#ifndef I2C_H
#define	I2C_H


typedef enum  {
    sample_number_lower_byte,
    sample_number_upper_byte,
            
    V1_lower_byte,
    V1_upper_byte,

    V2_lower_byte,
    V2_upper_byte,
            
    V3_lower_byte,
    V3_upper_byte,
            
    I1_lower_byte,
    I1_upper_byte,
            
    I2_lower_byte,
    I2_upper_byte,
            
    I3_lower_byte,
    I3_upper_byte
} SAMPLE_TYPE; // states for state machine to use device as I2C SLAVE

void I2C1_Init(void);
void i2cPrepare(void);
void i2cTransmitErrorCheckingByte(void);
void i2cTransmitBufferElement(void);
void packetSentRoutine(void);
void doneReadingAllBuffersRoutine(void);
char getHigherByte(int x);
char getLowerByte(int x);

#endif	/* I2C_H */