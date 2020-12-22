# Routine for closing relays on powerstar board.
# 
# This program uses the minicom interface.
#
# Author: Thomas Gwasira (tomgwasira@gmail.com), MLT Power
# Date: 09/12/2020
# Version: 1.0

import matplotlib.pyplot as plt
import numpy as
import serial

# Setup primary UART (miniUART) and read from Powerstar card
with serial.Serial("/dev/serial0", 115200, timeout=1) as ser:
    ser.flushInput # clear input serial buffer
    
    ser.write(b"-p p 5555\r") # get access to testing mode
    ser.write(b"-p a 3\r") # get access to factory testing mode
    ser.write(b"-m1\r") # reset faults
    ser.write(b"-sd8\r") # check mode. Must be in standby mode.
    
    # ser.write(b"-f f ?\r") # help on factory mode
    ser.write(b"-f f test_io\r") # put device in IO test mode
    # ser.write(b"-f i ?\r") # help on IO factory mode
    ser.write(b"-f i src_1=1\r") # close src_1 relay
    ser.write(b"-f i src_2=1\r") # close src_2 relay
    
    # headings = [((x.strip()).strip(',')).split(',') for x in (((ser.read(1024)).strip()).strip('\r')).split('\r')] # read serial data in response to headings request. strip() necessary for any leading and trailing spaces in data
    # print(headings)

    # ser.write(b"-twrh\r"); ser.read(1024); # set to high resolution logging fadc = 10kHz
    # # ser.write(b"-twrl\r"); ser.read(1024); # set to low resolution logging fadc = 1kHz

    # ser.write(b"-tw\r") # request signals from Powerstar 10 card
    # data = [((x.strip()).strip(',')).split(',') for x in (((ser.read(10000)).strip()).strip('\r')).split('\r')] # read serial data
    # data = np.array(data[:-1]) # discard last list in data. Usually no complete.
    # data = data.astype(np.float) # convert data array of strings to array of floats 
    # data = np.transpose(data) # transpose data so that a list at each row is a buffer for a partricular measured quantity
    # # print(data)