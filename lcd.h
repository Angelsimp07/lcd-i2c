#ifndef H_LCD
#define H_LCD

#include "gd32f4xx.h"
#include "I2C.h"
#include <string.h>

#define DFR_ADDR_WRITE    (0x7C) // All masks from lcd_mannual 

#define DFR1_0            (0)
#define DFR1_1            (1)

#define CLEAR_DISPLAY     (0x01)
#define DISPLAY_ON        (0x0C)
#define DISPLLAY_OFF      (0x08)
#define CURSOR_ON         (0x0A)
#define CURSOR_OFF        (0x08)

#define MOVE_CURSOR_RIGHT (0x06)
#define MOVE_CURSOR_LEFT  (0x04)
#define TWO_LINE_DISPLAY  (0x38)
#define CGRAM_MASK        (0x40)
#define DDRAM_MASK        (0x80)
#define CMD_WRITE         (0x00)// Next bit will be a comment
#define DATA_WRITE        (0x40)// Next bit will be Data

void dfrobot_init(void);

void i2c1_send_text_to_dfr(char*str);

void dfrobot_clear_display(void);

void dfrobot_set_cursor_position(uint32_t row, uint8_t col);

#endif