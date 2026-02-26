#include "systick.h"

void systick_config(uint32_t csr_mask, uint32_t reload_val, uint32_t int_priority)
{
 // Disable SysTick timer before configuring its registers
 ptr_systick->CSR = 0;

 // Clear current reload value
 ptr_systick->CVR = 0;

 // Set the required reload value 
 ptr_systick->RVR = reload_val - 1;

 // Set the interrupt priority
 NVIC_SetPriority(SysTick_IRQn, int_priority);

 // Enable SysTick timer
 ptr_systick->CSR |= csr_mask;
}

void delay_systick_us(uint32_t us_count)
{
 // Initialize SysTick timer // found in DUI0553A_cortex section 4.4.1
 systick_config(7, 42, 0);

 for(uint32_t i = 0; i < us_count; i++)
 {
  // Wait until the COUNTFLG (Bit[16]) in SYSTEM_CSR is set
  while(!(ptr_systick->CSR & (1 << 16)));
 }

 // Disable SysTick timer
 ptr_systick->CSR = 0;
}

uint32_t g_systick_count = 0;

void SysTick_Handler(void) // Already declared
{
 g_systick_count++;
}