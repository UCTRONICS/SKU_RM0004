#!/usr/bin/python
import sys
from ctypes import cdll
import time 
UCTRONICS = cdll.LoadLibrary('./librm0004_display.so')  #The directory where the dynamic link library is located


if __name__ == '__main__':
    if UCTRONICS.lcd_begin() > 0:
        sys.exit(0)
    switch_flag = 0
    while True:
        UCTRONICS.lcd_display(switch_flag)
        time.sleep(1)
        time.sleep(1)
        switch_flag += 1
        if switch_flag == 4 :
            switch_flag = 0
    