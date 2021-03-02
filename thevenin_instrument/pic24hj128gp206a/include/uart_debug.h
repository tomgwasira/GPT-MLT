/* 
 * File:   uart_debug.h
 * Author: Thomas
 *
 * Created on 28 January 2021, 2:39 PM
 */

#ifndef UART_DEBUG_H
#define	UART_DEBUG_H

void UART_Init(void);
void debugWelcomeMessage(void);
void debuggerDisplay(void);
void printAllMeasuredData(void);
void uartCharTx(unsigned char byte);
void uartPrintf(const char *input_string);

#endif	/* UART_DEBUG_H */

