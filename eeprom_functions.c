#include "eeprom.h"
#include "I2C.h"
#include "systick.h"

void i2c1_eeprom_write(uint8_t dadr_write, uint16_t mem_addr, uint8_t data)
{
  // Send the START condition
  i2cx_start(ptr_i2c1);

  //Send EEPROM devie address with write command
  i2cx_send_dadr(ptr_i2c1, dadr_write);

  // Send the high memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)((mem_addr >> 8) & 0xFF));

  // Send the low memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)(mem_addr & 0xFF));

  //Send the data byte
  i2cx_send_byte(ptr_i2c1, data);

  // Send the Stop condition
  i2cx_stop(ptr_i2c1);

  // Wait for the EEPROM internal write cycle to complete (usually 5ms)
  delay_systick_us(5000);
}

uint8_t i2c1_eeprom_read(uint8_t dadr_write, uint8_t dadr_read, uint16_t mem_addr)
{
  // Send the START condition
  i2cx_start(ptr_i2c1);

  //Send EEPROM devie address with write command
  i2cx_send_dadr(ptr_i2c1, dadr_write);

  // Send the high memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)((mem_addr >> 8) & 0xFF));

  // Send the low memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)(mem_addr & 0xFF));

  // Send RESTART condition
  i2cx_restart(ptr_i2c1);

  // Send the device address with read command
  i2cx_send_dadr(ptr_i2c1, dadr_read);

  // Clear ACK Flag before reading the last byte
  ptr_i2c1->CTL0 &= ~(1 << 10);

  // Send the STOP condition
  ptr_i2c1->CTL0 |= (1 << 9);

  // Wait until the RBNE flag is set
  while (!(ptr_i2c1->STAT0 & (1 << 6)));

  // Read the data from EEPROM
  uint8_t data_read = (uint8_t)ptr_i2c1->DATA;

  return data_read;
}