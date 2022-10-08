#!/usr/bin/python
import sys
# import ctypes
from ctypes import cdll
import time 
Cfun = cdll.LoadLibrary('./librm0004_display.so')  ##动态链接库所在目录


if __name__ == '__main__':
    if Cfun.lcd_begin() > 0:
        sys.exit(0)
    switch_flag = 0
    while True:
        Cfun.lcd_display(switch_flag)
        time.sleep(1)
        time.sleep(1)
        switch_flag += 1
        if switch_flag == 4 :
            switch_flag = 0
    