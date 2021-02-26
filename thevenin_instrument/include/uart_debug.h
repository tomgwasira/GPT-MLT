/* 
 * File:   uart_debug.h
 * Author: Thomas
 *
 * Created on 28 January 2021, 2:39 PM
 */

#ifndef UART_DEBUG_H
#define	UART_DEBUG_H
//
//extern unsigned char Dbg_Start;
//extern unsigned char Dbg_Len;
//extern unsigned char Debug_Buf[32];
//extern unsigned char Debug_cmd;
//extern unsigned char Bat_UV;
//extern unsigned char Inv_OL;
//extern unsigned char Inv_SC;
//extern unsigned char Inv_OT;
//extern unsigned char INV_MODE;
//extern unsigned char SYS_MODE;
//extern int calibrations[max_cal_table];
//extern int Data_table[max_Data_table];

void debuggerDisplay(void);
void printAllMeasuredData(void);
void debugWelcomeMessage(void);
//void Debug_Inv_Mode(unsigned char Jumper_Pos);
//void Debug_LCD(void);
//void Parse_debug_buffer(void);

#endif	/* UART_DEBUG_H */

