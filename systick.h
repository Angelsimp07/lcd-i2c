#ifndef H_SYSTICK
#define H_SYSTICK

#include "gd32f4xx.h"

typedef struct
{
 __IOM uint32_t CSR;
 __IOM uint32_t RVR;
 __IOM uint32_t CVR;
 __IOM uint32_t CALIB;
}systick_t;

#define SYSTICK_BASE_ADDR (0xE000E010)

#define ptr_systick ((systick_t*)SYSTICK_BASE_ADDR) // pointer

void systick_config(uint32_t csr_mask, uint32_t reload_val, uint32_t int_priority);

void delay_systick_us(uint32_t us_count);

extern uint32_t g_systick_count; // An external counter

int systick_clock_demo(void);

#endif