#ifndef H_EEPROM
#define H_EEPROM

#include "I2C.h"

#define EEPROM_ADDR_WRITE (0xA0)
#define EEPROM_ADDR_READ  (0xA1)

void i2c1_eeprom_write(uint8_t dadr_write, uint16_t mem_addr, uint8_t data);

uint8_t i2c1_eeprom_read(uint8_t dadr_write, uint8_t dadr_read, uint16_t mem_addr);

#endif