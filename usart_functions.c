#include "usart.h"
#include "rcu.h"
#include <string.h>

void usartx_config(usart_t* ptr_usartx,
                   uint32_t usart_number,
                   uint32_t usartx_en_mask,
                   uint32_t usartdiv,
                   uint32_t ovs_mode,
                   uint32_t word_len,
                   uint32_t pcen,
                   uint32_t parity_mode,
                   uint32_t num_stop_bits,
                   uint32_t dma,
                   uint32_t data_sequence,
                   uint32_t interrupt_mask,
                   uint32_t usartx_irqn,
                   uint32_t interrupt_priority)
{
 // Enable USARTx clock
 if((usart_number == 1)||(usart_number == 2))
 {
  ptr_rcu->APB1EN |= usartx_en_mask;
 }

 else if((usart_number == 0)||(usart_number == 5))
 {
  ptr_rcu->APB2EN |= usartx_en_mask;
 }

 // Disable USARTx before configuring its parameters by clearing
 // Bit[13] in USART_CTL0
 ptr_usartx->CTL0 &= ~(1 << 13);

 // Configure the baud rate by writing USARTDIV to USART_BAUD
 ptr_usartx->BAUD = usartdiv;

 // Clear Bit[15] in USART_CTL0
 ptr_usartx->CTL0 &= ~(1 << 15);

 // Set the oversampling mode
 ptr_usartx->CTL0 |= ovs_mode << 15;

 // Clear Bit[12] in USART_CTL0
 ptr_usartx->CTL0 &= ~(1 << 12);

 // Set the word length
 ptr_usartx->CTL0 |= word_len << 12;

 // Clear Bit[10] in USART_CTL0
 ptr_usartx->CTL0 &= ~(1 << 10);

 // Enable/disable parity control
 ptr_usartx->CTL0 |= pcen << 10;

 // Clear Bit[9] in USART_CTL0
 ptr_usartx->CTL0 &= ~(1 << 9);

 // Set the parity mode
 ptr_usartx->CTL0 |= parity_mode << 9;

 // Clear Bits[13:12] in USART_CTL1
 ptr_usartx->CTL1 &= ~(3 << 12);

 // Set the number of stop bits
 ptr_usartx->CTL1 |=  num_stop_bits << 12;

 // Clear Bits[7:6] in USART_CTL2
 ptr_usartx->CTL2 &= ~(3 << 6);

 // Set the DMA mode
 ptr_usartx->CTL2 |= dma << 6;

 // Clear Bit[11] in USART_CTL3
 ptr_usartx->CTL3 &= ~(1 << 11);

 // Set the data sequence
 ptr_usartx->CTL3 |= data_sequence << 11;

 // Write the interrupt mask to USART_STAT0
 ptr_usartx->CTL0 |= interrupt_mask;

 // Enable USARTx, USARTx TX and USARTx RX
 ptr_usartx->CTL0 |= (3 << 2)|(1 << 13);

 // Set interrupt priority
 NVIC_SetPriority(usartx_irqn, interrupt_priority);

 // Enable NVIC-level interrupt
 NVIC_EnableIRQ(usartx_irqn);
}


void usartx_send_text(usart_t* ptr_usartx, char *str)
{
 for(uint32_t i = 0; i < strlen(str); i++)
 {
  // Wait until the DATA buffer is empty
  while((ptr_usartx->STAT0 & (1 << 7)) != (1 << 7));

  // Write one byte at a time to USART_DATA
  ptr_usartx->DATA = str[i];
 }
}

// Initialize g_usart1_rx count
uint32_t g_usart1_rx_count = 0;

// Initialize g_usart1_rx_arr
char g_usart1_rx_arr[256] = {};

void USART1_IRQHandler(void)
{
 // If RBNE flag is set in USART_STAT0 register
 if((ptr_usart1->STAT0 & (1 << 5)) == (1 << 5))
 {
  // Increment the character count
  g_usart1_rx_count++;

  // Read the character from USART_DATA register and copy it to g_usart1_rx_arr
  g_usart1_rx_arr[g_usart1_rx_count - 1] = ptr_usart1->DATA;
  
  if(g_usart1_rx_count >= 256)
  {
   // If the array if full, then reset the array
   memset(g_usart1_rx_arr, 0, 256);

   // Reset the character count
   g_usart1_rx_count = 0;
  }
 }
}









