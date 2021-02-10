#include "p24hj128gp206a.h"
#include "math.h"
#include "tables.h"
#include "defines.h"
#include "adc.h"

unsigned char Calc_a;
unsigned char Calc_b;
unsigned char Calc_c;

unsigned int ADCBuffer[32] __attribute__((space(dma)));	//Assign an array of 32 int's in DMA memory space

//-----------------------------------------------------------------
//ADC Initialisation
//-----------------------------------------------------------------
void ADC_Init(void)
{

	AD1CON1bits.FORM = 0; // Data Output Format: Simple integer
	AD1CON1bits.SSRC = 7; // Sample Clock Source: Internal counter starts conversion
	AD1CON1bits.ASAM = 1; // Sampling begins immediately after conversion
	AD1CON1bits.AD12B = 1; // 12-bit ADC operation
	AD1CON1bits.SIMSAM = 0; // Samples individual channels sequentially
	AD1CON2bits.BUFM = 0;
	AD1CON2bits.CSCNA = 1; // Scan CH0+ Input Selections during Sample A bit
	AD1CON2bits.CHPS = 0; // Converts CH0
	AD1CON3bits.ADRC = 0; // ADC Clock is derived from Systems Clock
	AD1CON3bits.ADCS = 6; // ADC Conversion Clock prescaler
	
	//AD1CHS0: A/D Input Select Register
	AD1CHS0bits.CH0SA = 0; // MUXA +ve input selection (AIN0) for CH0
	AD1CHS0bits.CH0NA = 0; // MUXA -ve input selection (Vref-) for CH0
	//AD1CHS123: A/D Input Select Register
	AD1CHS123bits.CH123SA = 0; // MUXA +ve input selection (AIN0) for CH1
	AD1CHS123bits.CH123NA = 0; // MUXA -ve input selection (Vref-) for CH1
	//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
	AD1CSSH = 0x0000;
	AD1CSSLbits.CSS0 = 1;		//Scan ADC0	- Line 1 Volts
	AD1CSSLbits.CSS1 = 1;		//Scan ADC1 - Line 2 Volts
	AD1CSSLbits.CSS2 = 1;		//Scan ADC2 - Line 3 Volts
	AD1CSSLbits.CSS3 = 1;		//Scan ADC3 - Line 1 Current
	AD1CSSLbits.CSS4 = 1;		//Scan ADC4 - Line 2 Current
	AD1CSSLbits.CSS5 = 1;		//Scan ADC5 - Line 3 Current
	AD1CSSLbits.CSS6 = 1;		//Scan ADC6 - Backup battery voltage
	AD1CSSLbits.CSS7 = 1;		//Scan ADC7 - External 0-5V / 0-20mA measurement
	
	//Set up DMA Channel 5 for Register Indirect with Post-Increment Addressing:
	
	DMA5CONbits.AMODE = 0; // Configure DMA for Register indirect mode with post-increment
	DMA5CONbits.MODE = 0; // Configure DMA for Continuous mode, no ping-pong
	DMA5PAD = (volatile unsigned int)&ADC1BUF0; // Point DMA to ADC1BUF0
	DMA5CNT = 31; // 32 DMA request
	DMA5REQ = 13; // Select ADC1 as DMA Request source
	DMA5STA = __builtin_dmaoffset(ADCBuffer);
	IFS3bits.DMA5IF = 0; //Clear the DMA interrupt flag bit
	IEC3bits.DMA5IE = 0; //Clear the DMA interrupt enable bit
	DMA5CONbits.CHEN=1; // Enable DMA
	
	//Set up ADC1 for DMA operation:
	AD1CON1bits.ADDMABM = 0; // Don't Care: ADC address generation is ignored by DMA
	AD1CON2bits.SMPI = 7; // Increment DMA address after every 8 samples
	AD1CON4bits.DMABL = 3; // Don't Care
	IFS0bits.AD1IF = 0; // Clear the A/D interrupt flag bit
	IEC0bits.AD1IE = 0; // Do Not Enable A/D interrupt 
	AD1CON1bits.ADON = 1; // Turn on the A/D converter

}

//-----------------------------------------------------------------
//Do all the necessary measurements
//-----------------------------------------------------------------
void ADC_measure_all(void)
{
	long int RMS_Temp;
	long int Calc_temp;
	unsigned long RMS_Calc;
	
	AVG_Count++;
	
	//Measure Line 1 voltage
	Va_temp = ADCBuffer[0] + ADCBuffer[8] + ADCBuffer[16] + ADCBuffer[24];				//Get instantaneous Line 1 Voltage from ADC
	Va_temp >>= 2;
	Va_AVG_temp += Va_temp;

	//Measure Line 2 voltage
	Vb_temp = ADCBuffer[1] + ADCBuffer[9] + ADCBuffer[17] + ADCBuffer[25];				//Get instantaneous Line 2 Voltage from ADC
	Vb_temp >>= 2;
	Vb_AVG_temp += Vb_temp;
	
	//Measure Line 3 voltage
	Vc_temp = ADCBuffer[2] + ADCBuffer[10] + ADCBuffer[18] + ADCBuffer[26];				//Get instantaneous Line 3 Voltage from ADC
	Vc_temp >>= 2;
	Vc_AVG_temp += Vc_temp;
	
	//Measure Line 1 current	
	Ia_temp = ADCBuffer[3] + ADCBuffer[11] + ADCBuffer[19] + ADCBuffer[27];				//Get instantaneous Line 1 Current from ADC Buffer
	Ia_temp >>= 2;
	Ia_AVG_temp += Ia_temp;

	//Measure Line 2 current	
	Ib_temp = ADCBuffer[4] + ADCBuffer[12] + ADCBuffer[20] + ADCBuffer[28];				//Get instantaneous Line 2 Current from ADC Buffer
	Ib_temp >>= 2;
	Ib_AVG_temp += Ib_temp;
	
	//Measure Line 3 current	
	Ic_temp = ADCBuffer[5] + ADCBuffer[13] + ADCBuffer[21] + ADCBuffer[29];				//Get instantaneous Line 3 Current from ADC Buffer
	Ic_temp >>= 2;
	Ic_AVG_temp += Ic_temp;
	
	//Measure Backup battery voltage
	Vbat_temp = ADCBuffer[6] + ADCBuffer[14] + ADCBuffer[22] + ADCBuffer[30];				//Get instantaneous Line 2 Voltage from ADC
	Vbat_temp >>= 2;
	
	//Measure external input
	Vext_temp = ADCBuffer[7] + ADCBuffer[15] + ADCBuffer[23] + ADCBuffer[31];				//Get instantaneous Line 2 Voltage from ADC
	Vext_temp >>= 2;

	IFS0bits.AD1IF = 0;							// clear ADC interrupt flag
	
	Va_temp -= (int)(Va_AVG);					//Remove DC Offset now for RMS calculation
	Vb_temp -= (int)(Vb_AVG);
	Vc_temp -= (int)(Vc_AVG);
	
	Ia_temp -= (int)(Ia_AVG);
	Ib_temp -= (int)(Ib_AVG);
	Ic_temp -= (int)(Ic_AVG);
	
	Calc_temp = (long int)(Va_temp);			//Scale Instantaneous Voltage measurement
	Calc_temp *= (long int)(Va_cal);			//using calibration value
	Calc_temp /= 1024;							//Cant >> signed values
	Va_temp = Calc_temp;
	
	Calc_temp = (long int)(Vb_temp);			//Scale Instantaneous Voltage measurement
	Calc_temp *= (long int)(Vb_cal);			//using calibration value
	Calc_temp /= 1024;							//Cant >> signed values
	Vb_temp = Calc_temp;

	Calc_temp = (long int)(Vc_temp);			//Scale Instantaneous Voltage measurement
	Calc_temp *= (long int)(Vc_cal);			//using calibration value
	Calc_temp /= 1024;							//Cant >> signed values
	Vc_temp = Calc_temp;
	
	Calc_temp = (long int)(Ia_temp);			//Scale Instantaneous Current measurement
	Calc_temp *= (long int)(Ia_cal);			//using calibration value
	Calc_temp *= (long int)(CT_Ratio);			//and CT Ratio
	Calc_temp /= 10240;							//Divide by 10 times extra to remove 1 decimal
	Ia_temp = Calc_temp;
	
	Calc_temp = (long int)(Ib_temp);			//Scale Instantaneous Current measurement
	Calc_temp *= (long int)(Ib_cal);			//using calibration value
	Calc_temp *= (long int)(CT_Ratio);			//and CT Ratio
	Calc_temp /= 10240;							//Divide by 10 times extra to remove 1 decimal
	Ib_temp = Calc_temp;
	
	Calc_temp = (long int)(Ic_temp);			//Scale Instantaneous Current measurement
	Calc_temp *= (long int)(Ic_cal);			//using calibration value
	Calc_temp *= (long int)(CT_Ratio);			//and CT Ratio
	Calc_temp /= 10240;							//Divide by 10 times extra to remove 1 decimal
	Ic_temp = Calc_temp;
	
	Calc_temp = (long int)(Va_temp);			//Calculate Instantaneous power
	Calc_temp *= (long int)(Ia_temp);			//Iac is 10 times too big (1 decimal)
	Calc_temp /= 10;
	Pwr_a_sum += (long int)(Calc_temp);
	
	Calc_temp = (long int)(Vb_temp);			//Calculate Instantaneous power
	Calc_temp *= (long int)(Ib_temp);			//Iac is 10 times too big
	Calc_temp /= 10;
	Pwr_b_sum += (long int)(Calc_temp);
	
	Calc_temp = (long int)(Vc_temp);			//Calculate Instantaneous power
	Calc_temp *= (long int)(Ic_temp);			//Iac is 10 times too big
	Calc_temp /= 10;
	Pwr_c_sum += (long int)(Calc_temp);

	Calc_temp = (long int)(Vbat_temp);			//Scale Instantaneous Voltage measurement
	Calc_temp *= (long int)(Vbat_cal);			//using calibration value
	Calc_temp /= 1024;							//Cant >> signed values
	Vbat_AVG_temp += Calc_temp;	

	Calc_temp = (long int)(Vext_temp);			//Scale Instantaneous Voltage measurement
	Calc_temp *= (long int)(Vext_cal);			//using calibration value
	Calc_temp /= 1024;							//Cant >> signed values
	Vext_AVG_temp += Calc_temp;		

    
    ///// ~ Change sampling rate if more or less than 128 samples /////
	if(((Va_old < 0) && (Va_temp >= 0)) || (Zero_cross > 200))
	{
		//Using Va as the reference for the grid frequency
		//We always want 128 samples per cycle, so we update the timer 3 period register
		//so that we always have 128 interrupts per cycle
		if((Zero_cross > 127) && (PR3 < 7102)) PR3++;		//Grid frequency is lower ~ because of Zero_cross at 128 that means we are at sample 129 which is already too much, so we need to check if Zero_cross at 127 meaning we are at sample 128 which is enough. Greater than 127 means do something to sample rate. 
		if((Zero_cross < 127) && (PR3 > 4807)) PR3--;		//Grid frequency is higher
		
		Zero_cross = 0;	//Check for source voltage zero-crossing;
		Calc_a = 1; // ~ Used to mark when there is one complete cycle. This is required for power calculations later on.
	}
	else
	{
		Zero_cross++;
		RMS_a_Count++;
		Calc_a = 0;
	}
	
	if((Vb_old < 0) && (Vb_temp >= 0))
	{
		Calc_b = 1;
	}
	else
	{
		RMS_b_Count++;
		Calc_b = 0;
	}
	
	if((Vc_old < 0) && (Vc_temp >= 0))
	{
		Calc_c = 1;
	}
	else
	{
		RMS_c_Count++;
		Calc_c = 0;
	}
	
	Va_old = Va_temp;
	Vb_old = Vb_temp;
	Vc_old = Vc_temp;
	
	//Calculate the squares of the measurements for RMS calculations
	RMS_Temp = (long int)(Va_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Va_RMS_sum += RMS_Calc;
	
	RMS_Temp = (long int)(Vb_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Vb_RMS_sum += RMS_Calc;

	RMS_Temp = (long int)(Vc_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Vc_RMS_sum += RMS_Calc;

	RMS_Temp = (long int)(Ia_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Ia_RMS_sum += RMS_Calc;
	
	RMS_Temp = (long int)(Ib_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Ib_RMS_sum += RMS_Calc;
	
	RMS_Temp = (long int)(Ic_temp);
	RMS_Temp *= RMS_Temp;
	RMS_Calc = (unsigned long)(RMS_Temp);
	Ic_RMS_sum += RMS_Calc;
	
	if(AVG_Count == 128)
	{
		AVG_Count = 0;
		
		Va_AVG_temp >>= 7;				//Get Average of AC Voltage measurements
		Va_AVG = Va_AVG_temp;			//Used for removing offset
		Va_AVG_temp = 0;
		
		Vb_AVG_temp >>= 7;				//Get Average of AC Voltage measurements
		Vb_AVG = Vb_AVG_temp;			//Used for removing offset
		Vb_AVG_temp = 0;

		Vc_AVG_temp >>= 7;				//Get Average of AC Voltage measurements
		Vc_AVG = Vc_AVG_temp;			//Used for removing offset
		Vc_AVG_temp = 0;
				
		Ia_AVG_temp >>= 7;				//Get Average of AC Current measurements
		Ia_AVG = Ia_AVG_temp;			//Used for removing offset
		Ia_AVG_temp = 0;

		Ib_AVG_temp >>= 7;				//Get Average of AC Current measurements
		Ib_AVG = Ib_AVG_temp;			//Used for removing offset
		Ib_AVG_temp = 0;
		
		Ic_AVG_temp >>= 7;				//Get Average of AC Current measurements
		Ic_AVG = Ic_AVG_temp;			//Used for removing offset
		Ic_AVG_temp = 0;

		Vbat_AVG_temp >>= 7;
		Vbat = Vbat_AVG_temp;
		Vbat_AVG_temp = 0;

		Vext_AVG_temp >>= 7;		
		Vext = Vext_AVG_temp;
		Vext_AVG_temp = 0;
	}	

	VaWave[waveCounter] = Va_temp;  // ~ VaWave is an array of 256 int values
	VbWave[waveCounter] = Vb_temp;
	VcWave[waveCounter] = Vc_temp;
	IaWave[waveCounter] = Ia_temp;
	IbWave[waveCounter] = Ib_temp;
	IcWave[waveCounter] = Ic_temp;
	waveCounter++;	

	if(Calc_a == 1)						//One AC cycle
	{
		Calc_a = 0;
		
		Pwr_a_sum /= RMS_a_Count;
		Pwr_a_sum /= 100;				//Current is 10 x too big (We now have 10W resolution)
		Pa_disp_sum += Pwr_a_sum;
		Pwr_a_sum = 0;
		
		Va_RMS_sum /= RMS_a_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Va_RMS_sum);	//Take square root, to get true RMS value
		Va_disp_sum += RMS_Calc;
		Va_RMS_sum = 0;
		
		Ia_RMS_sum /= RMS_a_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Ia_RMS_sum);	//Take square root, to get true RMS value
		Ia_disp_sum += RMS_Calc;
 		Ia_RMS_sum = 0;
 		
 		A_disp_cnt++;
 		
 		if(A_disp_cnt > 31)
 		{
	 		Va_RMS = Va_disp_sum / 32;	//Get the average over 32 cycles
	 		Ia_RMS = Ia_disp_sum / 32;	//Get the average over 32 cycles
	 		Pa_W = Pa_disp_sum / 32;	//Get the average over 32 cycles
	 		A_disp_cnt = 0;
	 		Va_disp_sum = 0;
	 		Ia_disp_sum = 0;
	 		Pa_disp_sum = 0;
	 	}	
 		
		RMS_a_Count = 0;
		if((waveCounter > 230) || (waveCounter < 25)) waveCounter = 0;		//Re-trigger for start of new cycle if close to the end
	}
	
	if(Calc_b == 1)
	{
		Calc_b = 0;
		
		Pwr_b_sum /= RMS_b_Count;
		Pwr_b_sum /= 100;				//Current is 10 x too big (We now have 10W resolution)
		Pb_disp_sum +=Pwr_b_sum;
		Pwr_b_sum = 0;
		
		Vb_RMS_sum /= RMS_b_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Vb_RMS_sum);	//Take square root, to get true RMS value
		Vb_disp_sum += RMS_Calc;
		Vb_RMS_sum = 0;
		
		Ib_RMS_sum  /= RMS_b_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Ib_RMS_sum);	//Take square root, to get true RMS value
		Ib_disp_sum += RMS_Calc;
 		Ib_RMS_sum = 0;
 		
 		B_disp_cnt++;
 		
 		if(B_disp_cnt > 31)
 		{
	 		Vb_RMS = Vb_disp_sum / 32;	//Get the average over 32 cycles
	 		Ib_RMS = Ib_disp_sum / 32;	//Get the average over 32 cycles
	 		Pb_W = Pb_disp_sum / 32;	//Get the average over 32 cycles
	 		B_disp_cnt = 0;
	 		Vb_disp_sum = 0;
	 		Ib_disp_sum = 0;
	 		Pb_disp_sum = 0;
	 	}
 		
 		RMS_b_Count = 0;
	}
	
	if(Calc_c == 1)
	{	
		Calc_c = 0;
		
		Pwr_c_sum /= RMS_c_Count;
		Pwr_c_sum /= 100;				//Current is 10 x too big (We now have 10W resolution)
		Pc_disp_sum += Pwr_c_sum;
		Pwr_c_sum = 0;
		
		Vc_RMS_sum /= RMS_c_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Vc_RMS_sum);	//Take square root, to get true RMS value
		Vc_disp_sum += RMS_Calc;
		Vc_RMS_sum = 0;
		
		Ic_RMS_sum /= RMS_c_Count;		//Take the mean of the squares
		RMS_Calc = sqrt2(Ic_RMS_sum);	//Take square root, to get true RMS value
		Ic_disp_sum += RMS_Calc;
 		Ic_RMS_sum = 0;
 		
 		C_disp_cnt++;
 		
 		if(C_disp_cnt > 31)
 		{
	 		Vc_RMS = Vc_disp_sum / 32;	//Get the average over 32 cycles
	 		Ic_RMS = Ic_disp_sum / 32;	//Get the average over 32 cycles
	 		Pc_W = Pc_disp_sum / 32;	//Get the average over 32 cycles
	 		C_disp_cnt = 0;
	 		Vc_disp_sum = 0;
	 		Ic_disp_sum = 0;
	 		Pc_disp_sum = 0;
	 	}
 		
 		RMS_c_Count = 0;
	}
}
