/* 
 * File:   flash.h
 * Author: Thomas
 *
 * Created on 08 February 2021, 1:12 PM
 */

#ifndef FLASH_H
#define	FLASH_H

extern int calibrations[max_cal_table];
extern int Data_table[max_Data_table];

void Write_cal_flash(void);
void Read_cal_flash(void);

#endif	/* FLASH_H */

