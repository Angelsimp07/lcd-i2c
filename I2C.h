#ifndef H_I2C
#define H_I2C

#include "gd32f4xx.h"

typedef struct // found in user manual 20.4.11
{
 __IOM uint32_t CTL0;
 __IOM uint32_t CTL1;
 __IOM uint32_t SADDR0;
 __IOM uint32_t SADDR1;
 __IOM uint32_t DATA;
 __IOM uint32_t STAT0;
 __IOM uint32_t STAT1;
 __IOM uint32_t CKCFG;
 __IOM uint32_t RT;
 __IOM uint32_t FCTL;
 uint32_t rsvd[21]; // 0x28 to 0x7C are missing meaning 0x44[68 bits] are missing which is 17 address
 __IOM uint32_t SAMCS;
}i2c_t;

// Define the base address
#define I2C1_BASE_ADDR (0x40005800)

#define ptr_i2c1 ((i2c_t*)I2C1_BASE_ADDR) // making a pointer back to the struct and address

#define I2C1EN_MASK (1 << 22) // This ca be found in user manual 4.3.13

#define APB1_FREQ      (42000000)
#define SCL_SPEED_STD  (0)
#define SCL_SPEED_FAST (1)

void i2cx_config(i2c_t* ptr_i2cx,
                 uint32_t i2cx_en_mask,
                 uint32_t dma,
                 uint32_t duty,
                 uint32_t scl_speed);// Declared

void i2cx_start(i2c_t* ptr_i2cx);

void i2cx_restart(i2c_t* ptr_i2cx);

void i2cx_send_dadr(i2c_t* ptr_i2cx, uint8_t dadr);

void i2cx_send_byte(i2c_t* ptr_i2cx, uint8_t byte);

void i2cx_stop(i2c_t* ptr_i2cx);

void i2cx_send_text(i2c_t* ptr_i2cx, const char *str);

uint8_t i2cx_read_byte(i2c_t* ptr_i2cx);

void i2cx_read_bytes(i2c_t* ptr_i2cx, uint8_t* buffer, uint32_t len);

int dfrobot_display(uint32_t dfr_version);

int tft_test(void);

int tft_background_color_test(void);

int i2c1_eeprom_write_read(void);

int ens161_config_test(void);

int weather_station_example(void);

#endif
