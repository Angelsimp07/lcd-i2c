#include "usart.h"
#include "gpio.h"
#include "I2C.h"
#include "lcd.h"

 // Define the main menu to be displayed on teraterm
 char main_menu[] = "\r\n\t\tMain_Menu\r\n\
 \t1 - dfrobot display\r\n\
 \t2 - eeprom\r\n";


 // Define the Esc message
 char esc_msg[] = "\r\n\t\tPress Esc to return to the Main Menu\r\n";

int main(void)
{
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
   case '1' : dfrobot_display(DFR1_1); break;

   case '2' : i2c1_eeprom_write_read(); break;

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