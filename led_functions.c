#include "I2C.h"
#include "led_driver.h"
#include "systick.h"

void pca9633_init(void)
{
  //Wake up the LED driver chip

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send PCA9633 device software address with write
  i2cx_send_dadr(ptr_i2c1, PCA9633_SW_ADDR);

  i2cx_send_byte(ptr_i2c1, 0xA5);

  i2cx_send_byte(ptr_i2c1, 0x5A);

  i2cx_stop(ptr_i2c1);

  //Wake up the LED driver chip

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send PCA9633 device address with write
  i2cx_send_dadr(ptr_i2c1, PCA9633_ADDR);

  i2cx_send_byte(ptr_i2c1, MODE1);

  i2cx_send_byte(ptr_i2c1, NORMAL_MODE);

  i2cx_stop(ptr_i2c1);

  //Turn on LED backlight with full brightness
  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send PCA9633 device address with write
  i2cx_send_dadr(ptr_i2c1, PCA9633_ADDR);

  i2cx_send_byte(ptr_i2c1, LED_OUT);

  i2cx_send_byte(ptr_i2c1, LED0_ON);

  i2cx_stop(ptr_i2c1);
}

void is31f_init(void)
{
  //Step 1
  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of shutdown register
  i2cx_send_byte(ptr_i2c1, 0x00);


  //Enabble all channels and normal operation
  i2cx_send_byte(ptr_i2c1, 0x20);

  i2cx_stop(ptr_i2c1);

  //Step 2

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of breathing control register
  i2cx_send_byte(ptr_i2c1, 0x01);


  //Enabble ramping mode, hold on T2, disable breathing mark
  //and select output channel 1
  i2cx_send_byte(ptr_i2c1, 0x00);

  i2cx_stop(ptr_i2c1);

  //Step 3

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of LED mode register
  i2cx_send_byte(ptr_i2c1, 0x02);


  //Enabble PWM control mode
  i2cx_send_byte(ptr_i2c1, 0x00);

  i2cx_stop(ptr_i2c1);

  //Step 4

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of current setting register
  i2cx_send_byte(ptr_i2c1, 0x03);


  //set maximum current setting to 42mA
  i2cx_send_byte(ptr_i2c1, 0x00);

  i2cx_stop(ptr_i2c1);

  //Step 5

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of PWM output 1 register
  i2cx_send_byte(ptr_i2c1, 0x04);


  //send I_out1 = I_out_max
  i2cx_send_byte(ptr_i2c1, 0xFF);

  i2cx_stop(ptr_i2c1);

  //Step 6

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address PWN Update register
  i2cx_send_byte(ptr_i2c1, 0x07);


  //Update PWM register
  i2cx_send_byte(ptr_i2c1, 0x00);

  i2cx_stop(ptr_i2c1);

  //Step 7

  delay_systick_us(500);

  i2cx_start(ptr_i2c1);

  //Send IS31F device address
  i2cx_send_dadr(ptr_i2c1, IS31F_ADDR);

  //Send address of LED control register
  i2cx_send_byte(ptr_i2c1, 0x1D);


  //Turn on LED
  i2cx_send_byte(ptr_i2c1, 0x01);

  i2cx_stop(ptr_i2c1);
}