/* 
 * File:   uart.h
 * Author: Thomas
 *
 * Created on 29 January 2021, 11:03 AM
 */

#ifndef UART_H
#define	UART_H

#include "p24hj128gp206a.h"


#define SLAVE_I2C_ADDR 0x60
#define BUFSIZE 64

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA,
  STATE_SEND_READ_LAST
} STATE;

void UART_Init(void);
void i2c_Init(void);
void UART_char_Tx(unsigned char ch);
void UART_printf(const char *instring);
void UART_num_Tx(unsigned long bigNumber);
void UART_Int_TX(unsigned int num,unsigned char len);
void UART_Frac_TX(unsigned int num,unsigned char units,unsigned char decimal);
void UART_S16_Tx(int num);
void UART_SFrac_TX(int num, unsigned char decimal);

#endif	/* UART_H */

