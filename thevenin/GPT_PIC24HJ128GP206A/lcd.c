#include "p24hj128gp206a.h"
#include "lcd.h"


#define OUTPUT_PIN 0
#define OUTPUT_DATA 0x00

#define u8 unsigned char

unsigned char lcdc;
unsigned char LcdBuf[20];

void DelayUs(unsigned int delay)
{
	while(delay > 0)
	{
		delay--;
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");	
	}
}
//---------------------------------------------------------------------------------------------------------
void LCD_STROBE1(void)
{
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_EN1 = 1; 
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_EN1 = 0;
	asm("nop");
	asm("nop");
	asm("nop");
}
//---------------------------------------------------------------------------------------------------------
void LCD_STROBE2(void)
{
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_EN2 = 1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	LCD_EN2 = 0;
	asm("nop");
	asm("nop");
	asm("nop");
}
//---------------------------------------------------------------------------------------------------------
void hj_Data(unsigned char c)	//Put data onto correct pins for 24HJ (hack, since PCB was designed for 24FJ)
{
	unsigned char ctemp;
	unsigned char ccount;
	
	for(ccount = 0; ccount < 8; ccount++)
	{
		ctemp = c;
		ctemp >>= ccount;
		ctemp &= 0b00000001;
		switch(ccount)
		{
			case 0:
				LATGbits.LATG1 = ctemp;
				break;
			case 1:
				LATGbits.LATG0 = ctemp;
				break;
			case 2:
				LATGbits.LATG14 = ctemp;
				break;
			case 3:
				LATGbits.LATG12 = ctemp;
				break;
			case 4:
				LATGbits.LATG13 = ctemp;
				break;
			case 5:
				LATGbits.LATG15 = ctemp;
				break;
			case 6:
				LATCbits.LATC1 = ctemp;
				break;
			case 7:
				LATCbits.LATC2 = ctemp;
				break;
		}
	}
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void lcd_cmd(unsigned char c)	//Send command to LCD
{
	DelayUs(80);

	//LCD_DATA = c;
	hj_Data(c);

	DelayUs(80);

	switch (lcdc)
	{
	  case 0x00:
	    LCD_STROBE1();
	    break;
	  case 0x01:
	    LCD_STROBE2();
	    break;
	  case 0x02:
	    LCD_STROBE1();
	    LCD_STROBE2();
	}
	DelayUs(240);
	DelayUs(100);	
}
//---------------------------------------------------------------------------------------------------------
void lcd_data(unsigned char c)		//Send data to LCD
{
	DelayUs(50);

	//LCD_DATA = 0;
	hj_Data(0);
	LCD_RS = 1;
	//LCD_DATA = c;
	hj_Data(c);
	switch (lcdc)
	{
	  case 0x00:
	    LCD_STROBE1();
	    break;
	  case 0x01:
	    LCD_STROBE2();
	    break;
	  case 0x02:
	    LCD_STROBE1();
	    LCD_STROBE2();
	}
	LCD_RS = 0;
	DelayUs(35);
}
//---------------------------------------------------------------------------------------------------------
void lcd_home(void)
{
	lcdc = 0;
	lcd_cmd(0x2);
	DelayUs(240);
	DelayUs(240);
	DelayUs(240);
}
//---------------------------------------------------------------------------------------------------------
void lcd_line2(void)
{
	lcdc = 0;
	lcd_cmd(0xC0);
	DelayUs(240);
}
//---------------------------------------------------------------------------------------------------------
void lcd_line3(void)
{
	lcdc = 1;
	lcd_cmd(0x2);
	DelayUs(240);
	DelayUs(240);
	DelayUs(240);
}
//---------------------------------------------------------------------------------------------------------
void lcd_line4(void)
{
	lcdc = 1;
	lcd_cmd(0xC0);
	DelayUs(240);
}
//---------------------------------------------------------------------------------------------------------
void lcd_puts(const char * s)		//Send a string to LCD
{
	while(*s)
	lcd_data(*s++);
}
//---------------------------------------------------------------------------------------------------------
void lcd_init(void)
{
	LCD_RS = 0;
	LCD_EN1 = 0;
	LCD_EN2 = 0;
	LCD_RS_TRIS	 = OUTPUT_PIN;
	LCD_EN1_TRIS	 = OUTPUT_PIN;
	LCD_EN2_TRIS	 = OUTPUT_PIN;
	//LCD_DATA_TRIS	 = OUTPUT_DATA;
	
	TRISGbits.TRISG1 = OUTPUT_PIN;	//These are the new data pins for 24HJ pic
	TRISGbits.TRISG0 = OUTPUT_PIN;
	TRISGbits.TRISG14 = OUTPUT_PIN;
	TRISGbits.TRISG12 = OUTPUT_PIN;
	TRISGbits.TRISG13 = OUTPUT_PIN;
	TRISGbits.TRISG15 = OUTPUT_PIN;
	TRISCbits.TRISC1 = OUTPUT_PIN;
	TRISCbits.TRISC2 = OUTPUT_PIN;
	
	DelayUs(240);
	DelayUs(240);
	DelayUs(240);
	
	//LCD_DATA	 = 0x3F;
	hj_Data(0x3F);
	LCD_STROBE1();
	LCD_STROBE2();
	
	DelayUs(240);
	DelayUs(240);
	DelayUs(240);
	
	//LCD_DATA	 = 0x3F;
	hj_Data(0x3F);
	LCD_STROBE1();
	LCD_STROBE2();
	
	DelayUs(240);
	
	//LCD_DATA	 = 0x3F;
	hj_Data(0x3F);
	LCD_STROBE1();
	LCD_STROBE2();
	
	DelayUs(240);
	
	lcdc = 0;
	lcd_cmd(0x38);
	
	DelayUs(240);
	
	lcdc = 1;
	lcd_cmd(0x38);
	
	DelayUs(240);
	
	lcdc = 0;
	lcd_cmd(0xC); //Display On, Cursor Off
	
	DelayUs(240);
	
	lcdc = 1;
	lcd_cmd(0xC); //Display On, Cursor Off
	
	DelayUs(240);
	
	lcdc = 0;
	lcd_cmd(0x1); //Display Clear
	
	DelayUs(240);
	
	lcdc = 1;
	lcd_cmd(0x1); //Display Clear
	
	DelayUs(240);
	
	lcdc = 0;
	lcd_cmd(0x6); //Entry Mode
	
	DelayUs(240);
	
	lcdc = 1;
	lcd_cmd(0x6); //Entry Mode
	
	DelayUs(240);
	
	lcdc = 0;
	lcd_cmd(0x80); //Initialize DDRAM address to zero
	
	DelayUs(240);
	
	lcdc = 1;
	lcd_cmd(0x80); //Initialize DDRAM address to zero
	
	DelayUs(240);
	DelayUs(240);
	
}
//-------------------------------------------------------------------------------------
void NumDisp(unsigned char num)
{
	if(((num/100)%10) > '0')
	{
		lcd_data(((num/100)%10) + '0');
		lcd_data(((num/10)%10) + '0');
		lcd_data('0'+(num%10));
	}
	else
	{
		if(((num/10)%10) > '0')
		{
			lcd_data(((num/10)%10) + '0');
			lcd_data('0'+(num%10));
		}
		else lcd_data('0'+(num%10));
	}
}
//---------------------------------------------------------------------------------------------------------
void LongIntDisp(unsigned long num,unsigned char len)
{
	if(len > 0x0A) len = 0x0A;
	switch(len)
	{
		case 0x0A:
			lcd_data(((num/1000000000)%10) + '0');
		case 0x09:
			lcd_data(((num/100000000)%10) + '0');
		case 0x08:
			lcd_data(((num/10000000)%10) + '0');
		case 0x07:
			lcd_data(((num/1000000)%10) + '0');
		case 0x06:
			lcd_data(((num/100000)%10) + '0');
		case 0x05:
			lcd_data(((num/10000)%10) + '0');
		case 0x04:
			lcd_data(((num/1000)%10) + '0');
		case 0x03:
			lcd_data(((num/100)%10) + '0');
		case 0x02:
			lcd_data(((num/10)%10) + '0');
		case 0x01:
			lcd_data('0'+(num%10));
	}
}
//---------------------------------------------------------------------------------------------------------
void IntDisp(unsigned int num,unsigned char len)
{
	if(len > 5) len = 5;
	switch(len)
	{
		case 0x05:
			lcd_data(((num/10000)%10) + '0');
		case 0x04:
			lcd_data(((num/1000)%10) + '0');
		case 0x03:
			lcd_data(((num/100)%10) + '0');
		case 0x02:
			lcd_data(((num/10)%10) + '0');
		case 0x01:
			lcd_data('0'+(num%10));
	}
}
//---------------------------------------------------------------------------------------------------------
void FracDisp(unsigned int num,unsigned char units,unsigned char decimal)
{
	unsigned char tot, i;
	tot = units + decimal;
	if (tot > 5) tot = 5;
	i = 0;
	switch(tot)
	{
		case 0x05:
			LcdBuf[i] = (((num/10000)%10) + '0');
			i++;
		case 0x04:
			LcdBuf[i] = (((num/1000)%10) + '0');
			i++;
		case 0x03:
			LcdBuf[i] = (((num/100)%10) + '0');
			i++;
		case 0x02:
			LcdBuf[i] = (((num/10)%10) + '0');
			i++;
			LcdBuf[i] = ('0'+(num%10));
			i++;
	}
	for (i = 0; i < units; i++)
	{
		lcd_data(LcdBuf[i]);
	}
	lcd_data('.');
	for (i = units;i < tot;i++)
	{
		lcd_data(LcdBuf[i]);
	}
}
//---------------------------------------------------------------------------------------------------------
void s16IntDisp(int num,unsigned char len)
{
	if(len > 5) len = 5;
	if(num < 0)
	{
		lcd_puts("-");
		num = -num;
	}
	else lcd_data(32);
	switch(len)
	{
		case 0x05:
			lcd_data(((num/10000)%10) + '0');
		case 0x04:
			lcd_data(((num/1000)%10) + '0');
		case 0x03:
			lcd_data(((num/100)%10) + '0');
		case 0x02:
			lcd_data(((num/10)%10) + '0');
		case 0x01:
			lcd_data('0'+(num%10));
	}
}
//---------------------------------------------------------------------------------------------------------
void s16FracDisp(int num,unsigned char units,unsigned char decimal)
{
	unsigned char tot, i;
	if(num < 0)
	{
		lcd_data('-');
		num = -num;
	}
	else lcd_data(32);
	tot = units + decimal;
	if (tot > 5) tot = 5;
	i = 0;
	switch(tot)
	{
		case 0x05:
			LcdBuf[i] = (((num/10000)%10) + '0');
			i++;
		case 0x04:
			LcdBuf[i] = (((num/1000)%10) + '0');
			i++;
		case 0x03:
			LcdBuf[i] = (((num/100)%10) + '0');
			i++;
		case 0x02:
			LcdBuf[i] = (((num/10)%10) + '0');
			i++;
			LcdBuf[i] = ('0'+(num%10));
			i++;
	}
	for (i = 0; i < units; i++)
	{
		lcd_data(LcdBuf[i]);
	}
	lcd_data('.');
	for (i = units;i < tot;i++)
	{
		lcd_data(LcdBuf[i]);
	}
}

