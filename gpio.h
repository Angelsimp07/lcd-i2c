#ifndef H_GPIO
#define H_GPIO

#include "gd32f4xx.h"

#define GPIOC_BASE_ADDR  (0x40020800)
#define GPIOA_BASE_ADDR  (0x40020000)
#define GPIOB_BASE_ADDR  (0x40020400)
#define GPIOF_BASE_ADDR  (0x40021400)

#define PCEN_MASK  (1 << 2)
#define PAEN_MASK  (1 << 0)
#define PBEN_MASK  (1 << 1)
#define PFEN_MASK  (1 << 5)

// Define a structure of GPIOx registers
typedef struct
{
 __IOM uint32_t CTL;
 __IOM uint32_t OMODE;
 __IOM uint32_t OSPD;
 __IOM uint32_t PUD;
 __IM uint32_t ISTAT;
 __IOM uint32_t OCTL;
 __OM uint32_t BOP;
 __IOM uint32_t LOCK;
 __IOM uint32_t AFSEL0;
 __IOM uint32_t AFSEL1;
 __OM uint32_t BC;
 __OM uint32_t TG;
} gpio_t;

// Define a pointer to the base address of GPIOC
#define ptr_gpioc ((gpio_t*)GPIOC_BASE_ADDR)

#define ptr_gpioa ((gpio_t*)GPIOA_BASE_ADDR)

#define ptr_gpiob ((gpio_t*)GPIOB_BASE_ADDR)

#define ptr_gpiof ((gpio_t*)GPIOF_BASE_ADDR)

// Define a structure of SYSCFG registers
typedef struct
{
 __IOM uint32_t CFG0;
 __IOM uint32_t CFG1;
 __IOM uint32_t EXTI_SS0;
 __IOM uint32_t EXTI_SS1;
 __IOM uint32_t EXTI_SS2;
 __IOM uint32_t EXTI_SS3;
 uint32_t rsvd[2];
 __IOM uint32_t CPSCTL;
} syscfg_t;

#define SYSCFG_BASE_ADDR   (0x40013800)

// Define a pointer to the base address of SYSCFG
#define ptr_syscfg  ((syscfg_t*)SYSCFG_BASE_ADDR)

// Define a structure of EXTI registers
typedef struct
{
 __IOM uint32_t INTEN;
 __IOM uint32_t EVEN;
 __IOM uint32_t RTEN;
 __IOM uint32_t FTEN;
 __IM uint32_t SWIEV;
 __IOM uint32_t PD;
} exti_t;

#define EXTI_BASE_ADDR   (0x40013C00)

// Define a pointer to the base address of EXTI
#define ptr_exti  ((exti_t*)EXTI_BASE_ADDR)



void delay(uint32_t num_count);

int blink_led(void);

int control_led_using_pb(void);

void gpiox_config(gpio_t* ptr_gpiox,
                  uint32_t gpiox_en_mask,
                  uint32_t gpiox_pin_number,
                  uint32_t gpiox_mode,
                  uint32_t gpiox_output_type,
                  uint32_t gpiox_ouput_speed,
                  uint32_t af_number,
                  uint32_t gpiox_pupd);
  
int control_led_using_kb(void);

extern uint32_t g_exti0_count;

void gpiox_config_exti(gpio_t* ptr_gpiox,
                       uint32_t gpiox_en_mask,
                       uint32_t gpiox_pin_number,
                       uint32_t gpiox_port_value,
                       uint32_t exti_inten_mask,
                       uint32_t exti_rten_mask,
                       uint32_t exti_ften_mask,
                       uint32_t exti_irqn,
                       uint32_t exti_int_priority);

int control_led_using_pb_exti(void);



#endif