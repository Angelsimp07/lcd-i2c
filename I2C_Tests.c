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
      
        usartx_send_text(ptr_usart1, user_prompt);
      
        //Configure PB10 in alternate funciton mode
        gpiox_config(ptr_gpiob, PBEN_MASK, 10, 2, 1, 3, 4, 1);
      
        //Configure PB11 in alternate funciton mode
        gpiox_config(ptr_gpiob, PBEN_MASK, 11, 2, 1, 3, 4, 1);
      
        //Configure and initialize I2C1
        i2cx_config(ptr_i2c1, I2C1EN_MASK, 0, 1, 1);
      
        while(1)
        {
          // Black Background - Weather Theme
          tft_set_background_color(0, 0, 0);
          tft_draw_rect(1, 10, 10, 300, 220, 255, 255, 255); 
          tft_draw_text(2, 50, 20, 255, 255, 255, "WEATHER ICON"); 
          tft_draw_icon(3, 50, 100, 256, "/weather icon/sunny.png"); 
          tft_draw_gif(4, 200, 100, 256, "/weather icon/rainbow2.gif"); 
          
          for(int i=0; i<100; i++) {
            if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B)) return 0x1B;
            delay_systick_us(10000); 
          }

          // Blue Background - Sensor Theme
          tft_set_background_color(0, 0, 255);
          tft_draw_rect(1, 10, 10, 300, 220, 255, 255, 255);
          tft_draw_text(2, 50, 20, 255, 255, 255, "SENSOR ICON");
          tft_draw_icon(3, 50, 100, 256, "/sensor icon/thermometer.png");
          tft_draw_gif(4, 200, 100, 256, "/sensor icon/pressure.gif"); 
          
          for(int i=0; i<100; i++) {
            if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B)) return 0x1B;
            delay_systick_us(10000);
          }

          // Red Background - Botany Theme
          tft_set_background_color(255, 0, 0);
          tft_draw_rect(1, 10, 10, 300, 220, 255, 255, 255);
          tft_draw_text(2, 50, 20, 255, 255, 255, "BOTANY ICON");
          tft_draw_icon(3, 50, 100, 256, "/botany icon/cactus1.png");
          tft_draw_gif(4, 200, 100, 256, "/botany icon/Potted plant flower.gif");

          for(int i=0; i<100; i++) {
            if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B)) return 0x1B;
            delay_systick_us(10000);
          }
        }
      }      