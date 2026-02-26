#include "spi.h"
#include "rcu.h"
#include "gpio.h"

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
                 uint32_t int_priority)
{
 // Enable SPIx clock
 ptr_rcu->APB1EN |= spix_en_mask;

 // Disable SPIx before configuring its registers
 ptr_spix->CTL0 &= ~(1 << 6);

 // Clear CKPH (Bit[0]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 0);

 // Configure for the required clock phase
 ptr_spix->CTL0 |= ckph << 0;

 // Clear CKPL (Bit[1]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 1);

 // Configure for the required clock polarity
 ptr_spix->CTL0 |= ckpl << 1;

 // Clear MSTMOD (Bit[2]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 2);

 // Configure for the required operating mode
 ptr_spix->CTL0 |= mode << 2;

 // Clear PSC (Bit[5:3]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(7 << 3);

 // Configure for the required prescalar
 ptr_spix->CTL0 |= psc << 3;

 // Clear LF (Bit[7]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 7);

 // Configure for the required data sequence
 ptr_spix->CTL0 |= data_seq << 7;

 // Clear SWNSS (Bit[8]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 8);

 // Configure for the software/hardware slave select
 ptr_spix->CTL0 |= sw_mode << 8;

 // Clear SWNSSEN (Bit[9]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 9);

 // Configure for the required software pin mode
 ptr_spix->CTL0 |= sw_pin_mode << 9;

 // Clear FF16 (Bit[11]) in SPIx_CTL0
 ptr_spix->CTL0 &= ~(1 << 11);

 // Configure for the required frame length
 ptr_spix->CTL0 |= frame_length << 11;

 // Enable the required interrupt
 ptr_spix->CTL1 |= interrupt_mask;

 // Enable SPIx
 ptr_spix->CTL0 |= (1 << 6);

 // Configure for the interrupt priority
 NVIC_SetPriority(spix_irqn, int_priority);

 // Enable NVIC level interrupt
 NVIC_EnableIRQ(spix_irqn);
}

void spix_send_data(spi_t* ptr_spix, uint8_t data)
{
 // Wait until the transmit data buffer is empty
 while((ptr_spix->STAT & (1 << 1)) != (1 << 1));

 // Select the slave by driving PB12 (SPI1_NSS) LOW
 ptr_gpiob->OCTL &= ~(1 << 12);

 // Write the data to SPIx_DATA register
 ptr_spix->DATA = data;

 // Wait until the transmission is complete
 while((ptr_spix->STAT & (1 << 7)) == (1 << 7));

 // Deselect the slave by driving PB12 HIGH
 ptr_gpiob->OCTL |= (1 << 12);
}

uint8_t spix_mosi_read(spi_t* ptr_spix)
{
 while((ptr_spix->STAT & (1 << 0)) != (1 << 0)); // wait until STAT has a value // This reading

 return ptr_spix->DATA; // return the value from STAT using DATA
}






