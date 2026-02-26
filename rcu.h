#ifndef H_RCU
#define H_RCU

#include "gd32f4xx.h"

#define RCU_BASE_ADDR (0x40023800)

typedef struct {
    __IOM uint32_t CTL;           // Control Register
    __IOM uint32_t PLL;           // PLL Configuration Register
    __IOM uint32_t CFG0;          // Configuration Register 0
    __IOM uint32_t INT;           // Interrupt Register
    __IOM uint32_t AHB1RST;       // AHB Peripheral Reset Register
    __IOM uint32_t AHB2RST;       // AHB Peripheral Reset Register
    __IOM uint32_t AHB3RST;       // AHB Peripheral Reset Register
          uint32_t reserved1;
    __IOM uint32_t APB1RST;       // APB2 Peripheral Reset Register   
    __IOM uint32_t APB2RST;        // APB1 Peripheral Reset Register
          uint32_t reserved2[2];
    __IOM uint32_t AHB1EN;         // AHB Peripheral Clock Enable Register
    __IOM uint32_t AHB2EN;         // AHB Peripheral Clock Enable Register
    __IOM uint32_t AHB3EN;         // AHB Peripheral Clock Enable Register
          uint32_t reserved3;
    __IOM uint32_t APB1EN;          // APB2 Peripheral Clock Enable Register
    __IOM uint32_t APB2EN;          // APB1 Peripheral Clock Enable Register
          uint32_t reserved4[2];
    __IOM uint32_t AHB1SPEN;         // AHB Peripheral Clock Enable Register
    __IOM uint32_t AHB2SPEN;         // AHB Peripheral Clock Enable Register
    __IOM uint32_t AHB3SPEN;         // AHB Peripheral Clock Enable Register
          uint32_t reserved5;
    __IOM uint32_t APB1SPEN;         // AHB Peripheral Clock Enable Register
    __IOM uint32_t APB2SPEN;         // AHB Peripheral Clock Enable Register
          uint32_t reserved6[2];
    __IOM uint32_t BDCTL;             // Backup Domain Control Register
    __IOM uint32_t RSTSCK;            // Reset Source / Clock Source Register
          uint32_t reserved7[2];
    __IOM uint32_t PLLSSCTL;           // AHB Peripheral Clock Enable Register
    __IOM uint32_t PLLI2S;             // AHB Peripheral Clock Enable Register
    __IOM uint32_t PLLSAI;             // AHB Peripheral Clock Enable Register
    __IOM uint32_t CFG1;               // Configuration Register 1
          uint32_t reserved8[12];
    __IOM uint32_t ADDCTL;               // Deep Sleep Mode Voltage Register
          uint32_t reserved9[2];
    __IOM uint32_t ADDINT;                 // AHB Peripheral Clock Enable Register
          uint32_t reserved10[4];
    __IOM uint32_t ADDAPB1RST;             // PLLI2S Configuration Register
    __IOM uint32_t ADDAPB1EN;             // AHB Peripheral Clock Enable Register
    __IOM uint32_t ADDAPB1SPEN;           // AHB Peripheral Clock Enable Register
          uint32_t reserved11[5];
    __IOM uint32_t VKEY;                   // AHB Peripheral Clock Enable Register
          uint32_t reserved12[11];
    __IOM uint32_t DSV;                  // AHB Peripheral Clock Enable Register
} rcu_t;

// Define a pointer to the base address of RCU

#define ptr_rcu  ((rcu_t*)RCU_BASE_ADDR)

#define PCEN_MASK  (1 << 2)








#endif