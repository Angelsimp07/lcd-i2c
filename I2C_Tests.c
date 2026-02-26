#include "gpio.h"
#include "usart.h"
#include "lcd.h"
#include "led_driver.h"
#include "eeprom.h"
#include "systick.h"
#include <stdio.h>

int dfrobot_display(uint32_t dfr_version)
{
  //User prompt
  char user_prompt[] = "\r\n\n\tConnect PB10->SCL, PB11->SDA, 3.3V->VCC, and GND->GND!\r\n";

  usartx_send_text(ptr_usart1, user_prompt);

  //Configure PB10 in alternate funciton mode
  gpiox_config(ptr_gpiob, PBEN_MASK, 10, 2, 1, 3, 4, 0);

  //Configure PB11 in alternate funciton mode
  gpiox_config(ptr_gpiob, PBEN_MASK, 11, 2, 1, 3, 4, 0);

  //Configure and initialize I2C1
  i2cx_config(ptr_i2c1, I2C1EN_MASK, 0, 1, 1);

  if(dfr_version == DFR1_0)
  {
    //Initalize PCA9633 driver
    pca9633_init();
  }

  else if(dfr_version == DFR1_1)
  {
    //Initialize IS31F LED driver
    is31f_init();
  }

  //Initialize DFROBOT
  dfrobot_init();

  dfrobot_set_cursor_position(0, 0);

  i2c1_send_text_to_dfr("Bingus");

  dfrobot_set_cursor_position(1, 0);

  i2c1_send_text_to_dfr(":P");

  while(1)
  {
    if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == Esc))
    {
      dfrobot_clear_display();

      return Esc;
    }
  }
}

int i2c1_eeprom_write_read(void)
{
  // Define the array os data to be written to EEPROM
  uint8_t write_data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};

  uint8_t num_byte = 10;

  uint8_t read_data[num_byte];

  char display[50];

  //Configure PB10 in alternate funciton mode
  gpiox_config(ptr_gpiob, PBEN_MASK, 10, 2, 1, 3, 4, 0);

  //Configure PB11 in alternate funciton mode
  gpiox_config(ptr_gpiob, PBEN_MASK, 11, 2, 1, 3, 4, 0);

  //Configure and initialize I2C1
  i2cx_config(ptr_i2c1, I2C1EN_MASK, 0, 1, 1);

  char user_prompt[] = "\r\n\n\tConnect pins 1, 2, 3, 4, and 7 to ground\
                        \r\n\tConnect pin 8 to 3.3V\
                        \r\n\tConnect pin 5 to PB11 and pin 6 to PB10\r\n";

  usartx_send_text(ptr_usart1, user_prompt);

  while (1)
  {
    if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == Esc)) return Esc;

    usartx_send_text(ptr_usart1, "\r\nWriting to EEPROM:\r\n\r\n");

    for(uint16_t addr = 0; addr < num_byte; addr++)
    {
      i2c1_eeprom_write(EEPROM_ADDR_WRITE, addr, write_data[addr]);

      delay_systick_us(500000); //500ms of delay

      sprintf(display, "0x%02X\r\n", write_data[addr]);

      usartx_send_text(ptr_usart1, display);
    }

     usartx_send_text(ptr_usart1, "\r\nReading EEPROM:\r\n\r\n");

     for(uint16_t addr = 0; addr < num_byte; addr++)
    {
      read_data[addr] = i2c1_eeprom_read(EEPROM_ADDR_WRITE, EEPROM_ADDR_READ, addr);

      delay_systick_us(500000); //500ms of delay

      sprintf(display, "0x%02X\r\n", read_data[addr]);

      usartx_send_text(ptr_usart1, display);
    }
  }
}