#include "ens161.h"
#include "systick.h"

void ens161_write_reg(uint8_t reg, uint8_t val)
{
  i2cx_start(ptr_i2c2);
  i2cx_send_dadr(ptr_i2c2, ENS161_I2C_ADDR);
  i2cx_send_byte(ptr_i2c2, reg);
  i2cx_send_byte(ptr_i2c2, val);
  i2cx_stop(ptr_i2c2);
}

uint8_t ens161_read_reg(uint8_t reg)
{
  uint8_t val;
  i2cx_start(ptr_i2c2);
  i2cx_send_dadr(ptr_i2c2, ENS161_I2C_ADDR);
  i2cx_send_byte(ptr_i2c2, reg);
  i2cx_stop(ptr_i2c2);

  delay_systick_us(100);

  i2cx_start(ptr_i2c2);
  i2cx_send_dadr(ptr_i2c2, ENS161_I2C_ADDR | 1);
  val = i2cx_read_byte(ptr_i2c2);
  // i2cx_read_byte already sends the STOP condition internally!
  return val;
}

void ens161_read_multi(uint8_t reg, uint8_t* buffer, uint32_t len)
{
  i2cx_start(ptr_i2c2);
  i2cx_send_dadr(ptr_i2c2, ENS161_I2C_ADDR);
  i2cx_send_byte(ptr_i2c2, reg);
  i2cx_stop(ptr_i2c2);

  delay_systick_us(100);

  i2cx_start(ptr_i2c2);
  i2cx_send_dadr(ptr_i2c2, ENS161_I2C_ADDR | 1);
  i2cx_read_bytes(ptr_i2c2, buffer, len);
  // i2cx_read_bytes already sends the STOP condition on the last byte!
}

void ens161_init(void)
{
  // Set OPMODE to STANDARD
  ens161_write_reg(ENS161_REG_OPMODE, ENS161_OPMODE_STANDARD);
  delay_systick_us(1000);
}

uint8_t ens161_get_status(void)
{
  return ens161_read_reg(ENS161_REG_DEVICE_STATUS);
}

void ens161_read_data(ens161_data_t* data)
{
  uint8_t buffer[8];
  
  // Read AQI (0x21), TVOC (0x22, 0x23), ECO2 (0x24, 0x25), AQI_S (0x26, 0x27)
  ens161_read_multi(ENS161_REG_DATA_AQI, buffer, 7);
  
  data->aqi = buffer[0];
  data->tvoc = (uint16_t)buffer[1] | ((uint16_t)buffer[2] << 8);
  data->eco2 = (uint16_t)buffer[3] | ((uint16_t)buffer[4] << 8);
  data->aqi_s = (uint16_t)buffer[5] | ((uint16_t)buffer[6] << 8);
}
