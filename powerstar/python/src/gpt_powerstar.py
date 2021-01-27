# Routine for reading data from DSP on inverter board into Raspberry Pi.
# 
# This program uses the minicom interface to make requests for data to a serial port.
#
# Author: Thomas Gwasira (tomgwasira@gmail.com), MLT Power
# Date: 09/12/2020
# Version: 1.0

import matplotlib.pyplot as plt
import numpy as np
import serial

#---------------------------------------------------------------
# Reading data from Powerstar 10 card
#---------------------------------------------------------------

# Setup primary UART (miniUART) and read from Powerstar card
with serial.Serial("/dev/serial0", 115200, timeout=1) as ser:
    ser.reset_input_buffer() # clear input serial buffer
    ser.reset_output_buffer() # clear output serial buffer
    ser.write(b"-twh\r") # request headings for signals from Powerstar 10 card
    headings = str(ser.read(1024)) # read serial data in response to headings request. strip() necessary for any leading and trailing spaces in data
    headings = headings.split('r')
    print(headings)
    ser.close()