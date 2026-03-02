#ifndef H_DFROBOT_TFT
#define H_DFROBOT_TFT

#include "gd32f4xx.h"
#include "I2C.h"
#include <string.h>

#define TFT_ADDR_WRITE      (0x58) // 0x2C << 1

// Command IDs
#define CMD_SET_BACKGROUND_COLOR      0x19
#define CMD_SET_BACKGROUND_IMG        0x1A
#define CMD_OF_DRAW_PIXEL             0x02
#define CMD_OF_DRAW_LINE              0x03
#define CMD_OF_DRAW_RECT              0x04
#define CMD_OF_DRAW_CIRCLE            0x06
#define CMD_OF_DRAW_TRIANGLE          0x07
#define CMD_OF_DRAW_ICON_INTERNAL     0x08
#define CMD_OF_DRAW_ICON_EXTERNAL     0x09
#define CMD_OF_DRAW_BAR               0x0A
#define CMD_OF_DRAW_BAR_VALUE         0x0B
#define CMD_OF_DRAW_SLIDER            0x0C
#define CMD_OF_DRAW_SLIDER_VALUE      0x0D
#define CMD_OF_DRAW_LINE_METER        0x10
#define CMD_OF_DRAW_LINE_METER_VALUE  0x11
#define CMD_OF_DRAW_COMPASS           0x0E
#define CMD_OF_DRAW_COMPASS_VALUE     0x0F
#define CMD_OF_DRAW_GAUGE             0x12
#define CMD_OF_DRAW_GAUGE_VALUE       0x13
#define CMD_OF_DRAW_LINE_CHART        0x14
#define CMD_OF_DRAW_LINE_CHART_TEXT   0x15
#define CMD_OF_DRAW_SERIE             0x16
#define CMD_OF_DRAW_SERIE_DATA        0x17
#define CMD_OF_DRAW_TEXT              0x18
#define CMD_DELETE_OBJ                0x1B
#define CMD_SET_TOP_OBJ               0x1C
#define CMD_SET_ANGLE_OBJ             0x1E
#define CMD_OF_DRAW_GIF_INTERNAL      0x1F
#define CMD_OF_DRAW_GIF_EXTERNAL      0x20

// Command Header IDs
#define CMD_HEADER_HIGH               0x55
#define CMD_HEADER_LOW                0xaa

void tft_send_packet(uint8_t cmd, uint8_t* data, uint8_t len);

void tft_set_background_color(uint8_t r, uint8_t g, uint8_t b);

void tft_draw_text(uint8_t id, uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, const char* str);

void tft_draw_line(uint8_t id, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t r, uint8_t g, uint8_t b);

void tft_draw_rect(uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b);

void tft_delete_obj(uint8_t id);

void tft_draw_icon(uint8_t id, uint16_t x, uint16_t y, uint16_t scale, const char* path);

void tft_draw_gif(uint8_t id, uint16_t x, uint16_t y, uint16_t scale, const char* path);

#endif
