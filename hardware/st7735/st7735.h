/* vim: set ai et ts=4 sw=4: */
#ifndef __ST7735_H__
#define __ST7735_H__

#include "fonts.h"
#include <stdbool.h>

#define I2C_ADDRESS       0x18
#define BURST_MAX_LENGTH  160

#define X_COORDINATE_MAX  160
#define X_COORDINATE_MIN  0
#define Y_COORDINATE_MAX  80
#define Y_COORDINATE_MIN  0

#define X_COORDINATE_REG   0X2A
#define Y_COORDINATE_REG   0X2B
#define CHAR_DATA_REG      0X2C
#define SCAN_DIRECTION_REG 0x36
#define WRITE_DATA_REG     0x00
#define BURST_WRITE_REG    0X01
#define SYNC_REG           0X03


#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04




// mini 160x80 display (it's unlikely you want the default orientation)
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 24
#define ST7735_YSTART 0
#define ST7735_WIDTH 80
#define ST7735_HEIGHT 160
//#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY |ST7735_MADCTL_BGR)
#define ST7735_ROTATION (ST7735_MADCTL_BGR)
*/
// mini 160x80, rotate left
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 0
#define ST7735_YSTART 24
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV |ST7735_MADCTL_BGR)
*/

// mini 160x80, rotate right

#define ST7735_IS_160X80 1
#define ST7735_XSTART 0
#define ST7735_YSTART 24
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)

/****************************/

#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define ST7735_BLACK 0x0000
#define ST7735_BLUE 0x001F
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_WHITE 0xFFFF
#define ST7735_GRAY 0x8410
#define ST7735_COLOR565(r, g, b)                                               \
  (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#ifdef __cplusplus
extern "C" {
#endif
// call before initializing any SPI devices

void lcd_write_string(uint16_t x, uint16_t y,  char *str, FontDef font,uint16_t color, uint16_t bgcolor);
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color);
void lcd_fill_screen(uint16_t color);
void lcd_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
void lcd_set_address_window(uint8_t x0, uint8_t y0, uint8_t x1,uint8_t y1);
uint8_t lcd_begin(void);
void i2c_write_data(uint8_t high, uint8_t low);
void i2c_write_command(uint8_t command,uint8_t high, uint8_t low);
void lcd_write_char(uint16_t x, uint16_t y, char ch, FontDef font,uint16_t color, uint16_t bgcolor);
void i2c_burst_transfer(uint8_t* buff, uint32_t length);
void lcd_display(uint8_t symbol);
void lcd_display_cpuLoad(void);
void lcd_display_ram(void);
void lcd_display_temp(void);
void lcd_display_disk(void);
void lcd_display_percentage(uint8_t val, uint16_t color);
#ifdef __cplusplus
}
#endif

#endif // __ST7735_H__
