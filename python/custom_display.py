#!/usr/bin/python
from enum import Enum
import os
import sys
from ctypes import c_uint16, cdll  
import time        
class FontType(Enum):
    Font_7x10 = 0
    Font_8x16 = 1
    Font_11x18 = 2
    Font_16x26  = 3         

UCTRONICS = cdll.LoadLibrary('./librm0004_display.so')  #The directory where the dynamic link library is located

def getCPUtemperature():
    res = os.popen('vcgencmd measure_temp').readline()
    return(res.replace("temp=","").replace("'C\n",""))

def getRAMinfo():
    p = os.popen('free')
    i = 0
    while 1:
        i = i + 1
        line = p.readline()
        if i==2:
            return(line.split()[1:4])


def getDiskSpace():
    p = os.popen("df -h /")
    i = 0
    while 1:
        i = i +1
        line = p.readline()
        if i==2:
            return(line.split()[1:5])

def getCPUuse():
    return(str(os.popen("top -n1 | awk '/Cpu\(s\):/ {print $2}'").readline().strip()))
 
if __name__ == '__main__':
    if UCTRONICS.lcd_begin() > 0:
        sys.exit(0)
    UCTRONICS.lcd_fill_screen(0x0000)


    while True:
        
        UCTRONICS.lcd_fill_rectangle(0,10,160,20,80)
        UCTRONICS.lcd_write_str(5,10,bytes("TEMP:","utf8"),FontType.Font_11x18.value,0xFFFF,80)
        UCTRONICS.lcd_write_str(65,10,bytes(getCPUtemperature()+"C","utf8"),FontType.Font_11x18.value,0xFFFF,80)

        UCTRONICS.lcd_fill_rectangle(0,45,160,20,80)
        UCTRONICS.lcd_write_str(5,45,bytes("USE:","utf8"),FontType.Font_11x18.value,0xFFFF,80)
        UCTRONICS.lcd_write_str(65,45,bytes(getCPUuse()+"%","utf8"),FontType.Font_11x18.value,0xFFFF,80)
        time.sleep(1)

            
