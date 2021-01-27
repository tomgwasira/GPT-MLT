#include "p24hj128gp206a.h"
#include "p24hxxxx.h"
#include "UART.h"
#include "DEE Emulation 16-bit.h"
#include "tables.h"
#include "flash.h"

//------------------------------------------------------------
//This function writes all the calibration values to flash memory
//------------------------------------------------------------
void Write_cal_flash(void)
{
	unsigned char i;
	
	for(i = 0; i < max_cal_table; i++)
	{
		if(DataEEWrite(calibrations[i],i) != 0) UART_printf("Flash write failed!");
	}
}

//------------------------------------------------------------
//This function reads all the calibration values from flash memory
//If the memory was blank, default values are used, and written
//to the flash memory
//------------------------------------------------------------
void Read_cal_flash(void)
{
	unsigned char i;
	
	for(i = 0; i < max_cal_table; i++)
	{
		calibrations[i] = DataEERead(i);
	}
	if((calibrations[0] < 50) || (calibrations[0] > 4096))	//Calibration values not initialised yet
	{
		Va_cal = 2364;		//Todo: Work out the default values for these
		Vb_cal = 2364;
		Vc_cal = 2364;
		Ia_cal = 351;
		Ib_cal = 351;
		Ic_cal = 351;
		Vbat_cal = 1237;
		Vext_cal = 2633;
		CT_Size = 300;
		Write_cal_flash();	//Write default values to flash
	}
}
