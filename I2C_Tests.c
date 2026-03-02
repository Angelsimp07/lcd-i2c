#include "gpio.h"
#include "usart.h"
#include "lcd.h"
#include "led_driver.h"
#include "eeprom.h"
#include "systick.h"
#include <stdio.h>
#include "dfrobot_tft.h"

      int tft_test(void)
      {
        //User prompt
        char user_prompt[] = "\r\n\n\tConnect PB10->SCL, PB11->SDA, 3.3V->VCC, and GND->GND!\r\n";
        char image_prompt[] = "\r\n\tCopy IMG_4346.png to the TFT storage root, then press 1.\r\n";
      
        usartx_send_text(ptr_usart1, user_prompt);
        usartx_send_text(ptr_usart1, image_prompt);
      
        //Configure PB10 in alternate funciton mode
        gpiox_config(ptr_gpiob, PBEN_MASK, 10, 2, 1, 3, 4, 1);
      
        //Configure PB11 in alternate funciton mode
        gpiox_config(ptr_gpiob, PBEN_MASK, 11, 2, 1, 3, 4, 1);
      
        //Configure and initialize I2C1
        i2cx_config(ptr_i2c1, I2C1EN_MASK, 0, 1, 1);
      
        while(1)
        {
          // Draw a single external image from the TFT's accessible storage.
          tft_set_background_color(0, 0, 0);
          tft_draw_rect(1, 10, 10, 300, 220, 255, 255, 255); 
          tft_draw_text(2, 30, 20, 255, 255, 255, "IMG_4346.png");
          tft_draw_icon(3, 40, 60, 1, "/IMG_4346.png");

          for(int i=0; i<100; i++) {
            if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B)) return 0x1B;
            delay_systick_us(10000);
          }
        }
      }      
