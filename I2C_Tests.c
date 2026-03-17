#include "gpio.h"
#include "usart.h"
#include "lcd.h"
#include "led_driver.h"
#include "eeprom.h"
#include "systick.h"
#include "dfrobot_tft.h"
#include <stdio.h>

#define ENS161_REG_PART_ID_L  (0x00)
#define ENS161_REG_OPMODE     (0x10)
#define ENS161_REG_STATUS     (0x20)
#define ENS161_REG_DATA_AQI   (0x21)
#define ENS161_REG_DATA_TVOC  (0x22)
#define ENS161_REG_DATA_ECO2  (0x24)

#define ENS161_OPMODE_STD     (0x02)

#define ENS161_I2C_TIMEOUT_CYCLES (200000U)

#define TFT_ID_WIDGET_TITLE_B   (40)
#define TFT_ID_WIDGET_TITLE_L   (41)
#define TFT_ID_WIDGET_STATUS_B  (42)
#define TFT_ID_WIDGET_AQI_B     (43)
#define TFT_ID_WIDGET_TVOC_B    (44)
#define TFT_ID_WIDGET_ECO2_B    (45)
#define TFT_ID_WIDGET_STATUS_L  (46)
#define TFT_ID_WIDGET_AQI_L     (47)
#define TFT_ID_WIDGET_TVOC_L    (48)
#define TFT_ID_WIDGET_ECO2_L    (49)
#define TFT_ID_WIDGET_STATUS_V  (50)
#define TFT_ID_WIDGET_AQI_V     (51)
#define TFT_ID_WIDGET_TVOC_V    (52)
#define TFT_ID_WIDGET_ECO2_V    (53)
#define TFT_ID_WIDGET_COMPASS   (54)
#define TFT_ID_WIDGET_COMPASS_L (55)

static int ens161_wait_set(volatile uint32_t *reg, uint32_t mask, uint32_t timeout)
{
  while (((*reg & mask) != mask) && (timeout > 0U))
  {
    timeout--;
  }

  return (timeout == 0U) ? -1 : 0;
}

static int ens161_wait_clear(volatile uint32_t *reg, uint32_t mask, uint32_t timeout)
{
  while (((*reg & mask) != 0U) && (timeout > 0U))
  {
    timeout--;
  }

  return (timeout == 0U) ? -1 : 0;
}

static void ens161_force_stop(void)
{
  ptr_i2c1->CTL0 |= (1U << 9);
  (void)ens161_wait_clear(&ptr_i2c1->CTL0, (1U << 9), ENS161_I2C_TIMEOUT_CYCLES);
}

static int ens161_send_addr(uint8_t dadr)
{
  ptr_i2c1->DATA = dadr;

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 1), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    return -1;
  }

  (void)ptr_i2c1->STAT0;
  (void)ptr_i2c1->STAT1;
  return 0;
}

static int ens161_write_reg(uint8_t addr_7bit, uint8_t reg_addr, uint8_t value)
{
  if (ens161_wait_clear(&ptr_i2c1->STAT1, (1U << 1), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    return -1;
  }

  ptr_i2c1->CTL0 |= (1U << 8);
  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 0), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -2;
  }

  (void)ptr_i2c1->STAT0;

  if (ens161_send_addr((uint8_t)(addr_7bit << 1)) != 0)
  {
    ens161_force_stop();
    return -3;
  }

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 7), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -4;
  }
  ptr_i2c1->DATA = reg_addr;

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 7), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -5;
  }
  ptr_i2c1->DATA = value;

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 2), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -6;
  }

  ens161_force_stop();
  return 0;
}

static int ens161_read_reg(uint8_t addr_7bit, uint8_t reg_addr, uint8_t *value)
{
  if ((value == 0))
  {
    return -20;
  }

  if (ens161_wait_clear(&ptr_i2c1->STAT1, (1U << 1), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    return -1;
  }

  ptr_i2c1->CTL0 |= (1U << 8);
  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 0), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -2;
  }

  (void)ptr_i2c1->STAT0;

  if (ens161_send_addr((uint8_t)(addr_7bit << 1)) != 0)
  {
    ens161_force_stop();
    return -3;
  }

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 7), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -4;
  }
  ptr_i2c1->DATA = reg_addr;

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 7), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -5;
  }

  ptr_i2c1->CTL0 |= (1U << 8);
  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 0), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    return -6;
  }

  (void)ptr_i2c1->STAT0;

  if (ens161_send_addr((uint8_t)((addr_7bit << 1) | 0x01U)) != 0)
  {
    ens161_force_stop();
    return -7;
  }

  ptr_i2c1->CTL0 &= ~(1U << 10);
  ptr_i2c1->CTL0 |= (1U << 9);

  if (ens161_wait_set(&ptr_i2c1->STAT0, (1U << 6), ENS161_I2C_TIMEOUT_CYCLES) != 0)
  {
    ens161_force_stop();
    ptr_i2c1->CTL0 |= (1U << 10);
    return -8;
  }

  *value = (uint8_t)ptr_i2c1->DATA;
  ptr_i2c1->CTL0 |= (1U << 10);

  return 0;
}

static int ens161_read_u16(uint8_t addr_7bit, uint8_t reg_addr_lsb, uint16_t *value)
{
  uint8_t low_byte = 0;
  uint8_t high_byte = 0;
  int status;

  if (value == 0)
  {
    return -20;
  }

  status = ens161_read_reg(addr_7bit, reg_addr_lsb, &low_byte);
  if (status != 0)
  {
    return status;
  }

  status = ens161_read_reg(addr_7bit, (uint8_t)(reg_addr_lsb + 1U), &high_byte);
  if (status != 0)
  {
    return status;
  }

  *value = (uint16_t)(((uint16_t)high_byte << 8) | low_byte);
  return 0;
}

static void ens161_tft_draw_widgets(void)
{
  tft_set_background_color(12, 20, 28);

  tft_draw_rect(TFT_ID_WIDGET_TITLE_B, 10, 10, 460, 40, 0, 190, 255);
  tft_draw_text(TFT_ID_WIDGET_TITLE_L, 20, 20, 255, 255, 255, "ENS161 AIR QUALITY DASHBOARD");

  tft_draw_rect(TFT_ID_WIDGET_STATUS_B, 10, 70, 220, 45, 0, 190, 255);
  tft_draw_rect(TFT_ID_WIDGET_AQI_B, 250, 70, 220, 45, 0, 190, 255);
  tft_draw_rect(TFT_ID_WIDGET_TVOC_B, 10, 130, 220, 45, 0, 190, 255);
  tft_draw_rect(TFT_ID_WIDGET_ECO2_B, 250, 130, 220, 45, 0, 190, 255);

  tft_draw_text(TFT_ID_WIDGET_STATUS_L, 22, 84, 220, 220, 220, "STATUS");
  tft_draw_text(TFT_ID_WIDGET_AQI_L, 262, 84, 220, 220, 220, "AQI");
  tft_draw_text(TFT_ID_WIDGET_TVOC_L, 22, 144, 220, 220, 220, "TVOC (ppb)");
  tft_draw_text(TFT_ID_WIDGET_ECO2_L, 262, 144, 220, 220, 220, "eCO2 (ppm)");

  tft_draw_compass(TFT_ID_WIDGET_COMPASS, 240, 240, 120);
  tft_draw_text(TFT_ID_WIDGET_COMPASS_L, 180, 310, 220, 220, 220, "AQI Compass");
}

static void ens161_tft_update_values(uint8_t status_reg, uint8_t aqi, uint16_t tvoc_ppb, uint16_t eco2_ppm)
{
  char text[40];
  uint16_t compass_scale = 0;

  tft_delete_obj(TFT_ID_WIDGET_STATUS_V);
  tft_delete_obj(TFT_ID_WIDGET_AQI_V);
  tft_delete_obj(TFT_ID_WIDGET_TVOC_V);
  tft_delete_obj(TFT_ID_WIDGET_ECO2_V);

  snprintf(text, sizeof(text), "0x%02X", status_reg);
  tft_draw_text(TFT_ID_WIDGET_STATUS_V, 130, 84, 255, 255, 255, text);

  snprintf(text, sizeof(text), "%u", aqi);
  tft_draw_text(TFT_ID_WIDGET_AQI_V, 340, 84, 255, 255, 255, text);

  snprintf(text, sizeof(text), "%u", tvoc_ppb);
  tft_draw_text(TFT_ID_WIDGET_TVOC_V, 130, 144, 255, 255, 255, text);

  snprintf(text, sizeof(text), "%u", eco2_ppm);
  tft_draw_text(TFT_ID_WIDGET_ECO2_V, 380, 144, 255, 255, 255, text);

  if (aqi > 5U)
  {
    aqi = 5U;
  }

  compass_scale = (uint16_t)(((uint32_t)aqi * 3600U) / 5U);
  tft_set_compass_value(TFT_ID_WIDGET_COMPASS, compass_scale);
}

int tft_test(void)
{
  char image_prompt[] = "\r\n\tDisplaying IMG_4346.png as an icon.\r\n";

  usartx_send_text(ptr_usart1, image_prompt);

  // Draw the stored image using the icon API and the module's zoom parameter.
  tft_draw_icon(3, 0, 0, 256, "IMG_4346.png");
  delay_systick_us(200000);

  return 0;
}

int tft_background_color_test(void)
{
  char color_prompt[] = "\r\n\tCycling TFT background colors every 1 second.\r\n";

  usartx_send_text(ptr_usart1, color_prompt);

  while(1)
  {
    tft_set_background_color(255, 0, 0);
    delay_systick_us(2000000);

    tft_set_background_color(0, 255, 0);
    delay_systick_us(2000000);

    tft_set_background_color(0, 0, 255);
    delay_systick_us(2000000);

    tft_set_background_color(255, 255, 255);
    delay_systick_us(2000000);

    if((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B))
    {
      break;
    }
  }

  return 0;
}

static void weather_station_draw_ui(void)
{
  tft_set_background_color(0, 0, 50); // Deep blue background

  // Title
  tft_draw_rect(60, 10, 10, 460, 40, 255, 255, 255);
  tft_draw_text(61, 80, 20, 0, 0, 0, "ENS161 WEATHER STATION");

  // Labels
  tft_draw_text(62, 30, 80, 200, 200, 200, "AQI Index:");
  tft_draw_text(63, 30, 130, 200, 200, 200, "TVOC (ppb):");
  tft_draw_text(64, 30, 180, 200, 200, 200, "eCO2 (ppm):");

  // Units/Status Label
  tft_draw_text(65, 30, 230, 200, 200, 200, "Status:");

  // Compass for AQI
  tft_draw_compass(TFT_ID_WIDGET_COMPASS, 300, 200, 150);
  tft_draw_text(66, 310, 290, 255, 255, 255, "AQI");
}

static void weather_station_update_data(uint8_t aqi, uint16_t tvoc, uint16_t eco2, uint8_t status)
{
  char buf[32];

  // Delete old values
  tft_delete_obj(70);
  tft_delete_obj(71);
  tft_delete_obj(72);
  tft_delete_obj(73);

  // AQI
  snprintf(buf, sizeof(buf), "%u", aqi);
  tft_draw_text(70, 180, 80, 255, 255, 255, buf);

  // TVOC
  snprintf(buf, sizeof(buf), "%u", tvoc);
  tft_draw_text(71, 180, 130, 255, 255, 255, buf);

  // eCO2
  snprintf(buf, sizeof(buf), "%u", eco2);
  tft_draw_text(72, 180, 180, 255, 255, 255, buf);

  // Status
  snprintf(buf, sizeof(buf), "0x%02X", status);
  tft_draw_text(73, 180, 230, 255, 255, 255, buf);

  // Compass Update
  if (aqi > 5) aqi = 5;
  uint16_t angle = (uint16_t)(aqi * 720); // 0-3600 scale
  tft_set_compass_value(TFT_ID_WIDGET_COMPASS, angle);
}

int weather_station_example(void)
{
  uint8_t aqi, status_reg;
  uint16_t tvoc, eco2;
  uint8_t ens_addr = 0x53; // Default HIGH address
  int res;

  usartx_send_text(ptr_usart1, "\r\nStarting Weather Station Example...\r\n");

  weather_station_draw_ui();

  // Try to find the sensor
  res = ens161_read_reg(0x53, ENS161_REG_PART_ID_L, &status_reg);
  if (res != 0) {
    res = ens161_read_reg(0x52, ENS161_REG_PART_ID_L, &status_reg);
    if (res == 0) ens_addr = 0x52;
    else {
      usartx_send_text(ptr_usart1, "ENS161 not found!\r\n");
      return -1;
    }
  }

  // Initialize Standard mode
  ens161_write_reg(ens_addr, ENS161_REG_OPMODE, ENS161_OPMODE_STD);
  delay_systick_us(10000);

  while(1)
  {
    ens161_read_reg(ens_addr, ENS161_REG_STATUS, &status_reg);
    ens161_read_reg(ens_addr, ENS161_REG_DATA_AQI, &aqi);
    ens161_read_u16(ens_addr, ENS161_REG_DATA_TVOC, &tvoc);
    ens161_read_u16(ens_addr, ENS161_REG_DATA_ECO2, &eco2);

    weather_station_update_data(aqi, tvoc, eco2, status_reg);

    delay_systick_us(1000000); // 1s update

    if ((g_usart1_rx_count > 0) && (g_usart1_rx_arr[g_usart1_rx_count - 1] == 0x1B))
    {
      break;
    }
  }

  return 0;
}

int ens161_config_test(void)
{
  char prompt[] = "\r\n\tConfiguring ENS161-BGT and streaming STATUS/AQI/TVOC/eCO2 (Esc to stop).\r\n";
  char result[128];
  uint8_t part_id_l;
  uint8_t opmode_readback;
  uint8_t status_reg;
  uint8_t aqi;
  uint16_t tvoc_ppb;
  uint16_t eco2_ppm;
  uint8_t ens161_addr_7bit = 0;
  int status;
  uint8_t addr_candidates[] = {0x52, 0x53};
  uint32_t i;

  usartx_send_text(ptr_usart1, prompt);
  ens161_tft_draw_widgets();

  for (i = 0; i < (sizeof(addr_candidates) / sizeof(addr_candidates[0])); i++)
  {
    status = ens161_read_reg(addr_candidates[i], ENS161_REG_PART_ID_L, &part_id_l);
    if (status == 0)
    {
      ens161_addr_7bit = addr_candidates[i];
      break;
    }
  }

  if (ens161_addr_7bit == 0)
  {
    snprintf(result, sizeof(result),
             "\tENS161 not detected on 0x52/0x53 (check wiring/pull-ups/power).\r\n");
    usartx_send_text(ptr_usart1, result);
    return -2;
  }

  status = ens161_write_reg(ens161_addr_7bit, ENS161_REG_OPMODE, ENS161_OPMODE_STD);
  if (status != 0)
  {
    snprintf(result, sizeof(result),
             "\tENS161 write failed at 0x%02X (status %d)\r\n",
             ens161_addr_7bit,
             status);
    usartx_send_text(ptr_usart1, result);
    return -3;
  }

  delay_systick_us(10000);

  status = ens161_read_reg(ens161_addr_7bit, ENS161_REG_OPMODE, &opmode_readback);
  if (status != 0)
  {
    snprintf(result, sizeof(result),
             "\tENS161 readback failed at 0x%02X (status %d)\r\n",
             ens161_addr_7bit,
             status);
    usartx_send_text(ptr_usart1, result);
    return -4;
  }

  if (opmode_readback != ENS161_OPMODE_STD)
  {
    snprintf(result, sizeof(result),
             "\tENS161 config failed: ADDR=0x%02X PART_ID_L=0x%02X OPMODE=0x%02X\r\n",
             ens161_addr_7bit,
             part_id_l,
             opmode_readback);
    usartx_send_text(ptr_usart1, result);
    return -1;
  }

  snprintf(result, sizeof(result),
           "\tENS161 configured: ADDR=0x%02X PART_ID_L=0x%02X OPMODE=0x%02X\r\n",
           ens161_addr_7bit,
           part_id_l,
           opmode_readback);
  usartx_send_text(ptr_usart1, result);

  while (1)
  {
    status = ens161_read_reg(ens161_addr_7bit, ENS161_REG_STATUS, &status_reg);
    if (status != 0)
    {
      snprintf(result, sizeof(result), "\tENS161 STATUS read failed (%d)\r\n", status);
      usartx_send_text(ptr_usart1, result);
      return -5;
    }

    status = ens161_read_reg(ens161_addr_7bit, ENS161_REG_DATA_AQI, &aqi);
    if (status != 0)
    {
      snprintf(result, sizeof(result), "\tENS161 AQI read failed (%d)\r\n", status);
      usartx_send_text(ptr_usart1, result);
      return -6;
    }

    status = ens161_read_u16(ens161_addr_7bit, ENS161_REG_DATA_TVOC, &tvoc_ppb);
    if (status != 0)
    {
      snprintf(result, sizeof(result), "\tENS161 TVOC read failed (%d)\r\n", status);
      usartx_send_text(ptr_usart1, result);
      return -7;
    }

    status = ens161_read_u16(ens161_addr_7bit, ENS161_REG_DATA_ECO2, &eco2_ppm);
    if (status != 0)
    {
      snprintf(result, sizeof(result), "\tENS161 eCO2 read failed (%d)\r\n", status);
      usartx_send_text(ptr_usart1, result);
      return -8;
    }

    snprintf(result, sizeof(result),
             "\tSTATUS=0x%02X AQI=%u TVOC=%u ppb eCO2=%u ppm\r\n",
             status_reg,
             aqi,
             tvoc_ppb,
             eco2_ppm);
    usartx_send_text(ptr_usart1, result);
    ens161_tft_update_values(status_reg, aqi, tvoc_ppb, eco2_ppm);

    delay_systick_us(1000000);

    if ((g_usart1_rx_count > 0U) && (g_usart1_rx_arr[g_usart1_rx_count - 1U] == 0x1B))
    {
      break;
    }
  }

  return 0;
}
