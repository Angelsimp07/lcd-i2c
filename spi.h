#ifndef H_SPI
#define H_SPI

#include "gd32f4xx.h"

// Define a structure of SPI registers
typedef struct
{
 __IOM uint32_t CTL0;
 __IOM uint32_t CTL1;
 __IOM uint32_t STAT;
 __IOM uint32_t DATA;
 __IOM uint32_t CRCPOLY;
 __IM uint32_t RCRC;
 __IM uint32_t TCRC;
 __IOM uint32_t I2SCTL;
 __IOM uint32_t I2SPSC;
 uint32_t rsvd[18];
 __IOM uint32_t QCTL;
}spi_t;

#define SPI1_BASE_ADDR  (0x40003800)
#define SPI2_BASE_ADDR  (0x40003C00)

#define ptr_spi1  ((spi_t*)SPI1_BASE_ADDR)
#define ptr_spi2  ((spi_t*)SPI2_BASE_ADDR)

#define SPI1EN_MASK  (1 << 14)
#define SPI2EN_MASK  (1 << 15)

void spix_config(spi_t* ptr_spix,
                 uint32_t spix_en_mask,
                 uint32_t ckph,
                 uint32_t ckpl,
                 uint32_t mode,
                 uint32_t psc,
                 uint32_t data_seq,
                 uint32_t sw_mode,
                 uint32_t sw_pin_mode,
                 uint32_t frame_length,
                 uint32_t interrupt_mask,
                 uint32_t spix_irqn,
                 uint32_t int_priority);

void spix_send_data(spi_t* ptr_spix, uint8_t data);

int spi1_leader_transmits(void);

uint8_t spix_mosi_read(spi_t* ptr_spix);

int spi1_follower_receives(void);

#endif