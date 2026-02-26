#include "eeprom.h"
#include "I2C.h"

void i2c1_eeprom_write(uint8_t dadr_write, uint16_t mem_addr, uint8_t data)
{
  // Send the START condition
  i2cx_start(ptr_i2c1);

  //Send EEPROM devie address with write command
  i2cx_send_dadr(ptr_i2c1, EEPROM_ADDR_WRITE);

  // Send the high memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)((mem_addr & 0x7F00) >> 8)); // we want 1000000 so we & with 1111111 

  // Send the low memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)(mem_addr & 0xFF));

  //Send the data byte
  i2cx_send_byte(ptr_i2c1, data);

  // Send the Stop condition
  i2cx_stop(ptr_i2c1);
}

uint8_t i2c1_eeprom_read(uint8_t dadr_write, uint8_t dadr_read, uint16_t mem_addr)
{
  // Send the START condition
  i2cx_start(ptr_i2c1);

  //Send EEPROM devie address with write command
  i2cx_send_dadr(ptr_i2c1, EEPROM_ADDR_WRITE);

  // Send the high memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)((mem_addr & 0x7F00) >> 8)); // we want 1000000 so we & with 1111111 

  // Send the low memory address byte
  i2cx_send_byte(ptr_i2c1, (uint8_t)(mem_addr & 0xFF));

  // Send RESTART condition
  i2cx_restart(ptr_i2c1);

  // Send the device address with read command
  i2cx_send_dadr(ptr_i2c1, EEPROM_ADDR_READ);

  // Wait until the RBNE flag is set
  while (!(ptr_i2c1->STAT0 & (1 << 6)));

  // Read the data from EEPROM
  uint8_t data_read = ptr_i2c1->DATA;

  // Clear ACK Flag 
  ptr_i2c1->CTL0 &= ~(1 << 10);

  // Send the STOP condition
  ptr_i2c1->CTL0 |= (1 << 9); // We don't need to wait for any flags to be set

  return data_read;
}