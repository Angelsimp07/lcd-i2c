#include "I2C.h"
#include "lcd.h"
#include "systick.h"

void dfrobot_init(void)
{
  // 1. Wait at least 15ms after power-on
  delay_systick_us(20000);

  // 2. Initial Function Set 1 (0x38)
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, TWO_LINE_DISPLAY);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(5000);

  // 3. Initial Function Set 2 (0x38)
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, TWO_LINE_DISPLAY);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(100);

  // 4. Initial Function Set 3 (0x38)
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, TWO_LINE_DISPLAY);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(100);

  // 5. Display OFF
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, DISPLLAY_OFF);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(100);

  // 6. Clear Display
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, CLEAR_DISPLAY);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(2000);

  // 7. Entry Mode Set
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, MOVE_CURSOR_RIGHT);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(100);

  // 8. Display ON
  i2cx_start(ptr_i2c1);
  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);
  i2cx_send_byte(ptr_i2c1, CMD_WRITE);
  i2cx_send_byte(ptr_i2c1, DISPLAY_ON);
  i2cx_stop(ptr_i2c1);
  delay_systick_us(100);
}

void i2c1_send_text_to_dfr(char*str)
{
 delay_systick_us(800);

 i2cx_start(ptr_i2c1);

 // Send the device address with write 
 i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);

 // Next byte is a delay byte
 i2cx_send_byte(ptr_i2c1, DATA_WRITE);

 // Send the text string
 i2cx_send_text(ptr_i2c1, str);

 delay_systick_us(40);

 i2cx_stop(ptr_i2c1);
}

void dfrobot_clear_display(void)
{
  delay_systick_us(2000);

  i2cx_start(ptr_i2c1);

  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);

  i2cx_send_byte(ptr_i2c1, CMD_WRITE);

  i2cx_send_byte(ptr_i2c1, CLEAR_DISPLAY);

  delay_systick_us(1530);

  i2cx_stop(ptr_i2c1);
}

void dfrobot_set_cursor_position(uint32_t row, uint8_t col)
{
  delay_systick_us(800);

  i2cx_start(ptr_i2c1);

  i2cx_send_dadr(ptr_i2c1, DFR_ADDR_WRITE);

  i2cx_send_byte(ptr_i2c1, CMD_WRITE);

  if(row == 0)
  {
    i2cx_send_byte(ptr_i2c1, DDRAM_MASK|(0x00|col));
  }
  else if(row == 1)
  {
    i2cx_send_byte(ptr_i2c1, DDRAM_MASK|(0x40|col));
  }

  delay_systick_us(40);

  i2cx_stop(ptr_i2c1);
}