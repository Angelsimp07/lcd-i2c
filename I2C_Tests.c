#include "gpio.h"
#include "usart.h"
#include "lcd.h"
#include "led_driver.h"
#include "eeprom.h"
#include "systick.h"
#include "dfrobot_tft.h"

int tft_test(void)
{
  char image_prompt[] = "\r\n\tDisplaying IMG_4346.png as an icon.\r\n";

  usartx_send_text(ptr_usart1, image_prompt);

  // Draw the stored image using the icon API and the module's zoom parameter.
  tft_draw_icon(3, 0, 0, 256, "IMG_4346.png");
  delay_systick_us(200000);

  return 0;
}

int tft_background_color_test(void)
{
  char color_prompt[] = "\r\n\tCycling TFT background colors every 1 second.\r\n";

  usartx_send_text(ptr_usart1, color_prompt);

  while(1)
  {
    tft_set_background_color(255, 0, 0);
    delay_systick_us(2000000);

    tft_set_background_color(0, 255, 0);
    delay_systick_us(2000000);

    tft_set_background_color(0, 0, 255);
    delay_systick_us(2000000);

    tft_set_background_color(255, 255, 255);
    delay_systick_us(2000000);

    if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B))
    {
      break;
    }
  }

  return 0;
}
