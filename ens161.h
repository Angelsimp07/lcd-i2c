#ifndef ENS161_H
#define ENS161_H

#include "gd32f4xx.h"
#include "I2C.h"

#define ENS161_I2C_ADDR_LOW     0x52
#define ENS161_I2C_ADDR_HIGH    0x53
#define ENS161_I2C_ADDR         (ENS161_I2C_ADDR_HIGH << 1)

#define ENS161_REG_PART_ID      0x00
#define ENS161_REG_OPMODE       0x10
#define ENS161_REG_CONFIG       0x11
#define ENS161_REG_COMMAND      0x12
#define ENS161_REG_DEVICE_STATUS 0x20
#define ENS161_REG_DATA_AQI     0x21
#define ENS161_REG_DATA_TVOC    0x22
#define ENS161_REG_DATA_ECO2    0x24
#define ENS161_REG_DATA_AQI_S   0x26
#define ENS161_REG_DATA_ETOH    0x28

#define ENS161_OPMODE_DEEP_SLEEP 0x00
#define ENS161_OPMODE_IDLE       0x01
#define ENS161_OPMODE_STANDARD   0x02

typedef struct {
    uint8_t aqi;
    uint16_t tvoc;
    uint16_t eco2;
    uint16_t aqi_s;
} ens161_data_t;

void ens161_init(void);
uint8_t ens161_get_status(void);
void ens161_read_data(ens161_data_t* data);

#endif
