/* 
 * File:   thevenin_instrument.h
 * Author: Thomas
 *
 * Created on 28 January 2021, 12:08 PM
 */

#ifndef THEVENIN_INSTRUMENT_H
#define	THEVENIN_INSTRUMENT_H

#define Version 100


#define HB_LED				LATFbits.LATF0
#define HB_LED_tris			TRISFbits.TRISF0

#define OUTPUT 0
#define INPUT 1

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define s16 int
#define s32 long

#endif	/* THEVENIN_INSTRUMENT_H */

