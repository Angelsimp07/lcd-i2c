#ifndef H_USART
#define H_USART

#include "gd32f4xx.h"

// Define a structure of USART registers
typedef struct
{
 __IOM uint32_t STAT0;
 __IOM uint32_t DATA;
 __IOM uint32_t BAUD;
 __IOM uint32_t CTL0;
 __IOM uint32_t CTL1;
 __IOM uint32_t CTL2;
 __IOM uint32_t GP;
 uint32_t resvd1[24];
 __IOM uint32_t CTL3;
 __IOM uint32_t RT;
 __IOM uint32_t STAT1;
 uint32_t rsvd2[12];
 __IOM uint32_t CHC;
} usart_t;

#define USART1_BASE_ADDR  (0x40004400)
#define ptr_usart1  ((usart_t*)USART1_BASE_ADDR)

#define USART1EN_MASK   (1 << 17)

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
                   uint32_t interrupt_priority);

void usartx_send_text(usart_t* ptr_usartx, char *str);

//void usart1_display_text(void);

// Define a variable to store the count of keyboard characters
extern uint32_t g_usart1_rx_count;

// Define a string array to store the keyboard characters
extern char g_usart1_rx_arr[256];

#define Esc  (27)



#endif







