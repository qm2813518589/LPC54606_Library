/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2016 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        16. February 2016
 * $Revision:    V1.0
 *
 * Project:      SPI Driver Definitions for NXP LPC5410x
 * -------------------------------------------------------------------------- */

#ifndef __SPI_LPC5410X_H
#define __SPI_LPC5410X_H

#include <string.h>

#include "chip.h"
#include "Driver_SPI.h"

#include "RTE_Components.h"
#include "RTE_Device.h"

#if ((defined(RTE_Drivers_SPI0) || \
      defined(RTE_Drivers_SPI1) || \
      defined(RTE_Drivers_SPI2))   \
     && (RTE_SPI0 == 0)            \
     && (RTE_SPI1 == 0))

#error "SPI not configured in RTE_Device.h!"
#endif

/* IOCON settings for SPI pins */
#define SPI_PIN_IOCON_RESET     (IOCON_MODE_PULLUP | \
                                 IOCON_DIGITAL_EN  | \
                                 IOCON_INPFILT_OFF)

#define SPI_PIN_IOCON           (IOCON_MODE_INACT | \
                                 IOCON_DIGITAL_EN | \
                                 IOCON_INPFILT_OFF)
                                 

/* I2C interrupt mask */
#define I2C_MASTER_INTERRUPTS   (I2C_INTENSET_MSTPENDING  | \
                                 I2C_INTENSET_MSTRARBLOSS | \
                                 I2C_INTENSET_MSTSTSTPERR)

#define I2C_SLAVE_INTERRUPTS    (I2C_INTENSET_SLVPENDING | \
                                 I2C_INTENSET_SLVNOTSTR  | \
                                 I2C_INTENSET_SLVDESEL)

/* I2C Driver state flags */
#define SPI_INIT       (uint8_t)(1U << 0)   // Driver initialized
#define SPI_POWER      (uint8_t)(1U << 1)   // Driver power on
#define SPI_SETUP      (uint8_t)(1U << 2)   // Driver configured
#define SPI_SS_SWCTRL  (uint8_t)(1U << 3)   // Software controlled SSEL
#define SPI_16B_FRAME  (uint8_t)(1U << 4)   // Frame length is 2 bytes

/* SPI Transfer Information (Run-Time) */
typedef struct spi_transfer_info {
  uint32_t              num;                // Total number of transfers
  uint8_t              *rx_buf;             // Rx data pointer
  uint8_t              *tx_buf;             // Tx data pointer
  uint32_t              rx_cnt;             // Rx data counter
  uint32_t              tx_cnt;             // Tx data counter
  uint32_t              def_val;            // Default transfer value
  uint32_t              dummy;              // Data dump variable
} SPI_TRANSFER_INFO;

/* SPI status */
typedef struct spi_status {
  uint8_t busy;                             // Tx/Rx busy flag
  uint8_t data_lost;                        // Data lost: Rx overflow / Tx underflow
} SPI_STATUS;

/* I2C Control Information */
typedef struct spi_info {
  ARM_SPI_SignalEvent_t cb_event;           // Event callback
  SPI_TRANSFER_INFO     xfer;               // Transfer configuration
  SPI_STATUS            status;             // Status flags
  uint8_t               flags;              // Control and state flags
} SPI_INFO;

typedef struct spi_pin {
  uint8_t port;                             // Port number
  uint8_t pin;                              // Pin number
  uint8_t func;                             // Pin function
} SPI_PIN;

typedef struct spi_ssel {
  uint8_t port;                             // Port number
  uint8_t pin;                              // Pin number
  uint8_t func;                             // Pin function
  uint8_t num;                              // Slave select number
} const SPI_SSEL;

/* SPI Resource Configuration */
typedef struct spi_resources {
  LPC_SPI_T            *reg;                // SPI register interface
  IRQn_Type             irqn;               // SPI IRQ Number in NVIC
  CHIP_SYSCON_CLOCK_T   clk_msk;            // APB clock control bit mask
  SPI_PIN               mosi;               // SPI MOSI pin
  SPI_PIN               miso;               // SPI MISO pin
  SPI_PIN               sck;                // SPI SCK pin
  SPI_SSEL             *ssel;               // SPI SSEL pin
  SPI_INFO             *info;               // Run-Time control information
} const SPI_RESOURCES;

#endif /* __SPI_LPC5410X_H */
