cmake_minimum_required(VERSION 3.4)

PROJECT(LIBRM0004_DISPLAY)

include_directories(SYSTEM  hardware/rpiInfo hardware/st7735)

SET(LIBRM0004_DISPLAY_SRC hardware/rpiInfo/rpiInfo.c
    hardware/st7735/st7735.c
    hardware/st7735/fonts.c)

ADD_LIBRARY(rm0004_display SHARED ${LIBRM0004_DISPLAY_SRC})
