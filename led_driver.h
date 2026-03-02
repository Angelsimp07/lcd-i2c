#ifndef H_LED_DRIVER
#define H_LED_DRIVER

#define PCA9633_ADDR  (0xC0) // LED driver address for DFROBOT V1.0

#define PCA9633_SW_ADDR (0x06)

#define IS31F_ADDR (0xD6) // LED driver address for DFROBOT V1.1

// Commands for PCA9633

#define MODE1 (0x00) // Keep the brightness full

#define LED_OUT (0x08)

#define NORMAL_MODE (0x00)

#define LED0_ON (0x55)

void pca9633_init(void);

void is31f_init(void);

#endif