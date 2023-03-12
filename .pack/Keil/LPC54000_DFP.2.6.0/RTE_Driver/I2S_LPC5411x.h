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
 * $Date:        11. August 2016
 * $Revision:    V1.1
 *
 * Project:      SAI Driver Definitions for NXP LPC5411x
 * -------------------------------------------------------------------------- */

#ifndef __I2S_LPC5411x_H
#define __I2S_LPC5411x_H

#include "chip.h"
#include "Driver_SAI.h"

#include "RTE_Device.h"

#if (((RTE_FC6 == 0) || (RTE_FC6_PERSEL != 3)) && \
     ((RTE_FC7 == 0) || (RTE_FC7_PERSEL != 3)))

#error "Flexcomm Interface not configured for SAI in RTE_Device.h!"
#endif

// SAI0 (I2S6)
#if (RTE_FC6 != 0) && (RTE_FC6_PERSEL == 3)
  #define RTE_SAI0              1

  #define RTE_I2S6_WS_PIN_EN    RTE_FC6_TXD_SCL_MISO_EN
  #define RTE_I2S6_WS_PORT      RTE_FC6_TXD_SCL_MISO_PORT
  #define RTE_I2S6_WS_BIT       RTE_FC6_TXD_SCL_MISO_PIN
  #define RTE_I2S6_WS_FUNC      RTE_FC6_TXD_SCL_MISO_FUNC

  #define RTE_I2S6_SDA_PIN_EN   RTE_FC6_RXD_SDA_MOSI_EN
  #define RTE_I2S6_SDA_PORT     RTE_FC6_RXD_SDA_MOSI_PORT
  #define RTE_I2S6_SDA_BIT      RTE_FC6_RXD_SDA_MOSI_PIN
  #define RTE_I2S6_SDA_FUNC     RTE_FC6_RXD_SDA_MOSI_FUNC

  #define RTE_I2S6_SCK_PIN_EN   RTE_FC6_SCK_EN
  #define RTE_I2S6_SCK_PORT     RTE_FC6_SCK_PORT
  #define RTE_I2S6_SCK_BIT      RTE_FC6_SCK_PIN
  #define RTE_I2S6_SCK_FUNC     RTE_FC6_SCK_FUNC
#else
  #define RTE_SAI0              0
#endif

// USART7
#if (RTE_FC7 != 0) && (RTE_FC7_PERSEL == 3)
  #define RTE_SAI1              1

  #define RTE_I2S7_WS_PIN_EN    RTE_FC7_TXD_SCL_MISO_EN
  #define RTE_I2S7_WS_PORT      RTE_FC7_TXD_SCL_MISO_PORT
  #define RTE_I2S7_WS_BIT       RTE_FC7_TXD_SCL_MISO_PIN
  #define RTE_I2S7_WS_FUNC      RTE_FC7_TXD_SCL_MISO_FUNC

  #define RTE_I2S7_SDA_PIN_EN   RTE_FC7_RXD_SDA_MOSI_EN
  #define RTE_I2S7_SDA_PORT     RTE_FC7_RXD_SDA_MOSI_PORT
  #define RTE_I2S7_SDA_BIT      RTE_FC7_RXD_SDA_MOSI_PIN
  #define RTE_I2S7_SDA_FUNC     RTE_FC7_RXD_SDA_MOSI_FUNC

  #define RTE_I2S7_SCK_PIN_EN   RTE_FC7_SCK_EN
  #define RTE_I2S7_SCK_PORT     RTE_FC7_SCK_PORT
  #define RTE_I2S7_SCK_BIT      RTE_FC7_SCK_PIN
  #define RTE_I2S7_SCK_FUNC     RTE_FC7_SCK_FUNC
#else
  #define RTE_SAI1              0
#endif

// SAI instances
#define I2S6_FLEXCOMM           6
#define I2S7_FLEXCOMM           7

// SYSCON clock defines
#define I2S6_SYSCON_CLOCK       SYSCON_CLOCK_FLEXCOMM6
#define I2S7_SYSCON_CLOCK       SYSCON_CLOCK_FLEXCOMM7

// Reset defines
#define I2S6_RESET              RESET_FLEXCOMM6
#define I2S7_RESET              RESET_FLEXCOMM7

// DMA Enable defines
#define I2S6_TX_DMA_EN          RTE_FC6_DMA_TX_I2CM
#define I2S6_RX_DMA_EN          RTE_FC6_DMA_RX_I2CS
#define I2S7_TX_DMA_EN          RTE_FC7_DMA_TX_I2CM
#define I2S7_RX_DMA_EN          RTE_FC7_DMA_RX_I2CS

// DMA Channel defines
#define I2S6_TX_DMAREQ          DMAREQ_FLEXCOMM6_TX
#define I2S6_RX_DMAREQ          DMAREQ_FLEXCOMM6_RX
#define I2S7_TX_DMAREQ          DMAREQ_FLEXCOMM7_TX
#define I2S7_RX_DMAREQ          DMAREQ_FLEXCOMM7_RX

// SAI flags
#define SAI_FLAG_INITIALIZED    (     1U)
#define SAI_FLAG_POWERED        (1U << 1)
#define SAI_FLAG_CONFIGURED     (1U << 2)

// SAI STREAMs
#define SAI_STREAM_OUT           (0U)
#define SAI_STREAM_IN            (1U)

// SAI MCLK PIN
#define SAI_MCLK_PIN_NOT_USED    (0U)
#define SAI_MCLK_PIN_PIO1_2      (1U)
#define SAI_MCLK_PIN_PIO1_17     (2U)


// I2S Stream Information (Run-Time)
typedef struct _I2S_STREAM_INFO {
  uint32_t                num;           // Total number of data to be transmitted/received
  uint8_t  * volatile     buf;           // Pointer to data buffer
  uint32_t   volatile     cnt;           // Number of data transmitted/receive
  uint32_t   volatile     num_pending;   // Number of data pending to transmit/receive
  uint8_t                 data_bits;     // Number of data bits
  uint8_t                 mute;          // Mute flag
  uint8_t    volatile     dma_int_flag;  // DMA interrupt flag
} I2S_STREAM_INFO;

// SAI Status
typedef struct _SAI_STATUS {
  uint8_t busy;                          // Transmitter or Receiver busy flag
  uint8_t under_over_flow;               // Transmit data underflow detected (cleared on start of next send operation)
                                         //  or Receive data overflow detected (cleared on start of next receive operation)
  uint8_t frame_error;                   // Sync Frame error detected (cleared on start of next send/receive operation)
} SAI_STATUS;

// SAI Information (Run-Time)
typedef struct _SAI_INFO {
  ARM_SAI_SignalEvent_t   cb_event;      // Event callback
  SAI_STATUS              status;        // Status flags
  I2S_STREAM_INFO         xfer;          // Transmit/Receive information
  uint8_t                 flags;         // SAI driver flags
} SAI_INFO;

// I2S DMA
typedef const struct _I2S_DMA {
  DMA_CHID_T              chid;          // DMA channel ID
  DMA_CALLBACK_T          callback;      // DMA channel callback
} I2S_DMA;

// I2S Pin Configuration
typedef const struct _I2S_PINS {
  PINMUX_GRP_T           *ws;            // Word select Pin
  PINMUX_GRP_T           *sda;           // Serial data Pin
  PINMUX_GRP_T           *sck;           // Bit clock Pin
} I2S_PINS;

// SAI Resources definitions
typedef struct {
  ARM_SAI_CAPABILITIES    capabilities;  // SAI Capabilities
  LPC_I2S_T              *reg;           // Pointer to I2S peripheral
  I2S_PINS                pins;          // I2S pins configuration
  I2S_DMA                *dma;           // DMA
  IRQn_Type               irq_num;       // I2S IRQ Number
  uint8_t                 fifo_lvl;      // FIFO Trigger level
  I2S_DIR_T               stream_dir;    // Stream Direction (SAI_STREAM_IN or SAI_STREAM_OUT)
  SAI_INFO               *info;          // Run-Time Information
} const SAI_RESOURCES;

#endif /* __I2S_LPC5411x_H */
