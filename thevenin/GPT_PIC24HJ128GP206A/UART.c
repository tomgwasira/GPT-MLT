#include "p24hj128gp206a.h"
#include "UART.h"

//-----------------------------------------------------------------
//RS-232 Initialisation
//-----------------------------------------------------------------
void UART_Init(void)
{

	U2MODE = 0x8000;		// Reset UART to 8-n-1, and enable
	U2BRG	= 86;			// Baud Rate Generator Prescaler       
	U2MODEbits.BRGH = 1;	// high speed UART (affects BRGx calculation, /4 if BRGH=1 /16 if BRGH=0)
	U2STA  = 0x0440;		// Reset status register and enable TX & RX
	IFS1bits.U2RXIF = 0;	// Clear UART RX Interrupt Flag
	U2STAbits.URXISEL = 0;	// interrupt when any character is recieved
	IEC1bits.U2RXIE = 1;	// Enable receive interrupt

/*	
	U1MODE = 0x8000;	// Reset UART to 8-n-1, and enable
	U1BRG	= 86;	// Baud Rate Generator Prescaler       
	U1MODEbits.BRGH = 1;	// high speed UART (affects BRGx calculation, /4 if BRGH=1 /16 if BRGH=0)
	U1STA  = 0x0440;	// Reset status register and enable TX & RX
	IFS0bits.U1RXIF = 0;	// Clear UART RX Interrupt Flag
	U1STAbits.URXISEL = 0;	// interrupt when any character is recieved
	IEC0bits.U1RXIE = 1;	// Enable receive interrupt
*/
}

//-----------------------------------------------------------------
//Perform an I2C start operation.
//-----------------------------------------------------------------
void startI2C1(void) {
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1; //enable WDT
  I2C1CONbits.SEN = 1;   // initiate start
  // wait until start finished
  while (I2C1CONbits.SEN);
  _SWDTEN = u8_wdtState;  //restore WDT

}

//-----------------------------------------------------------------
//Perform an I2C repeated start operation.
//-----------------------------------------------------------------
void rstartI2C1(void) {// repeated start
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1;  //enable WDT
  I2C1CONbits.RSEN = 1;   // initiate start
  // wait until start finished
  while (I2C1CONbits.RSEN);
  _SWDTEN = u8_wdtState;  //restore WDT

}

//-----------------------------------------------------------------
//Perform an I2C stop operation.
//-----------------------------------------------------------------
void stopI2C1(void) {
  unsigned char u8_wdtState;

  u8_wdtState = _SWDTEN;  //save WDT state
  _SWDTEN = 1;  //enable WDT
  I2C1CONbits.PEN=1;     // initiate stop, PEN=1
  //wait until stop finished
  while (I2C1CONbits.PEN);
  _SWDTEN = u8_wdtState;  //restore WDT

}
//-----------------------------------------------------------------
//i2c Initialisation
//-----------------------------------------------------------------
void i2c_Init(void)
{
	
	I2C1BRG = 49;
	I2C1CON = 0b1001000100000000;
	I2C1ADD = SLAVE_I2C_ADDR>>1;   //initialize the address register	
	
	_SI2C1IF = 0;
	_SI2C1IP = 6;				//High priority
	_SI2C1IE = 1;                //enable ISR
	
}

//-----------------------------------------------------------------
//Transmits a single byte on the debug port
//-----------------------------------------------------------------
void UART_char_Tx(unsigned char ch)
{

	while(U2STAbits.TRMT==0);//wait till tx shift register empty
	U2TXREG =ch;

/*
	while(U1STAbits.TRMT==0);//wait till tx shift register empty
	U1TXREG =ch;
*/
}

//-----------------------------------------------------------------
//Transmits a string on the debug port
//-----------------------------------------------------------------
void UART_printf(const char *instring) 
{
  
  unsigned char count=0;
  while (instring[count])
    {UART_char_Tx(instring[count]);
	   count++;
   	}
  
}

//-----------------------------------------------------------------
//Transmit an unsigned long as readable text on the debug port
//-----------------------------------------------------------------
void UART_num_Tx(unsigned long bigNumber)
{
    unsigned long big=bigNumber;
    unsigned long ch=0;
    char flag=0;
    char flag0=0;
    unsigned long div = (unsigned long)1e9;
    while (div>=1)
    {
      ch=(big/div);
      if (ch!=0)
      {
        flag=1;
      }
      if (flag==1)
      {
        flag0=1;
        UART_char_Tx(ch+48);
        big=(big-(div*ch));
      }
      div=(div/10);
    }
    if (flag0==0)
    {
      UART_char_Tx('0');
    }
}

//-----------------------------------------------------------------
//Transmit an unsigned int as readable text on the debug port
//the no. of digits displayed can be set
//-----------------------------------------------------------------
void UART_Int_TX(unsigned int num,unsigned char len)
{
	if(len > 5) len = 5;
	switch(len)
	{
		case 0x05:
			UART_char_Tx(((num/10000)%10) + '0');
		case 0x04:
			UART_char_Tx(((num/1000)%10) + '0');
		case 0x03:
			UART_char_Tx(((num/100)%10) + '0');
		case 0x02:
			UART_char_Tx(((num/10)%10) + '0');
		case 0x01:
			UART_char_Tx('0'+(num%10));
	}
}

//-----------------------------------------------------------------
//Transmit an unsigned int as a fraction on the debug port
//the number of digits before and after the decimal can be set
//-----------------------------------------------------------------
void UART_Frac_TX(unsigned int num,unsigned char units,unsigned char decimal)
{
	unsigned char tot, i;
	unsigned char TxBuf[6];
	tot = units + decimal;
	if (tot > 5) tot = 5;
	i = 0;
	switch(tot)
	{
		case 0x05:
			TxBuf[i] = (((num/10000)%10) + '0');
			i++;
		case 0x04:
			TxBuf[i] = (((num/1000)%10) + '0');
			i++;
		case 0x03:
			TxBuf[i] = (((num/100)%10) + '0');
			i++;
		case 0x02:
			TxBuf[i] = (((num/10)%10) + '0');
			i++;
			TxBuf[i] = ('0'+(num%10));
			i++;
	}
	for (i = 0; i < units; i++)
	{
		UART_char_Tx(TxBuf[i]);
	}
	UART_char_Tx('.');
	for (i = units;i < tot;i++)
	{
		UART_char_Tx(TxBuf[i]);
	}
}

//-----------------------------------------------------------------
//Transmit a signed int on the debug port
//-----------------------------------------------------------------
void UART_S16_Tx(int num)
{
	if(num < 0)
	{
		UART_char_Tx('-');
		num = -num;
	}
	UART_char_Tx(((num/10000)%10) + '0');
	UART_char_Tx(((num/1000)%10) + '0');
	UART_char_Tx(((num/100)%10) + '0');
	UART_char_Tx(((num/10)%10) + '0');
	UART_char_Tx('0'+(num%10));	
}

//-----------------------------------------------------------------
//Transmit a signed int as a fraction on the debug port
//the number of digits after the decimal can be set
//-----------------------------------------------------------------
void UART_SFrac_TX(int num,unsigned char decimal)
{
	unsigned char i,units;
	unsigned char TxBuf[6];
	
	if(num < 0)
	{
		UART_char_Tx('-');
		num = -num;
	}
	
	i = 0;
	TxBuf[i] = (((num/10000)%10) + '0');
	i++;
	TxBuf[i] = (((num/1000)%10) + '0');
	i++;
	TxBuf[i] = (((num/100)%10) + '0');
	i++;
	TxBuf[i] = (((num/10)%10) + '0');
	i++;
	TxBuf[i] = ('0'+(num%10));
	i++;

	units = 5 - decimal;
	for (i = 0; i < units; i++)
	{
		UART_char_Tx(TxBuf[i]);
	}
	UART_char_Tx('.');
	for (i = units;i < 5;i++)
	{
		UART_char_Tx(TxBuf[i]);
	}
}
