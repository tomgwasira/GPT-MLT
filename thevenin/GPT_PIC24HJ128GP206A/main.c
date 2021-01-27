//Created by: Stanley Adams 2016

//Revision notes
//V1.00 - First prototype testing version


//End of Revision Notes

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Mode (Primary Oscillator (XT, HS, EC) w/ PLL)
#pragma config IESO = OFF               // Two-speed Oscillator Start-Up Enable (Start up with user-selected oscillator)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#include "p24hj128gp206a.h"
#include "UART.h"
#include "timers.h"
#include "adc.h"
#include "debug.h"
#include "tables.h"
#include "DEE Emulation 16-bit.h"
#include "flash.h"
#include "defines.h"

#define SSPSTAT_BIT_MASK  0b00111111            // Mask for I2C status bits
// bit 0 TBF: Transmit Buffer Full Status bit
// bit 1 RBF: Receive Buffer Full Status bit
// bit 2 R/W: Read/Write bit Information
// bit 3 S: START bit
// bit 4 P: STOP bit
// bit 5 D/A: Data/Address bit

// State 1 - SSPSTAT bits: S = 1, D_A = 0, R_W = 0, RBF = 1
#define state_1 0b00001010		//Master write. Receive buffer full, Last byte was address
// State 2 - SSPSTAT bits: S = 1, D_A = 1, R_W = 0, RBF = 1
#define state_2 0b00101010		//Master Write, Receive buffer full, Last byte was data
// State 3 - SSPSTAT bits: S = 1, D_A = 0, R_W = 1, RBF = 0
#define state_3 0b00001100		//Master Read, Last byte was address
// State 4 - SSPSTAT bits: S = 1, D_A = 1, R_W = 1, RBF = 0
#define state_4 0b00101100		//Master Read, Last byte was data
// State 5 - SSPSTAT bits: S = 1, D_A = 1, R_W = 0, RBF = 0
#define state_5 0b00101000		//Master Write, Receive buffer empty, Last byte was data
// State 6 - SSPSTAT bits: S = 1, D_A = 0, R_W = 1, RBF = 1
#define state_6 0b00001110		//Master Read, Receive buffer full, Last byte was address
// State 7 - SSPSTAT bits: S = 1, D_A = 1, R_W = 1, RBF = 1
#define state_7 0b00101110		//Master Read, Receive buffer full, Last byte was data
// State 8 - SSPSTAT bits: S = 1, D_A = 1, R_W = 1, RBF = 0
#define state_8 0b00110100		//Master Read, Receive buffer empty, Last byte was data
// State 9 - SSPSTAT bits: S = 1, D_A = 1, R_W = 1, RBF = 1
#define state_9 0b00110110
// State 10 - SSPSTAT bits: S = 1, D_A = 1, R_W = 0, RBF = 1
#define state_10 0b00110010
// State 11 - SSPSTAT bits: S = 1, D_A = 1, R_W = 0, RBF = 0
#define state_11 0b00110000
// State 12 - SSPSTAT bits: S = 1, D_A = 1, R_W = 0, RBF = 0
#define state_12 0b00001000		//Master write. Receive buffer not full, Last byte was address
//-----------------------------------------------------------------
//Variable definitions
//-----------------------------------------------------------------

extern const CalType Cal_names[max_cal_table];

const u16 Button_delay = 50;		 //Button debounce in ms

u8 old_state;
char  sz_1[BUFSIZE+1];
u16 u16_index;
u8 ux_index;
u8 i2c_state, i2c_status;
u8 i2cdebug, i2cdebug2, i2cdebug3, i2cdebug4, i2cdebug5;
u8 i2cError;
unsigned char ux_c;

unsigned int i2c_timeout;

const char * text = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

s16 calibrations[max_cal_table];
s16 Data_table[max_Data_table];

s16	Va_AVG;
s16	Vb_AVG;
s16	Vc_AVG;
s16	Ia_AVG;
s16	Ib_AVG;
s16	Ic_AVG;

s16 VaWave[256];
s16 VbWave[256];
s16 VcWave[256];
s16 IaWave[256];
s16 IbWave[256];
s16 IcWave[256];

u8 waveCounter;
u16 waveTxCounter;

s32 Pwr_a_sum;
s32 Pwr_b_sum;
s32 Pwr_c_sum;

s32 Va_disp_sum;
s32 Vb_disp_sum;
s32 Vc_disp_sum;
s32 Ia_disp_sum;
s32 Ib_disp_sum;
s32 Ic_disp_sum;
s32 Pa_disp_sum;
s32 Pb_disp_sum;
s32 Pc_disp_sum;

u32 Va_RMS_sum;
u32 Vb_RMS_sum;
u32 Vc_RMS_sum;
u32 Ia_RMS_sum;
u32 Ib_RMS_sum;
u32 Ic_RMS_sum;

u32 Va_AVG_temp;
u32 Vb_AVG_temp;
u32 Vc_AVG_temp;
u32 Ia_AVG_temp;
u32 Ib_AVG_temp;
u32 Ic_AVG_temp;
u32 Vbat_AVG_temp;
u32 Vext_AVG_temp;

u32 rcvTemp;

s16 Va_temp;
s16 Vb_temp;
s16 Vc_temp;
s16 Ia_temp;
s16 Ib_temp;
s16 Ic_temp;
s16 Vbat_temp;
s16 Vext_temp;

s16 Va_old;
s16 Vb_old;
s16 Vc_old;

s16 Va_disp;
s16 Vb_disp;
s16 Vc_disp;
s16 Ia_disp;
s16 Ib_disp;
s16 Ic_disp;
s16 Pa_disp;
s16 Pb_disp;
s16 Pc_disp;

u16 Timer_1ms;
u16 RMS_a_Count;
u16 RMS_b_Count;
u16 RMS_c_Count;
u16 Debug_val;
u16 Pout_abs;
u16 Flash_count;
u16 temp_del;
u16 LED_timer;
u16 PR3_calc;
u16 PR3_50Hz;
u16 PR3_53Hz;
u16 del_cnt;
u16 Relay_delay_Cnt;
u16 CT_Ratio;

u16 A_disp_cnt;
u16 B_disp_cnt;
u16 C_disp_cnt;

u16 DACVal;
u16 DACTemp;

u8 AVG_Count;
u8 Dbg_Start;
u8 Dbg_Len;
u8 Debug_Buf[32];
u8 Debug_cmd;
u8 cycle_cnt;
u8 LEDs;
u8 Zero_cross;
u8 Src_rec;
u8 Src_period;
u8 Seconds;
u8 Old_Seconds;
u8 Zero_cross;
u8 i2cDataType;
u8 tempCounter;
u8 rcvCount;
u8 updateCals;

//-----------------------------------------------------------------
//I/O Initialisation
//Sets the directions of all used I/O pins
//All outputs are also initialised to the correct startup state
//-----------------------------------------------------------------
void IO_Init(void)
{
	
	HB_LED_tris = OUTPUT;
	
	//Analog channels
	TRISBbits.TRISB0 = INPUT;
	TRISBbits.TRISB1 = INPUT;
	TRISBbits.TRISB2 = INPUT;
	TRISBbits.TRISB3 = INPUT;
	TRISBbits.TRISB4 = INPUT;
	TRISBbits.TRISB5 = INPUT;
	TRISBbits.TRISB6 = INPUT;
	TRISBbits.TRISB7 = INPUT;

	//DAC Outputs
	_TRISB8 = OUTPUT;
	_TRISB9 = OUTPUT;
	_TRISB10 = OUTPUT;
	_TRISB11 = OUTPUT;
	_TRISB12 = OUTPUT;
	_TRISB13 = OUTPUT;
	_TRISB14 = OUTPUT;
	_TRISB15 = OUTPUT;

	_LATB8 = 0;
	_LATB9 = 0;
	_LATB10 = 0;
	_LATB11 = 0;	
	_LATB12 = 0;
	_LATB13 = 0;
	_LATB14 = 0;
	_LATB15 = 0;	

	AD1PCFGH = 0xFFFF;
	AD1PCFGL = 0xFF00;													//Select which Analog inputs are in analog mode (0 = analog, 1 = digital)
}
//-----------------------------------------------------------------
//Main
//-----------------------------------------------------------------
int main(void) {
	
	RCONbits.SWDTEN = 0;		// Disable Watch Dog Timer	

    CLKDIV = 0;
    PLLFBD = 0x0026;
    
	IO_Init();			//Configure I/O pins
	
	//Initialise variables
	Timer_1ms = 0 ;
	Dbg_Start = 0;
	Dbg_Len = 0;
	Debug_cmd = 0;
	Zero_cross = 0;
	AVG_Count = 0;
	DACVal = 0;
	updateCals = 0;
	
	Va_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)
	Vb_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)
	Vc_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)
	Ia_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)
	Ib_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)
	Ic_AVG = 2048;		//For approximately a 1.65V offset. (To reduce initial error if value starts at 0)

	DataEEInit();		//Initialise Flash EEPROM emulation
	Read_cal_flash();	//Read calibration values from flash memory
	
	UART_Init();		//Initialise serial comms (115200 8-n-1)

	ux_index = 0;
	i2c_Init();
	
	Debug_sys_init();	//Display some debug info on startup
	
	ADC_Init();			//Setup ADC
	Timer4_Init();		//Setup Timer 4 operation
	Timer3_Init();		//Setup Timer 3 operation
	
	Timer_1ms = 1;
	
	//Endless loop
	while(1)
	{
		asm("clrwdt");
		if(Debug_cmd > 0)
		{
			Parse_debug_buffer();		//If there was a debug command, service it
			Debug_cmd--;
		}

		if(i2cError > 10)
		{
			i2c_Init();	//No I2C activity for > 10 seconds, re-init I2C
			i2cError = 0;
		}	

		if(Seconds != Old_Seconds)
		{
			HB_LED ^= 1;				//Toggle Heartbeat LED every second
			Old_Seconds = Seconds;
			
			CT_Ratio = CT_Size / 5;	//Since CT is xxx/5A
		}

		DACTemp = DACVal << 8;
		DACTemp &= 0xFF00;
		LATB = DACTemp;

		if(updateCals == 1)
		{
			Write_cal_flash();
			updateCals = 0;
		}

		if(i2cdebug != 0)
		{
			UART_Int_TX(i2cdebug, 3);
			UART_printf("  ");
			UART_Int_TX(i2cdebug2, 3);
			UART_printf("  ");
			UART_Int_TX(i2cdebug3, 3);
			UART_printf("  ");
			UART_Int_TX(i2cdebug4, 3);
			UART_printf("  ");
			UART_Int_TX(ux_c, 3);
			UART_printf("\r");
			i2cdebug = 0;
		}
	}  
	return 0;
} 

//-----------------------------------------------------------------
// All the Interrupt service routines follow
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Timer3 ISR:
// (128 interrupts per 50Hz cycle)
//-----------------------------------------------------------------
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void)
{
	IFS0bits.T3IF = 0;	//Clear TImer 3 Interrupt flag
	
	ADC_measure_all();	//Measure all ADC values once each interrupt (128 times per cycle)

}

//-----------------------------------------------------------------
// Timer4 ISR:
// Timer4 is set up to interrupt every 1mS
//-----------------------------------------------------------------
void __attribute__ ((interrupt,no_auto_psv)) _T4Interrupt(void)
{
	IFS1bits.T4IF = 0;	//Clear TImer 4 Interrupt flag
	Timer_1ms++;
	Timer_1ms %= 60000;

	i2c_timeout++;
	if(i2c_timeout > 1000)		//No I2C interrupts for 1 second, reset state machine
	{
		i2c_timeout = 0;
		i2c_state = 0;
		ux_index = 0;
		i2cError ++;
	}	

	if((Timer_1ms % 1000) == 0) Seconds++;	//Every Second
//	DACVal++;			//Used for testing the DAC
//	if(DACVal > 255) DACVal = 0;
		
}

//-----------------------------------------------------------------
// Uart 2 RX ISR:
// Uart 2 is the Debug port
//-----------------------------------------------------------------
void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
	unsigned char Debug_offset;
	IFS1bits.U2RXIF = 0;		//Clear Interrupt flag
	if(U2STAbits.OERR == 1)	U2STAbits.OERR = 0;	//Overflow error, ignore it for now
	Debug_offset = Dbg_Start + Dbg_Len;
	Debug_offset &= 0x1F;					//Same as %= 32
	Debug_Buf[Debug_offset] = U2RXREG;
	if(Debug_Buf[Debug_offset] == 13)		//<CR> was received
	{
		Debug_cmd ++;
		UART_printf("Got cmd.\r");
	}
	Dbg_Len++;
}

//-----------------------------------------------------------------
// I2C ISR:
//-----------------------------------------------------------------
void __attribute__ ((interrupt, no_auto_psv)) _SI2C1Interrupt(void)
{
	if(_I2COV)
	{
		ux_c = I2C1RCV; //clear RBF bit for address
		_I2COV = 0;
	}

	i2c_timeout = 0;	//Reset timeout
	i2cError = 0;
	// Mask the status bits out from the other unimportant register bits
    i2c_status = ( I2C1STAT & SSPSTAT_BIT_MASK);
	switch (i2c_status)
	{
		case state_1:			// State 1: Write operation, last byte was an address, buffer is full
		case state_12:
			ux_c = I2C1RCV;
			switch (i2cDataType)
			{
				case 8:		//Type 8 is for writing Calibrations, so we expect 3 more bytes - 1 for Index and 2 for the value
					rcvTemp <<= 8;
					rcvTemp += ux_c;	//Easy to fill the data in this way, but the byte order will be reversed now
					rcvCount++;
					if(rcvCount == 3)	//Got all three bytes
					{
						rcvCount = 0;
						i2cDataType = 0;
						ux_index = (rcvTemp >> 16) & 0xFF;	//First byte was index
						DACTemp = rcvTemp & 0xFF;
						DACTemp <<= 8;
						DACTemp += (rcvTemp >> 8) & 0xFF;	//Swap byte order
						calibrations[ux_index] = DACTemp;	//Update calibration table
						updateCals = 1;
					}
					break;
				case 12:	//Type 12 is for the DAC, so we expect another byte for the value
					DACVal = ux_c;
					i2cDataType = 0;
					break;
				default:
					ux_index = ux_c;	//Set the read index to the value received from the master
					if(ux_index < max_Data_table)
					{
						i2cDataType = 0;
						ux_index <<= 1;	//Set the read index to the value received from the master
					}
					else if(ux_index & 0x80)	//If MSB is set then we want other data
					{
						i2cDataType = (ux_index >> 1) & 0x3F;			//Drop the LSB and the MSB for the data type
						if((ux_index & 0x01) == 0) waveTxCounter = 0;	//LSB 0 for start and 1 for the rest of the packet
						else waveTxCounter++;
					}
					break;
			}
			break;

		case state_2:			// State 2: Write operation, last byte was data, buffer is full
		case state_5:			// State 5: Write operation, last byte was data, buffer is not full
		case state_10:
		case state_11:
			ux_c = I2C1RCV;
			switch (i2cDataType)
			{
				case 8:		//Type 8 is for writing Calibrations, so we expect 3 more bytes - 1 for Index and 2 for the value
					rcvTemp <<= 8;
					rcvTemp += ux_c;	//Easy to fill the data in this way, but the byte order will be reversed now
					rcvCount++;
					if(rcvCount == 3)	//Got all three bytes
					{
						rcvCount = 0;
						i2cDataType = 0;
						ux_index = (rcvTemp >> 16) & 0xFF;	//First byte was index
						DACTemp = rcvTemp & 0xFF;
						DACTemp <<= 8;
						DACTemp += (rcvTemp >> 8) & 0xFF;	//Swap byte order
						calibrations[ux_index] = DACTemp;	//Update calibration table
						updateCals = 1;
					}
					break;
				case 12:	//Type 12 is for the DAC, so we expect another byte for the value
					DACVal = ux_c;
					i2cDataType = 0;
					break;
				default:
					ux_index = ux_c;	//Set the read index to the value received from the master
					if(ux_index < max_Data_table)
					{
						i2cDataType = 0;
						ux_index <<= 1;	//Set the read index to the value received from the master
					}
					else if(ux_index & 0x80)	//If MSB is set then we want other data
					{
						i2cDataType = (ux_index >> 1) & 0x3F;			//Drop the LSB and the MSB for the data type
						if((ux_index & 0x01) == 0) waveTxCounter = 0;	//LSB 0 for start and 1 for the rest of the packet
						else waveTxCounter++;
					}
					break;
			}
			break;

		case state_3:			//  State 3: Read operation, last byte was an address, buffer is empty
		case state_6:
			//just keep placing characters in buffer as MASTER reads our I2C port
			ux_c = I2C1RCV;	//If the buffer is full, empty it

			while(_TBF) asm("NOP");	//Wait until transmit buffer is empty
			
			switch(i2cDataType)
			{
				case 0:	//Type 0 - Reading Data points
					if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					ux_index++;
					if(ux_index >= (max_Data_table * 2)) ux_index = 0;
					break;
				case 1:	//Type 1 - Read Va wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 2:	//Type 2 - Read Vb wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 3:	//Type 3 - Read Vc wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 4:	//Type 4 - Read Ia wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 5:	//Type 5 - Read Ib wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 6:	//Type 6 - Read Ic wavwform data
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 7:	//Type 7 - Read calibrations
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					waveTxCounter++;
					if(waveTxCounter >= (max_cal_table * 2)) waveTxCounter = 0;
					break;
				default:

					break;
			}
			I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
			break;

		case state_4:  //  State 4: Read operation, last byte was data, buffer is empty
		case state_7:
		case state_8:
		case state_9:
			//just keep placing characters in buffer as MASTER reads our I2C port
			ux_c = I2C1RCV;	//If the buffer is full, empty it

			while(_TBF) asm("NOP");	//Wait until transmit buffer is empty
			
			switch(i2cDataType)
			{
				case 0:
					if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((Data_table[(ux_index >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					ux_index++;
					if(ux_index >= (max_Data_table * 2)) ux_index = 0;
					break;
				case 1:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 2:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 3:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((VcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 4:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IaWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 5:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IbWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 6:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((IcWave[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter > 511) waveTxCounter = 0;
					break;
				case 7:
					if((waveTxCounter % 2) == 0) I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
					else I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					
					while(_IWCOL)
					{
						_IWCOL = 0;
						if((ux_index % 2) == 0) I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)]) & 0xFF);	//Send low byte first
						else I2C1TRN = (unsigned char)((calibrations[(waveTxCounter >> 1)] >> 8) & 0xFF);					//Send high byte second
					}
					//waveTxCounter++;
					if(waveTxCounter >= (max_cal_table * 2)) waveTxCounter = 0;
					break;
				default:

					break;
			}
			I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
			break;
		default:
			if(i2cdebug == 0)
			{
				i2cdebug = 6;
				i2cdebug2 = i2c_status;
			}
			i2c_state = 0x00;
	}

	_SI2C1IF = 0;
}
