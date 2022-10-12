# SKU_RM0004 python wrapper
You can import the dynamic link library through **ctypes**  , and then call the function
```c
extern void lcd_write_string(uint16_t x, uint16_t y,  char *str, FontDef font,uint16_t color, uint16_t bgcolor);
extern void lcd_write_str(uint16_t x, uint16_t y,  char *str, FontType font,uint16_t color, uint16_t bgcolor);
extern void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color);
extern void lcd_fill_screen(uint16_t color);
extern void lcd_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
extern void lcd_set_address_window(uint8_t x0, uint8_t y0, uint8_t x1,uint8_t y1);
extern uint8_t lcd_begin(void);
extern void i2c_write_data(uint8_t high, uint8_t low);
extern void i2c_write_command(uint8_t command,uint8_t high, uint8_t low);
extern void lcd_write_char(uint16_t x, uint16_t y, char ch, FontDef font,uint16_t color, uint16_t bgcolor);
extern void lcd_write_ch(uint16_t x, uint16_t y, char ch, FontType font,uint16_t color, uint16_t bgcolor);
extern void i2c_burst_transfer(uint8_t* buff, uint32_t length);
extern void lcd_display(uint8_t symbol);
extern void lcd_display_cpuLoad(void);
extern void lcd_display_ram(void);
extern void lcd_display_temp(void);
extern void lcd_display_disk(void);
extern void lcd_display_percentage(uint8_t val, uint16_t color);
``` 
