#include "I2C.h"
#include "gpio.h"
#include "rcu.h"
#include <string.h>
#include "systick.h"

void i2cx_config(i2c_t* ptr_i2cx,
                 uint32_t i2cx_en_mask,
                 uint32_t dma,
                 uint32_t duty,
                 uint32_t scl_speed)
{
  uint32_t clk_ctl, rise_time, i2c_freq;

  //Enable I2Cx peripheral clock
  ptr_rcu->APB1EN |= i2cx_en_mask;

  //Reset I2Cx peripheral
  ptr_i2cx->CTL0 |= (1 << 15);

  //Set I2Cx peripheral
  ptr_i2cx->CTL0 &= ~(1 << 15);

  //Configure I2Cx in I2C mode
  ptr_i2cx->CTL0 &= ~(1 << 1);

  i2c_freq = (uint32_t)APB1_FREQ/1000000;

  //Clear I2CCLK (Bits[5:0] in I2C_CTL1 register
  ptr_i2cx->CTL1 &= ~(0x3F);

  //Write the required I2CCLK value to bits[5:0] in I2C_CTL1 register
  ptr_i2cx->CTL1 |= i2c_freq;

  //Clear DMAON (Bit[11]) in I2Cx_CTL1 register
  ptr_i2cx->CTL1 &= ~(1 << 11);

  //Enable/disable DMA
  ptr_i2cx->CTL1 |= (dma << 11);

  //Clear FAST (Bit[15]) in I2Cx_CKCFG register
  ptr_i2cx->CKCFG &= ~(1 << 15);

  //Select the required SCL speed
  ptr_i2cx->CKCFG |= (scl_speed << 15);

  //Clear DTCY (Biy[14]) in I2c_CKFG register
  ptr_i2cx->CKCFG &= ~(1 << 14);

  //Select thre required duty cycle
  ptr_i2cx->CKCFG |= (duty << 14);

  if(scl_speed == SCL_SPEED_STD)
  {
    clk_ctl = (uint32_t)APB1_FREQ/(1000000000*2);

    //Maximum rise tiem in standard speed is 1000 ns
    rise_time = (uint32_t)(APB1_FREQ/1000000000 + 1);
  }

  else if(scl_speed == SCL_SPEED_FAST)
  {
    //Maximum rise time in fast mode 300 ns
    rise_time = (uint32_t)((APB1_FREQ*3/10000000) + 1);

    if(duty == 0)
    {
      clk_ctl = (uint32_t)APB1_FREQ/(400000*3);
    }
    else clk_ctl = (uint32_t)APB1_FREQ/(400000*25);
  }

  //Clear CLKC (Bits[11:0]) in I2c_CLKCFG register
  ptr_i2cx->CKCFG &= ~(0xFFF);

  //Write the required clkc value in I2c_CKCFG register
  ptr_i2cx->CKCFG |= clk_ctl;

  //Clear RISETIME (Bits[5:0]) in I2C_RT register
  ptr_i2cx->RT &= ~(0x3F);

  //Write the required rise_time value in I2c_RT register
  ptr_i2cx->RT |= rise_time;

  //Enable I2Cx
  ptr_i2cx->CTL0 |= (1 << 0);

  //Enable I2c ACK
  ptr_i2cx->CTL0 |= (1 << 10);
}

void i2cx_start(i2c_t* ptr_i2cx)
{
  //Wait while the bus is busy
  while(ptr_i2cx->STAT1 & (1 << 1));  //While((ptr_i2cx->STAT1 & (1 << 1)) == (1 << 1));

  //Send the STAR condition to the bus
  ptr_i2cx->CTL0 |= (1 << 8);
}

void i2cx_restart(i2c_t* ptr_i2cx)
{
  //Wait until the TBE flag is set
  while(!(ptr_i2cx->STAT0 & (1 << 7)));  //While((ptr_i2cx->STAT1 & (1 << 7)) != (1 << 7));

  //Send the STAR condition to the bus
  ptr_i2cx->CTL0 |= (1 << 8);
}

void i2cx_send_dadr(i2c_t* ptr_i2cx, uint8_t dadr)
{
  //Wait until SBSEND flag is set in I2C_STAT0 register
  while(!(ptr_i2cx->STAT0 & (1 << 0)));
  
  //Clear SBSEND flag by reading I2C_STAT0 register and discarding the read value
  (void)ptr_i2cx->STAT0;

  //end the device address by writing the dadr value to I2C_DATA
  ptr_i2cx->DATA = dadr;

  //Wait until ADDSEND flag is set
  while(!(ptr_i2cx->STAT0 & (1 << 1)));

  //Clear ADDSEND flag by reading I2C_STAT0 and I2C_STAT1 registers and discardign them
  (void)ptr_i2cx->STAT0;

  (void)ptr_i2cx->STAT1;
}

void i2cx_send_byte(i2c_t* ptr_i2cx, uint8_t byte)
{
  //Wait until the TBE flag is set
  while(!(ptr_i2cx->STAT0 & (1 << 7)));  //While((ptr_i2cx->STAT1 & (1 << 7)) != (1 << 7));

  //Write the data byte to I2C_DATA
  ptr_i2cx->DATA = byte;
}

void i2cx_stop(i2c_t* ptr_i2cx)
{
  //Wait until BTC (Bit[2]) flags are set in I2C_STAT0
  while(!(ptr_i2cx->STAT0 &((1 << 2)|(1 << 7))));

  //Send the STOP condition to the bus
  ptr_i2cx->CTL0 |= (1 << 9);

  //Wait while the STOP condition is TRUE
  while(ptr_i2cx->CTL0 & (1 << 9));
}

void i2cx_send_text(i2c_t* ptr_i2cx, const char *str)
{
  for(uint32_t i = 0; i < strlen(str); i++)
  {
    delay_systick_us(25000);

    //Wait until the TBE flag is set
    while(!(ptr_i2cx->STAT0 & (1 << 7)));  //While((ptr_i2cx->STAT1 & (1 << 7)) != (1 << 7));

    ptr_i2cx->DATA = str[i];
  }
  //Wait until the TBE flag is set
  while(!(ptr_i2cx->STAT0 & (1 << 7)));  //While((ptr_i2cx->STAT1 & (1 << 7)) != (1 << 7));
}