#include "dfrobot_tft.h"
#include "systick.h"

void tft_send_packet(uint8_t cmd, uint8_t* data, uint8_t len)
{
  uint8_t checksum = 0;
  uint8_t packet_len = len + 1;

  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, TFT_ADDR_WRITE);

  i2cx_send_byte(ptr_i2c1, CMD_HEADER_HIGH);
  i2cx_send_byte(ptr_i2c1, CMD_HEADER_LOW);
  
  i2cx_send_byte(ptr_i2c1, packet_len);
  checksum += packet_len;

  i2cx_send_byte(ptr_i2c1, cmd);
  checksum += cmd;

  for(uint8_t i = 0; i < len; i++)
  {
    i2cx_send_byte(ptr_i2c1, data[i]);
    checksum += data[i];
  }

  // Send the 8-bit checksum
  i2cx_send_byte(ptr_i2c1, checksum);

  i2cx_stop(ptr_i2c1);
  delay_systick_us(10000); // Increased delay to 10ms for processing
}

void tft_set_background_color(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t data[3] = {r, g, b};
  tft_send_packet(CMD_SET_BACKGROUND_COLOR, data, 3);
}

void tft_set_background_img(uint8_t location, const char* path)
{
  uint8_t len = strlen(path);
  uint8_t data[256];

  data[0] = location;
  memcpy(&data[1], path, len);
  data[1 + len] = '\0';

  tft_send_packet(CMD_SET_BACKGROUND_IMG, data, 2 + len);
}

void tft_draw_text(uint8_t id, uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, const char* str)
{
  uint8_t len = strlen(str);
  uint8_t data[256]; // Large enough for most strings
  
  data[0] = id;
  data[1] = 2;       // Font size (default 2)
  data[2] = r;       // R
  data[3] = g;       // G
  data[4] = b;       // B
  data[5] = (uint8_t)((x >> 8) & 0xFF);
  data[6] = (uint8_t)(x & 0xFF);
  data[7] = (uint8_t)((y >> 8) & 0xFF);
  data[8] = (uint8_t)(y & 0xFF);
  memcpy(&data[9], str, len);
  
  tft_send_packet(CMD_OF_DRAW_TEXT, data, 9 + len);
}

void tft_draw_line(uint8_t id, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t data[13];
  data[0] = id;
  data[1] = 2; // Line width
  data[2] = r;
  data[3] = g;
  data[4] = b;
  data[5] = (uint8_t)((x0 >> 8) & 0xFF);
  data[6] = (uint8_t)(x0 & 0xFF);
  data[7] = (uint8_t)((y0 >> 8) & 0xFF);
  data[8] = (uint8_t)(y0 & 0xFF);
  data[9] = (uint8_t)((x1 >> 8) & 0xFF);
  data[10] = (uint8_t)(x1 & 0xFF);
  data[11] = (uint8_t)((y1 >> 8) & 0xFF);
  data[12] = (uint8_t)(y1 & 0xFF);
  
  tft_send_packet(CMD_OF_DRAW_LINE, data, 13);
}

void tft_draw_rect(uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t data[18]; // Total 18 bytes of data for the packet
  
  data[0] = id;
  data[1] = 2;       // Border width (default 2)
  data[2] = r;       // Border R
  data[3] = g;       // Border G
  data[4] = b;       // Border B
  data[5] = 0;       // Fill flag (0 = no fill, 1 = fill)
  data[6] = 0;       // Fill R
  data[7] = 0;       // Fill G
  data[8] = 0;       // Fill B
  data[9] = 0;       // Rounded corner (0 = square)
  
  data[10] = (uint8_t)((x >> 8) & 0xFF);
  data[11] = (uint8_t)(x & 0xFF);
  data[12] = (uint8_t)((y >> 8) & 0xFF);
  data[13] = (uint8_t)(y & 0xFF);
  data[14] = (uint8_t)((w >> 8) & 0xFF);
  data[15] = (uint8_t)(w & 0xFF);
  data[16] = (uint8_t)((h >> 8) & 0xFF);
  data[17] = (uint8_t)(h & 0xFF);
  
  tft_send_packet(CMD_OF_DRAW_RECT, data, 18);
}

void tft_delete_obj(uint8_t id)
{
  uint8_t data[1] = {id};
  tft_send_packet(CMD_DELETE_OBJ, data, 1);
}

void tft_draw_icon(uint8_t id, uint16_t x, uint16_t y, uint16_t scale, const char* path)
{
  uint8_t len = strlen(path);
  uint8_t data[256]; 
  
  data[0] = id;
  data[1] = (uint8_t)((scale >> 8) & 0xFF);
  data[2] = (uint8_t)(scale & 0xFF);
  data[3] = (uint8_t)((x >> 8) & 0xFF);
  data[4] = (uint8_t)(x & 0xFF);
  data[5] = (uint8_t)((y >> 8) & 0xFF);
  data[6] = (uint8_t)(y & 0xFF);
  memcpy(&data[7], path, len);
  data[7 + len] = '\0'; // Null terminator

  // For DFR0997, string paths refer to files stored on the display's USB storage.
  tft_send_packet(CMD_OF_DRAW_ICON_EXTERNAL, data, 8 + len);
}

void tft_draw_gif(uint8_t id, uint16_t x, uint16_t y, uint16_t scale, const char* path)
{
  uint8_t len = strlen(path);
  uint8_t data[256];
  uint8_t cmd = CMD_OF_DRAW_GIF_INTERNAL;
  
  data[0] = id;
  data[1] = (uint8_t)((x >> 8) & 0xFF);
  data[2] = (uint8_t)(x & 0xFF);
  data[3] = (uint8_t)((y >> 8) & 0xFF);
  data[4] = (uint8_t)(y & 0xFF);
  data[5] = (uint8_t)((scale >> 8) & 0xFF);
  data[6] = (uint8_t)(scale & 0xFF);
  memcpy(&data[7], path, len);
  data[7 + len] = '\0'; // Null terminator

  if((len > 0) && ((path[0] == '/') || (path[0] == '\\')))
  {
    cmd = CMD_OF_DRAW_GIF_EXTERNAL;
  }
  
  tft_send_packet(cmd, data, 8 + len);
}
