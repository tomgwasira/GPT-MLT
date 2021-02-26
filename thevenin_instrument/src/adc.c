/*********************************************************************************************
*
* File:         adc.c 
* Written by:   Thomas Gwasira (tomgwasira@gmail.com), MLT Power
* Date:         February 2020
* Version:      1.00
*
* 
* Functionality:
* This code implements the use of the 12-bit ADC to sample voltage levels on different lines
* of the Thevenin instrument. The addToBuffers() method is called from an interrupt
* with a frequency determined by Timer 3 and adds sample values (averages 4 instantaneous
* values) on the input lines saved in the DMA buffer to their correct buffer. The frequency
* at which this occurs determines the so-called 'frequency rate' in the current project.
*
*********************************************************************************************/

#include "p24hj128gp206a.h"

#include "global.h"
#include "adc.h"



unsigned int ADCBuffer[32] __attribute__((space(dma)));	// array of 32 int's in DMA memory space. For voltage or current, stores 4 values before buffer is refreshed.

/* Global variables */
unsigned int buffer_index = 0; // index of each of the buffers to which a value is currently being written to

unsigned int sample_number_buffer[BUFFER_SIZE]; 
int V1_buffer[BUFFER_SIZE]; // stores data from DMA memory. DMA set to store 4 values for each line. (Original code takes the four values and averages them to reduce noise, but not doing that in this case).
int V2_buffer[BUFFER_SIZE];
int V3_buffer[BUFFER_SIZE];
int I1_buffer[BUFFER_SIZE];
int I2_buffer[BUFFER_SIZE];
int I3_buffer[BUFFER_SIZE];

int V1;
int V2;
int V3;
int I1;
int I2;
int I3;
int Vbackup_bat;
int Vextn;

/* Local variables */
unsigned int sample_count_since_i2c_start = 0; // number of samples added to buffers since I2C started transmitting. Used to pause adding to buffer if number of samples exceeded buffer size.
unsigned int sample_number = 0; // a count of the number of samples added to ADC buffers since program started running



/**
 * Initialises ADC1 , configures DMA for use with ADC and starts the ADC sampling.
 */
void ADC_Init(void) {
	AD1CON1bits.FORM = 0; // data output format: simple integer
	AD1CON1bits.SSRC = 7; // sample clock source: internal counter starts conversion
	AD1CON1bits.ASAM = 1; // Sampling begins immediately after conversion
	AD1CON1bits.AD12B = 1; // 12-bit ADC operation
	AD1CON1bits.SIMSAM = 0; // Samples individual channels sequentially
	AD1CON2bits.BUFM = 0;
	AD1CON2bits.CSCNA = 1; // scan CH0+ Input Selections during Sample A bit
	AD1CON2bits.CHPS = 0; // converts CH0
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
	AD1CSSLbits.CSS0 = 1; // scan ADC0	- Line 1 Volts
	AD1CSSLbits.CSS1 = 1; // scan ADC1 - Line 2 Volts
	AD1CSSLbits.CSS2 = 1; // scan ADC2 - Line 3 Volts
	AD1CSSLbits.CSS3 = 1; // scan ADC3 - Line 1 Current
	AD1CSSLbits.CSS4 = 1; // scan ADC4 - Line 2 Current
	AD1CSSLbits.CSS5 = 1; // scan ADC5 - Line 3 Current
	AD1CSSLbits.CSS6 = 1; // scan ADC6 - Backup battery voltage
	AD1CSSLbits.CSS7 = 1; // scan ADC7 - External 0-5V / 0-20mA measurement
	
	/* Set up DMA Channel 5 for Register Indirect with Post-Increment Addressing: */
	DMA5CONbits.AMODE = 0; // configure DMA for register indirect mode with post-increment
	DMA5CONbits.MODE = 0; // configure DMA for continuous mode, no ping-pong
	DMA5PAD = (volatile unsigned int)&ADC1BUF0; // point DMA to ADC1BUF0
	DMA5CNT = 31; // 32 DMA request
	DMA5REQ = 13; // select ADC1 as DMA Request source
	DMA5STA = __builtin_dmaoffset(ADCBuffer);
	IFS3bits.DMA5IF = 0; // clear the DMA interrupt flag bit
	IEC3bits.DMA5IE = 0; // clear the DMA interrupt enable bit
	DMA5CONbits.CHEN=1; // enable DMA
	
	/* Set up ADC1 for DMA operation: */
	AD1CON1bits.ADDMABM = 0; // don't Care: ADC address generation is ignored by DMA
	AD1CON2bits.SMPI = 7; // increment DMA address after every 8 samples
	AD1CON4bits.DMABL = 3; // don't Care
	IFS0bits.AD1IF = 0; // clear the A/D interrupt flag bit
	IEC0bits.AD1IE = 0; // do not enable A/D interrupt 
	AD1CON1bits.ADON = 1; // turn on the A/D converter
}

/**
 * Obtains instantaneous line voltages and currents from DMA buffers sampled by the ADC.
 * Note: Measured values have a DC offset due to a 1.65 V reference voltage being added to the ADC input such that negative voltage values are shifted up to
 * a level where they can be picked up by the ADc.
 * Offset will be corrected during calibration on the Raspberry Pi.
 */
void addToBuffers(void) {
	V1 = ADCBuffer[0] + ADCBuffer[8] + ADCBuffer[16] + ADCBuffer[24]; // get 4 consecutive Line 1 Voltages from DMA ADC buffer and sum them up
	V1 >>= 2; // average the summed up line 1 voltages

	V2 = ADCBuffer[1] + ADCBuffer[9] + ADCBuffer[17] + ADCBuffer[25]; // get 4 consecutive Line 2 Voltages from DMA ADC buffer and sum them up
	V2 >>= 2; // average the summed up line 2 voltages
	
	V3 = ADCBuffer[2] + ADCBuffer[10] + ADCBuffer[18] + ADCBuffer[26];	// get 4 consecutive Line 3 Voltages from DMA ADC buffer and sum them up
	V3 >>= 2; // average the summed up line 3 voltages
	
	I1 = ADCBuffer[3] + ADCBuffer[11] + ADCBuffer[19] + ADCBuffer[27];	// get 4 consecutive Line 1 currents from DMA ADC buffer and sum them up 
	I1 >>= 2; // average the summed up line 1 currents
	
	I2 = ADCBuffer[4] + ADCBuffer[12] + ADCBuffer[20] + ADCBuffer[28];	// get 4 consecutive Line 2 currents from DMA ADC buffer and sum them up 
	I2 >>= 2; // average the summed up line 2 currents
	
	I3 = ADCBuffer[5] + ADCBuffer[13] + ADCBuffer[21] + ADCBuffer[29];	// get 4 consecutive Line 3 currents from DMA ADC buffer and sum them up 
	I3 >>= 2; // average the summed up line 3 currents
	
	Vbackup_bat = ADCBuffer[6] + ADCBuffer[14] + ADCBuffer[22] + ADCBuffer[30]; // get 4 consecutive backup battery voltages from DMA ADC buffer and sum them up 
	Vbackup_bat >>= 2; // average the summed up backup battery voltages
	
	Vextn = ADCBuffer[7] + ADCBuffer[15] + ADCBuffer[23] + ADCBuffer[31]; // get 4 consecutive external voltages from DMA ADC buffer and sum them up 
	Vextn >>= 2; // average the summed up external voltages
    
	IFS0bits.AD1IF = 0;	// clear ADC interrupt flag
            
    if (i2c_transmitting_samples) { sample_count_since_i2c_start++; } // if I2C is transmitting, increment sample count so that if buffer has been filled up while I2C is transmitting, stop adding more elements. Placed above next if statement so that first sample added to buffer means sample_count_since_i2c_start = 1
    
    else { sample_count_since_i2c_start = 0; } // if I2C is not transmitting or done transmitting according to flag written by MASTER, reset sample count and buffer index at start of transmission
    
    if (sample_count_since_i2c_start <= BUFFER_SIZE) { // if number of samples added to buffer since I2C started transmission is less than size of buffer otherwise stop adding anymore new values to avoid overwriting values before they have been transmitted
        
        /* Note: Only NUMBER_BUFFERS_TO_TRANSMIT buffers will be transmitted over I2C. Alter this constant accordingly! */
        
        sample_number_buffer[buffer_index] = sample_number++;
        V1_buffer[buffer_index] = V1;      
        V2_buffer[buffer_index] = V2;   
        V3_buffer[buffer_index] = V3;
        I1_buffer[buffer_index] = I1;
        I2_buffer[buffer_index] = I2;
        I3_buffer[buffer_index] = I3;
        
        buffer_index++; // do not increment outside of if statement, because that will increment buffer index when nothing has been added to the buffers
        if (buffer_index >= BUFFER_SIZE) { buffer_index = 0; } // reset buffer index if greater than buffer size to avoid indexing out of range
    }
}
