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
 * $Date:        11. January 2016
 * $Revision:    V1.0
 *
 * Project:      USART Driver Definitions for NXP LPC5410x
 * -------------------------------------------------------------------------- */

#ifndef __USART_LPC5410x_H
#define __USART_LPC5410x_H

#include "chip.h"
#include "Driver_USART.h"

// USART CFG register definitions
#define UART_CFG_IRDA_MODE           (1U << 16)


// USART flags
#define USART_FLAG_INITIALIZED       (1U << 0)
#define USART_FLAG_POWERED           (1U << 1)
#define USART_FLAG_CONFIGURED        (1U << 2)

// USART synchronous xfer modes
#define USART_SYNC_MODE_TX           ( 1U )
#define USART_SYNC_MODE_RX           ( 2U )
#define USART_SYNC_MODE_TX_RX        (USART_SYNC_MODE_TX | \
                                      USART_SYNC_MODE_RX)


// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
  uint32_t            rx_num;            // Total number of data to be received
  uint32_t            tx_num;            // Total number of data to be send
  uint8_t  * volatile rx_buf;            // Pointer to in data buffer
  uint8_t  * volatile tx_buf;            // Pointer to out data buffer
  uint32_t   volatile rx_cnt;            // Number of data received
  uint32_t   volatile tx_cnt;            // Number of data sent
  uint8_t             tx_def_val;        // Transmit default value (used in USART_SYNC_MASTER_MODE_RX)
  uint8_t             rx_dump_val;       // Receive dump value (used in USART_SYNC_MASTER_MODE_TX)
  uint8_t    volatile send_active;       // Send active flag
  uint8_t             sync_mode;         // Synchronous mode
  uint8_t             receiver_enabled;  // Receiver enable flag
} USART_TRANSFER_INFO;

typedef struct _USART_RX_STATUS {
  uint8_t  rx_busy;                      // Receiver busy flag
  uint8_t  rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t  rx_break;                     // Break detected on receive (cleared on start of next receive operation)
  uint8_t  rx_framing_error;             // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t  rx_parity_error;              // Parity error detected on receive (cleared on start of next receive operation)
} USART_RX_STATUS;

// USART Information (Run-Time)
typedef struct _USART_INFO {
  ARM_USART_SignalEvent_t cb_event;      // Event callback
  USART_RX_STATUS         rx_status;     // Receive status flags
  USART_TRANSFER_INFO     xfer;          // Transfer information
  uint8_t                 mode;          // USART mode
  uint8_t                 flags;         // USART driver flags
  uint32_t                baudrate;      // Baudrate
} USART_INFO;

// USART Pin Configuration
typedef const struct _USART_PINS {
  PINMUX_GRP_T           *txd;           // TXD  Pin identifier
  PINMUX_GRP_T           *rxd;           // RXD  Pin identifier
  PINMUX_GRP_T           *sclk;          // SCLK Pin identifier
  PINMUX_GRP_T           *cts;           // CTS  Pin identifier
  PINMUX_GRP_T           *rts;           // RTS  Pin identifier
} USART_PINS;

// USART Clocks Configuration
typedef const struct _USART_CLOCK {
  __IO uint32_t          *reg_cfg;       // USART register interface clock configuration register
  __I  uint32_t          *reg_stat;      // USART register interface clock status register
  __IO uint32_t          *peri_cfg;      // USART peripheral clock configuration register
  __I  uint32_t          *peri_stat;     // USART peripheral clock status register
  __IO uint32_t          *base_clk;      // USART base clock
} USART_CLOCKS;

// USART Reset Configuration
typedef const struct _USART_RESET {
       uint32_t           reg_cfg_val;   // USART reset bit 
  __IO uint32_t          *reg_cfg;       // USART reset control register
  __I  uint32_t          *reg_stat;      // USART reset active status register
} USART_RESET;

// USART Resources definitions
typedef struct {
  ARM_USART_CAPABILITIES  capabilities;    // Capabilities
  LPC_USART_T                *reg;         // Pointer to USART peripheral
  USART_PINS                  pins;        // USART pins configuration
  CHIP_SYSCON_PERIPH_RESET_T  reset;       // USART Reset
  uint8_t                     oversample;  // Oversample
  IRQn_Type                   irq_num;     // USART IRQ Number
  USART_INFO                 *info;        // Run-Time Information
} const USART_RESOURCES;

#endif /* __USART_LPC5410x_H */
