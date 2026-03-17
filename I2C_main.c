#include "usart.h"
#include "gpio.h"
#include "I2C.h"
#include "lcd.h"

// Define the main menu to be displayed on teraterm
char main_menu[] = "\r\n\t\tMain_Menu\r\n\
\t1 - run IMG_4346.png test\r\n\
\t2 - run background color test\r\n\
\t3 - run ENS161-BGT value stream test\r\n\
\t4 - run Weather Station Example (ENS161)\r\n";


 // Define the Esc message
 char esc_msg[] = "\r\n\t\tPress Esc to return to the Main Menu\r\n";

int main(void)
{
 // Configure PB10/PB11 for I2C1 (SCL/SDA)
 gpiox_config(ptr_gpiob, PBEN_MASK, 10, 2, 1, 3, 4, 1);
 gpiox_config(ptr_gpiob, PBEN_MASK, 11, 2, 1, 3, 4, 1);

 // Match the original TFT test timing: fast mode with the alternate duty setting.
 i2cx_config(ptr_i2c1, I2C1EN_MASK, 0, 1, SCL_SPEED_FAST);

 // Configure PA2 in alternate function mode
 gpiox_config(ptr_gpioa, PAEN_MASK, 2, 2, 0, 0, 7, 0);

  // Configure PA2 in alternate function mode
 gpiox_config(ptr_gpioa, PAEN_MASK, 3, 2, 0, 0, 7, 0);

 // Configur Usart1
 usartx_config(ptr_usart1, 1, USART1EN_MASK, ((273 << 4)|7), 0, 0, 0, 0, 0, 0, 0, (1 << 5), USART1_IRQn, 0);
 
 // Display the main menu on teraterm
 usartx_send_text(ptr_usart1, main_menu);

 // Display the esc message on teraterm
 usartx_send_text(ptr_usart1, esc_msg);

 // Define a variable that will store the keyboard character
 char kb_input = 0;

 while(1)
 {
  // If no keyboard character is pressed
  if(g_usart1_rx_count == 0) continue;

  // Copy the character from usart1_rx_arr to kb_input
  kb_input = g_usart1_rx_arr[g_usart1_rx_count - 1];

  switch(kb_input)
  {
   case '1' : tft_test(); break;
   case '2' : tft_background_color_test(); break;
   case '3' : ens161_config_test(); break;
   case '4' : weather_station_example(); break;

   //case '2': spi1_follower_receives(); break;

   default: g_usart1_rx_count = 0;
  }

  // Reset the character count
  g_usart1_rx_count = 0;

  //Display the main menu on teraterm
  usartx_send_text(ptr_usart1, main_menu);

  //Display the main menu on teraterm
  usartx_send_text(ptr_usart1, esc_msg);

  // Display text on teraterm
  //usart1_display_text();
 }
}
