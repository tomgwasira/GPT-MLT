/*******************************************************************************
*
* UART debug port routines and configurations for Thevenin instrument. 
* Initialisations and other UART setup routines are done in uart.c.
*
* File: uart_debug.c
* Author: Thomas Gwasira (tomgwasira@gmail.com), MLT Inverters
* Original code by: Stanley Adams, MLT Power
* Date: 28 January 2020
*
*******************************************************************************/
#include "p24hj128gp206a.h"

//#include "adc.h"
#include "DEE Emulation 16-bit.h"
//#include "flash.h"
//#include "tables.h"
#include "global.h"
//#include "uart.h"
#include "uart_debug.h"

/**
 * Displays useful information for development of Thevenin instrument through UART port.
 */
void debuggerDisplay(void)
{
//    UART_Int_TX(i2c_transmitting, 3);
//    UART_char_Tx(i2c_transmitting);
//    printAllMeasuredData();   
//    RCON = 0;	
}

/**
 * Prints all data measured by ADC. Equivalent of case -AD in Parse_debug_buffer function
 */
void printAllMeasuredData(void) {
//    UART_Int_TX(V2, 3);
//    UART_printf("\r");
}

/**
 * Displays some information on start up.
 */
void debugWelcomeMessage(void)
{
//    UART_printf("Thevenin Instrument Line Voltages and Currents");
    
//	UART_printf("MLT Inverters - Site controller V");
//	UART_Frac_TX(Version,1,2);
//	UART_printf("\r");
//	UART_printf(__DATE__);
//	UART_printf("\r");
//	UART_printf(__TIME__);
//	UART_printf("\r");
//	UART_printf("\rEnter -h , -H or -? for help\r\r");
//	RCON = 0;	
}

///**
// * Checks the Debug buffer for a command and acts upon it.
// */
//
//void Parse_debug_buffer(void)
//{
//	unsigned char timeout;
//	unsigned char cmd_len = 0;
//	unsigned char i;
//	unsigned int rx_num;
//	unsigned int rx_num2;
//	
//	timeout = 0;
//	while(Debug_Buf[Dbg_Start] != '-')
//	{
//		Dbg_Start++;
//		if(Dbg_Len > 0) Dbg_Len--;
//		Dbg_Start &= 0x1F;
//		timeout++;
//		if(timeout > 31) break;	// something is wrong	
//	}
//	
//	if(Debug_Buf[Dbg_Start] == '-')	// start of a command
//	{
//		switch(Debug_Buf[(Dbg_Start + 1) & 0x1F])
//		{
//			case 'h':
//			case 'H':
//			case '?':
//				cmd_len = 3;	//-h<CR> = 3 chars
//				UART_printf("The following commands are accepted:\r");
//				UART_printf("-h / -H / -? : Help\r");
//				UART_printf("-AS : Shows all setpoint values\r");
//				UART_printf("-AD : Shows all data\r");
//				UART_printf("-AV : Shows all Voltages\r");
//				UART_printf("-AA : Shows all currents\r");
//				UART_printf("-AP : Shows all powers\r");
//				UART_printf("-USx y : Update setpoint x with value y\r");
//				break;
//			case 'a':
//			case 'A':
//				cmd_len = 4;	//-Ax<CR> = 4 chars
//				switch(Debug_Buf[(Dbg_Start + 2) & 0x1F])
//				{
//					case 'S':
//					case 's':
//						for(i = 0; i < max_cal_table; i++)
//						{
//							UART_Int_TX(i,2);
//							UART_char_Tx(' ');
////							UART_printf(Cal_names[i].Tag);
//							UART_printf("\t\t");
//							if(Cal_names[i].Scale > 0)
//							{
//								UART_Frac_TX(calibrations[i], (5 - Cal_names[i].Scale), Cal_names[i].Scale);	
//							}
//							else
//							{
//								UART_S16_Tx(calibrations[i]);
//							}
//							UART_printf(Cal_names[i].Units);
//							UART_char_Tx(13);
//						}
//						break;	
//					case 'd':
//					case 'D':
//						for(i = 0; i < max_Data_table; i++)
//						{
//							UART_printf(Data_names[i].Tag);
//							UART_printf("\t\t");
//							if(Data_names[i].Scale > 0)
//							{
//								UART_SFrac_TX(Data_table[i], Data_names[i].Scale);	
//							}
//							else
//							{
//								UART_S16_Tx(Data_table[i]);
//							}
//							UART_printf(Data_names[i].Units);
//							UART_char_Tx(13);
//						}	
//						break;
//					case 'b':
//					case 'B':
//						for(i = 0; i < (max_Data_table * 2); i++)
//						{
//							if((i % 2) == 0) UART_S16_Tx((Data_table[i >> 1] >> 8) & 0xFF);
//							else UART_S16_Tx((Data_table[i >> 1]) & 0xFF);
//							UART_char_Tx(13);
//						}	
//						break;
//					case 'v':
//					case 'V':
//						for(i = 0; i < max_Data_table; i++)
//						{
//							if((Data_names[i].Units[0] == 'V') && (Data_names[i].Units[1] != 'A'))
//							{
//								UART_printf(Data_names[i].Tag);
//								UART_printf("\t\t");
//								if(Data_names[i].Scale > 0)
//								{
//									UART_SFrac_TX(Data_table[i], Data_names[i].Scale);	
//								}
//								else
//								{
//									UART_S16_Tx(Data_table[i]);
//								}
//								UART_printf(Data_names[i].Units);
//								UART_char_Tx(13);
//							}
//						}	
//						break;
//					case 'a':
//					case 'A':
//						for(i = 0; i < max_Data_table; i++)
//						{
//							if(Data_names[i].Units[0] == 'A')
//							{
//								UART_printf(Data_names[i].Tag);
//								UART_printf("\t\t");
//								if(Data_names[i].Scale > 0)
//								{
//									UART_SFrac_TX(Data_table[i], Data_names[i].Scale);	
//								}
//								else
//								{
//									UART_S16_Tx(Data_table[i]);
//								}
//								UART_printf(Data_names[i].Units);
//								UART_char_Tx(13);
//							}
//						}	
//						break;
//					case 'p':
//					case 'P':
//						for(i = 0; i < max_Data_table; i++)
//						{
//							if(((Data_names[i].Units[0] == 'V') && (Data_names[i].Units[1] == 'A')) || (Data_names[i].Units[0] == 'W'))
//							{
//								UART_printf(Data_names[i].Tag);
//								UART_printf("\t\t");
//								if(Data_names[i].Scale > 0)
//								{
//									UART_SFrac_TX(Data_table[i], Data_names[i].Scale);	
//								}
//								else
//								{
//									UART_S16_Tx(Data_table[i]);
//								}
//								UART_printf(Data_names[i].Units);
//								UART_char_Tx(13);
//							}
//						}	
//						break;
//				}
//				break;
//			case 'u':
//			case 'U':
//				cmd_len = 5;	//-USx<CR> = 5 (Length of numbers must still be added
//				switch(Debug_Buf[(Dbg_Start + 2) & 0x1F])
//				{
//					case 's':
//					case 'S':
//						rx_num = (Debug_Buf[(Dbg_Start + 3) & 0x1F]) - '0';
//						i = 4;
//						timeout = 0;
//						while(Debug_Buf[(Dbg_Start + i) & 0x1F] != ' ')
//						{
//							rx_num *= 10;
//							rx_num += (Debug_Buf[(Dbg_Start + i) & 0x1F]) - '0';
//							i++;
//							cmd_len++;
//							timeout++;
//							if(timeout > 3) break;	//Something is wrong
//						}
//						i++;
//						cmd_len++;
//						rx_num2 = (Debug_Buf[(Dbg_Start + i) & 0x1F]) - '0';
//						i++;
//						cmd_len++;
//						timeout = 0;
//						while(Debug_Buf[(Dbg_Start + i) & 0x1F] != 13)
//						{
//							rx_num2 *= 10;
//							rx_num2 += (Debug_Buf[(Dbg_Start + i) & 0x1F]) - '0';
//							i++;
//							cmd_len++;
//							if(timeout > 3) break;	//Something is wrong
//						}
//						UART_printf("Command echo: -US");
//						UART_Int_TX(rx_num,2);
//						UART_char_Tx(' ');
//						UART_Int_TX(rx_num2,5);
//						UART_char_Tx(13);
//						if(rx_num < max_cal_table)
//						{
//							calibrations[rx_num] = rx_num2;
//							Write_cal_flash();	//Write new calibration values to flash
//						}
//						else
//						{
//							UART_printf("Invalid setpoint\r");	
//						}
//						break;
//				}
//				break;
//			case 'r':
//			case 'R':
//				cmd_len = 5;	//-USx<CR> = 5 (Length of numbers must still be added
//				switch(Debug_Buf[(Dbg_Start + 2) & 0x1F])
//				{
//					case 'r':	//Reset instruction
//					case 'R':
//						asm("reset"); //reset myself
//						break;
//				}
//				break;
//		}
//		Dbg_Start += cmd_len;
//		Dbg_Len -= cmd_len;
//		Dbg_Start &= 0x1F;
//		//Debug_cmd = 0;
//	}
//}
