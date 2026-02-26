#include "gpio.h"
#include "rcu.h"

void delay(uint32_t num_count)
{
 while(num_count--);
}

// Define a function to configure an arbitrary GPIOx pin in an arbitrary mode
void gpiox_config(gpio_t* ptr_gpiox,
                  uint32_t gpiox_en_mask,
                  uint32_t gpiox_pin_number,
                  uint32_t gpiox_mode,
                  uint32_t gpiox_output_type,
                  uint32_t gpiox_ouput_speed,
                  uint32_t af_number,
                  uint32_t gpiox_pupd)
{
 // Enable GPIOx clock
 ptr_rcu->AHB1EN |= gpiox_en_mask; 

 // Clear the bits in GPIOx_CTL associated with gpiox_pin_number
 ptr_gpiox->CTL &= ~(3 << 2*gpiox_pin_number);

 // Configure gpiox_pin_number in the desired mode
 ptr_gpiox->CTL |= gpiox_mode << 2*gpiox_pin_number;

 // Clear the bit associated with gpiox pin in GPIOx_OMODE
 ptr_gpiox->OMODE &= ~(1 << gpiox_pin_number);

 // Configure gpiox pin in the desired output mode
 ptr_gpiox->OMODE |= gpiox_output_type << gpiox_pin_number;

 // Clear the bits associated with gpiox pin in GPIOx_OSPD
 ptr_gpiox->OSPD &= ~(3 << 2*gpiox_pin_number);

 // Configure gpiox pin in the desired output speed
 ptr_gpiox->OSPD |= gpiox_ouput_speed << 2*gpiox_pin_number;

 // Clear the bits associated with gpiox pin in GPIOx_PUD
 ptr_gpiox->PUD &= ~(3 << 2*gpiox_pin_number);

 // Configure gpiox pin in the desired pull up, pull down or floating
 ptr_gpiox->PUD |= gpiox_pupd << 2*gpiox_pin_number;

 if(gpiox_mode == 2)  // If the gpiox pin is configured in AF mode
 {
  if((gpiox_pin_number >= 0) && (gpiox_pin_number < 8))
  {
   // Clear 4 bits associated with gpiox pin in GPIOx_AFSEL0
   ptr_gpiox->AFSEL0 &= ~(15 << 4*gpiox_pin_number);

   // Configure the AF number in GPIOx_AFSEL0
   ptr_gpiox->AFSEL0 |= af_number << 4*gpiox_pin_number;
  }

  else if((gpiox_pin_number >= 8) && (gpiox_pin_number < 16))
  {
   // Clear 4 bits associated with gpiox pin in GPIOx_AFSEL1
   ptr_gpiox->AFSEL1 &= ~(15 << (4*gpiox_pin_number - 32));

   // Configure the AF number in GPIOx_AFSEL1
   ptr_gpiox->AFSEL1 |= af_number << (4*gpiox_pin_number - 32);
  }
 }
}

// Define a function for configuring an arbitrary GPIOx pin in external interrupt mode
void gpiox_config_exti(gpio_t* ptr_gpiox,
                       uint32_t gpiox_en_mask,
                       uint32_t gpiox_pin_number,
                       uint32_t gpiox_port_value,
                       uint32_t exti_inten_mask,
                       uint32_t exti_rten_mask,
                       uint32_t exti_ften_mask,
                       uint32_t exti_irqn,
                       uint32_t exti_int_priority)
{
 // Enable GPIOx clock
 ptr_rcu->AHB1EN |= gpiox_en_mask;

 // Enable SYSCFG clock
 ptr_rcu->APB2EN |= (1 << 14);

 // Configure gpiox pin in input mode by clearing the two bits associated with gpiox pin in GPIOx_CTL
 ptr_gpiox->CTL &= ~(3 << 2*gpiox_pin_number);

 // Configure gpiox pin in floating mode by clearing the two bits associated with gpiox pin in GPIOx_PUD
 ptr_gpiox->PUD &= ~(3 << 2*gpiox_pin_number);

 // Select the trigger source
 if((gpiox_pin_number >= 0) && (gpiox_pin_number < 4))
 {
  // Clear the four bits associated with gpiox pin in SYSCFG_EXTISS0 register
  ptr_syscfg->EXTI_SS0 &= ~(15 << 4*gpiox_pin_number);

  // Write the port value to the fours bits associated with gpiox pin in SYSCFG_EXTISS0
  ptr_syscfg->EXTI_SS0 |= gpiox_port_value << 4*gpiox_pin_number;
 }

 else if((gpiox_pin_number >= 4) && (gpiox_pin_number < 8))
 {
  // Clear the four bits associated with gpiox pin in SYSCFG_EXTISS1 register
  ptr_syscfg->EXTI_SS1 &= ~(15 << (4*gpiox_pin_number - 16));

  // Write the port value to the fours bits associated with gpiox pin in SYSCFG_EXTISS1
  ptr_syscfg->EXTI_SS1 |= gpiox_port_value << (4*gpiox_pin_number - 16);
 }

 else if((gpiox_pin_number >= 8) && (gpiox_pin_number < 12))
 {
  // Clear the four bits associated with gpiox pin in SYSCFG_EXTISS2 register
  ptr_syscfg->EXTI_SS2 &= ~(15 << (4*gpiox_pin_number - 32));

  // Write the port value to the fours bits associated with gpiox pin in SYSCFG_EXTISS2
  ptr_syscfg->EXTI_SS2 |= gpiox_port_value << (4*gpiox_pin_number - 32);
 }

 else if((gpiox_pin_number >= 12) && (gpiox_pin_number < 16))
 {
  // Clear the four bits associated with gpiox pin in SYSCFG_EXTISS3 register
  ptr_syscfg->EXTI_SS3 &= ~(15 << (4*gpiox_pin_number - 48));

  // Write the port value to the fours bits associated with gpiox pin in SYSCFG_EXTISS3
  ptr_syscfg->EXTI_SS3 |= gpiox_port_value << (4*gpiox_pin_number - 48);
 }

 // Enable EXTIx interrupt in EXTI_INTEN
 ptr_exti->INTEN |= exti_inten_mask;

 // Enable/disable rising trigger
 ptr_exti->RTEN |= exti_rten_mask;

 // Enable/disable falling trigger
 ptr_exti->FTEN |= exti_ften_mask;

 // Set the interrupt priority
 NVIC_SetPriority(exti_irqn, exti_int_priority);

 // Enable NVIC level interrupt
 NVIC_EnableIRQ(exti_irqn);
}

uint32_t g_exti0_count = 0;

void EXTI0_IRQHandler(void)
{
 // If Bit[0] in EXTI_PD is set
 if((ptr_exti->PD & (1 << 0)) == (1 << 0))
 {
  g_exti0_count++;

  // Clear EXTI_PD flag by setting Bit[0]
  ptr_exti->PD |= (1 << 0);
 }
}









