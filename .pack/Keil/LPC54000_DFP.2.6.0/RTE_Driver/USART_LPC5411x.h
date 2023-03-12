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
 * $Date:        9. March 2016
 * $Revision:    V1.0
 *
 * Project:      USART Driver Definitions for NXP LPC5411x
 * -------------------------------------------------------------------------- */

#ifndef __USART_LPC5411x_H
#define __USART_LPC5411x_H

#include "chip.h"
#include "Driver_USART.h"

#include "RTE_Device.h"
#include "RTE_Components.h"

#if (((RTE_FC0 == 0) || (RTE_FC0_PERSEL != 2)) && \
     ((RTE_FC1 == 0) || (RTE_FC1_PERSEL != 2)) && \
     ((RTE_FC2 == 0) || (RTE_FC2_PERSEL != 2)) && \
     ((RTE_FC3 == 0) || (RTE_FC3_PERSEL != 2)) && \
     ((RTE_FC4 == 0) || (RTE_FC4_PERSEL != 2)) && \
     ((RTE_FC5 == 0) || (RTE_FC5_PERSEL != 2)) && \
     ((RTE_FC6 == 0) || (RTE_FC6_PERSEL != 2)) && \
     ((RTE_FC7 == 0) || (RTE_FC7_PERSEL != 2)))

#error "Flexcomm Interface not configured for USART in RTE_Device.h!"
#endif

// Enable USART instances

// USART0
#if (RTE_FC0 != 0) && (RTE_FC0_PERSEL == 2)
  #define RTE_USART0               1

  #define RTE_USART0_TXD_PIN_EN    RTE_FC0_TXD_SCL_MISO_EN
  #define RTE_USART0_TXD_PORT      RTE_FC0_TXD_SCL_MISO_PORT
  #define RTE_USART0_TXD_BIT       RTE_FC0_TXD_SCL_MISO_PIN
  #define RTE_USART0_TXD_FUNC      RTE_FC0_TXD_SCL_MISO_FUNC

  #define RTE_USART0_RXD_PIN_EN    RTE_FC0_RXD_SDA_MOSI_EN
  #define RTE_USART0_RXD_PORT      RTE_FC0_RXD_SDA_MOSI_PORT
  #define RTE_USART0_RXD_BIT       RTE_FC0_RXD_SDA_MOSI_PIN
  #define RTE_USART0_RXD_FUNC      RTE_FC0_RXD_SDA_MOSI_FUNC

  #define RTE_USART0_SCK_PIN_EN    RTE_FC0_SCK_EN
  #define RTE_USART0_SCK_PORT      RTE_FC0_SCK_PORT
  #define RTE_USART0_SCK_BIT       RTE_FC0_SCK_PIN
  #define RTE_USART0_SCK_FUNC      RTE_FC0_SCK_FUNC

  #define RTE_USART0_CTS_PIN_EN    RTE_FC0_CTS_SDA_SSEL0_EN
  #define RTE_USART0_CTS_PORT      RTE_FC0_CTS_SDA_SSEL0_PORT
  #define RTE_USART0_CTS_BIT       RTE_FC0_CTS_SDA_SSEL0_PIN
  #define RTE_USART0_CTS_FUNC      RTE_FC0_CTS_SDA_SSEL0_FUNC

  #define RTE_USART0_RTS_PIN_EN    RTE_FC0_RTS_SCL_SSEL1_EN
  #define RTE_USART0_RTS_PORT      RTE_FC0_RTS_SCL_SSEL1_PORT
  #define RTE_USART0_RTS_BIT       RTE_FC0_RTS_SCL_SSEL1_PIN
  #define RTE_USART0_RTS_FUNC      RTE_FC0_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART0               0
#endif

// USART1
#if (RTE_FC1 != 0) && (RTE_FC1_PERSEL == 2)
  #define RTE_USART1               1

  #define RTE_USART1_TXD_PIN_EN    RTE_FC1_TXD_SCL_MISO_EN
  #define RTE_USART1_TXD_PORT      RTE_FC1_TXD_SCL_MISO_PORT
  #define RTE_USART1_TXD_BIT       RTE_FC1_TXD_SCL_MISO_PIN
  #define RTE_USART1_TXD_FUNC      RTE_FC1_TXD_SCL_MISO_FUNC

  #define RTE_USART1_RXD_PIN_EN    RTE_FC1_RXD_SDA_MOSI_EN
  #define RTE_USART1_RXD_PORT      RTE_FC1_RXD_SDA_MOSI_PORT
  #define RTE_USART1_RXD_BIT       RTE_FC1_RXD_SDA_MOSI_PIN
  #define RTE_USART1_RXD_FUNC      RTE_FC1_RXD_SDA_MOSI_FUNC

  #define RTE_USART1_SCK_PIN_EN    RTE_FC1_SCK_EN
  #define RTE_USART1_SCK_PORT      RTE_FC1_SCK_PORT
  #define RTE_USART1_SCK_BIT       RTE_FC1_SCK_PIN
  #define RTE_USART1_SCK_FUNC      RTE_FC1_SCK_FUNC

  #define RTE_USART1_CTS_PIN_EN    RTE_FC1_CTS_SDA_SSEL0_EN
  #define RTE_USART1_CTS_PORT      RTE_FC1_CTS_SDA_SSEL0_PORT
  #define RTE_USART1_CTS_BIT       RTE_FC1_CTS_SDA_SSEL0_PIN
  #define RTE_USART1_CTS_FUNC      RTE_FC1_CTS_SDA_SSEL0_FUNC

  #define RTE_USART1_RTS_PIN_EN    RTE_FC1_RTS_SCL_SSEL1_EN
  #define RTE_USART1_RTS_PORT      RTE_FC1_RTS_SCL_SSEL1_PORT
  #define RTE_USART1_RTS_BIT       RTE_FC1_RTS_SCL_SSEL1_PIN
  #define RTE_USART1_RTS_FUNC      RTE_FC1_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART1               0
#endif

// USART2
#if (RTE_FC2 != 0) && (RTE_FC2_PERSEL == 2)
  #define RTE_USART2               1

  #define RTE_USART2_TXD_PIN_EN    RTE_FC2_TXD_SCL_MISO_EN
  #define RTE_USART2_TXD_PORT      RTE_FC2_TXD_SCL_MISO_PORT
  #define RTE_USART2_TXD_BIT       RTE_FC2_TXD_SCL_MISO_PIN
  #define RTE_USART2_TXD_FUNC      RTE_FC2_TXD_SCL_MISO_FUNC

  #define RTE_USART2_RXD_PIN_EN    RTE_FC2_RXD_SDA_MOSI_EN
  #define RTE_USART2_RXD_PORT      RTE_FC2_RXD_SDA_MOSI_PORT
  #define RTE_USART2_RXD_BIT       RTE_FC2_RXD_SDA_MOSI_PIN
  #define RTE_USART2_RXD_FUNC      RTE_FC2_RXD_SDA_MOSI_FUNC

  #define RTE_USART2_SCK_PIN_EN    RTE_FC2_SCK_EN
  #define RTE_USART2_SCK_PORT      RTE_FC2_SCK_PORT
  #define RTE_USART2_SCK_BIT       RTE_FC2_SCK_PIN
  #define RTE_USART2_SCK_FUNC      RTE_FC2_SCK_FUNC

  #define RTE_USART2_CTS_PIN_EN    RTE_FC2_CTS_SDA_SSEL0_EN
  #define RTE_USART2_CTS_PORT      RTE_FC2_CTS_SDA_SSEL0_PORT
  #define RTE_USART2_CTS_BIT       RTE_FC2_CTS_SDA_SSEL0_PIN
  #define RTE_USART2_CTS_FUNC      RTE_FC2_CTS_SDA_SSEL0_FUNC

  #define RTE_USART2_RTS_PIN_EN    RTE_FC2_RTS_SCL_SSEL1_EN
  #define RTE_USART2_RTS_PORT      RTE_FC2_RTS_SCL_SSEL1_PORT
  #define RTE_USART2_RTS_BIT       RTE_FC2_RTS_SCL_SSEL1_PIN
  #define RTE_USART2_RTS_FUNC      RTE_FC2_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART2               0
#endif

// USART3
#if (RTE_FC3 != 0) && (RTE_FC3_PERSEL == 2)
  #define RTE_USART3               1

  #define RTE_USART3_TXD_PIN_EN    RTE_FC3_TXD_SCL_MISO_EN
  #define RTE_USART3_TXD_PORT      RTE_FC3_TXD_SCL_MISO_PORT
  #define RTE_USART3_TXD_BIT       RTE_FC3_TXD_SCL_MISO_PIN
  #define RTE_USART3_TXD_FUNC      RTE_FC3_TXD_SCL_MISO_FUNC

  #define RTE_USART3_RXD_PIN_EN    RTE_FC3_RXD_SDA_MOSI_EN
  #define RTE_USART3_RXD_PORT      RTE_FC3_RXD_SDA_MOSI_PORT
  #define RTE_USART3_RXD_BIT       RTE_FC3_RXD_SDA_MOSI_PIN
  #define RTE_USART3_RXD_FUNC      RTE_FC3_RXD_SDA_MOSI_FUNC

  #define RTE_USART3_SCK_PIN_EN    RTE_FC3_SCK_EN
  #define RTE_USART3_SCK_PORT      RTE_FC3_SCK_PORT
  #define RTE_USART3_SCK_BIT       RTE_FC3_SCK_PIN
  #define RTE_USART3_SCK_FUNC      RTE_FC3_SCK_FUNC

  #define RTE_USART3_CTS_PIN_EN    RTE_FC3_CTS_SDA_SSEL0_EN
  #define RTE_USART3_CTS_PORT      RTE_FC3_CTS_SDA_SSEL0_PORT
  #define RTE_USART3_CTS_BIT       RTE_FC3_CTS_SDA_SSEL0_PIN
  #define RTE_USART3_CTS_FUNC      RTE_FC3_CTS_SDA_SSEL0_FUNC

  #define RTE_USART3_RTS_PIN_EN    RTE_FC3_RTS_SCL_SSEL1_EN
  #define RTE_USART3_RTS_PORT      RTE_FC3_RTS_SCL_SSEL1_PORT
  #define RTE_USART3_RTS_BIT       RTE_FC3_RTS_SCL_SSEL1_PIN
  #define RTE_USART3_RTS_FUNC      RTE_FC3_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART3               0
#endif

// USART4
#if (RTE_FC4 != 0) && (RTE_FC4_PERSEL == 2)
  #define RTE_USART4               1

  #define RTE_USART4_TXD_PIN_EN    RTE_FC4_TXD_SCL_MISO_EN
  #define RTE_USART4_TXD_PORT      RTE_FC4_TXD_SCL_MISO_PORT
  #define RTE_USART4_TXD_BIT       RTE_FC4_TXD_SCL_MISO_PIN
  #define RTE_USART4_TXD_FUNC      RTE_FC4_TXD_SCL_MISO_FUNC

  #define RTE_USART4_RXD_PIN_EN    RTE_FC4_RXD_SDA_MOSI_EN
  #define RTE_USART4_RXD_PORT      RTE_FC4_RXD_SDA_MOSI_PORT
  #define RTE_USART4_RXD_BIT       RTE_FC4_RXD_SDA_MOSI_PIN
  #define RTE_USART4_RXD_FUNC      RTE_FC4_RXD_SDA_MOSI_FUNC

  #define RTE_USART4_SCK_PIN_EN    RTE_FC4_SCK_EN
  #define RTE_USART4_SCK_PORT      RTE_FC4_SCK_PORT
  #define RTE_USART4_SCK_BIT       RTE_FC4_SCK_PIN
  #define RTE_USART4_SCK_FUNC      RTE_FC4_SCK_FUNC

  #define RTE_USART4_CTS_PIN_EN    RTE_FC4_CTS_SDA_SSEL0_EN
  #define RTE_USART4_CTS_PORT      RTE_FC4_CTS_SDA_SSEL0_PORT
  #define RTE_USART4_CTS_BIT       RTE_FC4_CTS_SDA_SSEL0_PIN
  #define RTE_USART4_CTS_FUNC      RTE_FC4_CTS_SDA_SSEL0_FUNC

  #define RTE_USART4_RTS_PIN_EN    RTE_FC4_RTS_SCL_SSEL1_EN
  #define RTE_USART4_RTS_PORT      RTE_FC4_RTS_SCL_SSEL1_PORT
  #define RTE_USART4_RTS_BIT       RTE_FC4_RTS_SCL_SSEL1_PIN
  #define RTE_USART4_RTS_FUNC      RTE_FC4_RTS_SCL_SSEL1_FUNC


#else
  #define RTE_USART4               0
#endif

// USART5
#if (RTE_FC5 != 0) && (RTE_FC5_PERSEL == 2)
  #define RTE_USART5               1

  #define RTE_USART5_TXD_PIN_EN    RTE_FC5_TXD_SCL_MISO_EN
  #define RTE_USART5_TXD_PORT      RTE_FC5_TXD_SCL_MISO_PORT
  #define RTE_USART5_TXD_BIT       RTE_FC5_TXD_SCL_MISO_PIN
  #define RTE_USART5_TXD_FUNC      RTE_FC5_TXD_SCL_MISO_FUNC

  #define RTE_USART5_RXD_PIN_EN    RTE_FC5_RXD_SDA_MOSI_EN
  #define RTE_USART5_RXD_PORT      RTE_FC5_RXD_SDA_MOSI_PORT
  #define RTE_USART5_RXD_BIT       RTE_FC5_RXD_SDA_MOSI_PIN
  #define RTE_USART5_RXD_FUNC      RTE_FC5_RXD_SDA_MOSI_FUNC

  #define RTE_USART5_SCK_PIN_EN    RTE_FC5_SCK_EN
  #define RTE_USART5_SCK_PORT      RTE_FC5_SCK_PORT
  #define RTE_USART5_SCK_BIT       RTE_FC5_SCK_PIN
  #define RTE_USART5_SCK_FUNC      RTE_FC5_SCK_FUNC

  #define RTE_USART5_CTS_PIN_EN    RTE_FC5_CTS_SDA_SSEL0_EN
  #define RTE_USART5_CTS_PORT      RTE_FC5_CTS_SDA_SSEL0_PORT
  #define RTE_USART5_CTS_BIT       RTE_FC5_CTS_SDA_SSEL0_PIN
  #define RTE_USART5_CTS_FUNC      RTE_FC5_CTS_SDA_SSEL0_FUNC

  #define RTE_USART5_RTS_PIN_EN    RTE_FC5_RTS_SCL_SSEL1_EN
  #define RTE_USART5_RTS_PORT      RTE_FC5_RTS_SCL_SSEL1_PORT
  #define RTE_USART5_RTS_BIT       RTE_FC5_RTS_SCL_SSEL1_PIN
  #define RTE_USART5_RTS_FUNC      RTE_FC5_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART5               0
#endif

// USART6
#if (RTE_FC6 != 0) && (RTE_FC6_PERSEL == 2)
  #define RTE_USART6               1

  #define RTE_USART6_TXD_PIN_EN    RTE_FC6_TXD_SCL_MISO_EN
  #define RTE_USART6_TXD_PORT      RTE_FC6_TXD_SCL_MISO_PORT
  #define RTE_USART6_TXD_BIT       RTE_FC6_TXD_SCL_MISO_PIN
  #define RTE_USART6_TXD_FUNC      RTE_FC6_TXD_SCL_MISO_FUNC

  #define RTE_USART6_RXD_PIN_EN    RTE_FC6_RXD_SDA_MOSI_EN
  #define RTE_USART6_RXD_PORT      RTE_FC6_RXD_SDA_MOSI_PORT
  #define RTE_USART6_RXD_BIT       RTE_FC6_RXD_SDA_MOSI_PIN
  #define RTE_USART6_RXD_FUNC      RTE_FC6_RXD_SDA_MOSI_FUNC

  #define RTE_USART6_SCK_PIN_EN    RTE_FC6_SCK_EN
  #define RTE_USART6_SCK_PORT      RTE_FC6_SCK_PORT
  #define RTE_USART6_SCK_BIT       RTE_FC6_SCK_PIN
  #define RTE_USART6_SCK_FUNC      RTE_FC6_SCK_FUNC

  #define RTE_USART6_CTS_PIN_EN    RTE_FC6_CTS_SDA_SSEL0_EN
  #define RTE_USART6_CTS_PORT      RTE_FC6_CTS_SDA_SSEL0_PORT
  #define RTE_USART6_CTS_BIT       RTE_FC6_CTS_SDA_SSEL0_PIN
  #define RTE_USART6_CTS_FUNC      RTE_FC6_CTS_SDA_SSEL0_FUNC

  #define RTE_USART6_RTS_PIN_EN    RTE_FC6_RTS_SCL_SSEL1_EN
  #define RTE_USART6_RTS_PORT      RTE_FC6_RTS_SCL_SSEL1_PORT
  #define RTE_USART6_RTS_BIT       RTE_FC6_RTS_SCL_SSEL1_PIN
  #define RTE_USART6_RTS_FUNC      RTE_FC6_RTS_SCL_SSEL1_FUNC

#else
  #define RTE_USART6               0
#endif

// USART7
#if (RTE_FC7 != 0) && (RTE_FC7_PERSEL == 2)
  #define RTE_USART7               1

  #define RTE_USART7_TXD_PIN_EN    RTE_FC7_TXD_SCL_MISO_EN
  #define RTE_USART7_TXD_PORT      RTE_FC7_TXD_SCL_MISO_PORT
  #define RTE_USART7_TXD_BIT       RTE_FC7_TXD_SCL_MISO_PIN
  #define RTE_USART7_TXD_FUNC      RTE_FC7_TXD_SCL_MISO_FUNC

  #define RTE_USART7_RXD_PIN_EN    RTE_FC7_RXD_SDA_MOSI_EN
  #define RTE_USART7_RXD_PORT      RTE_FC7_RXD_SDA_MOSI_PORT
  #define RTE_USART7_RXD_BIT       RTE_FC7_RXD_SDA_MOSI_PIN
  #define RTE_USART7_RXD_FUNC      RTE_FC7_RXD_SDA_MOSI_FUNC

  #define RTE_USART7_SCK_PIN_EN    RTE_FC7_SCK_EN
  #define RTE_USART7_SCK_PORT      RTE_FC7_SCK_PORT
  #define RTE_USART7_SCK_BIT       RTE_FC7_SCK_PIN
  #define RTE_USART7_SCK_FUNC      RTE_FC7_SCK_FUNC

  #define RTE_USART7_CTS_PIN_EN    RTE_FC7_CTS_SDA_SSEL0_EN
  #define RTE_USART7_CTS_PORT      RTE_FC7_CTS_SDA_SSEL0_PORT
  #define RTE_USART7_CTS_BIT       RTE_FC7_CTS_SDA_SSEL0_PIN
  #define RTE_USART7_CTS_FUNC      RTE_FC7_CTS_SDA_SSEL0_FUNC

  #define RTE_USART7_RTS_PIN_EN    RTE_FC7_RTS_SCL_SSEL1_EN
  #define RTE_USART7_RTS_PORT      RTE_FC7_RTS_SCL_SSEL1_PORT
  #define RTE_USART7_RTS_BIT       RTE_FC7_RTS_SCL_SSEL1_PIN
  #define RTE_USART7_RTS_FUNC      RTE_FC7_RTS_SCL_SSEL1_FUNC


#else
  #define RTE_USART7               0
#endif

// USART instances
#define USART0_FLEXCOMM          0
#define USART1_FLEXCOMM          1
#define USART2_FLEXCOMM          2
#define USART3_FLEXCOMM          3
#define USART4_FLEXCOMM          4
#define USART5_FLEXCOMM          5
#define USART6_FLEXCOMM          6
#define USART7_FLEXCOMM          7

// SYSCON clock defines
#define USART0_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM0
#define USART1_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM1
#define USART2_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM2
#define USART3_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM3
#define USART4_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM4
#define USART5_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM5
#define USART6_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM6
#define USART7_SYSCON_CLOCK      SYSCON_CLOCK_FLEXCOMM7

// Reset defines
#define USART0_RESET             RESET_FLEXCOMM0
#define USART1_RESET             RESET_FLEXCOMM1
#define USART2_RESET             RESET_FLEXCOMM2
#define USART3_RESET             RESET_FLEXCOMM3
#define USART4_RESET             RESET_FLEXCOMM4
#define USART5_RESET             RESET_FLEXCOMM5
#define USART6_RESET             RESET_FLEXCOMM6
#define USART7_RESET             RESET_FLEXCOMM7

// DMA Enable defines
#define USART0_TX_DMA_EN         RTE_FC0_DMA_TX_I2CM
#define USART0_RX_DMA_EN         RTE_FC0_DMA_RX_I2CS
#define USART1_TX_DMA_EN         RTE_FC1_DMA_TX_I2CM
#define USART1_RX_DMA_EN         RTE_FC1_DMA_RX_I2CS
#define USART2_TX_DMA_EN         RTE_FC2_DMA_TX_I2CM
#define USART2_RX_DMA_EN         RTE_FC2_DMA_RX_I2CS
#define USART3_TX_DMA_EN         RTE_FC3_DMA_TX_I2CM
#define USART3_RX_DMA_EN         RTE_FC3_DMA_RX_I2CS
#define USART4_TX_DMA_EN         RTE_FC4_DMA_TX_I2CM
#define USART4_RX_DMA_EN         RTE_FC4_DMA_RX_I2CS
#define USART5_TX_DMA_EN         RTE_FC5_DMA_TX_I2CM
#define USART5_RX_DMA_EN         RTE_FC5_DMA_RX_I2CS
#define USART6_TX_DMA_EN         RTE_FC6_DMA_TX_I2CM
#define USART6_RX_DMA_EN         RTE_FC6_DMA_RX_I2CS
#define USART7_TX_DMA_EN         RTE_FC7_DMA_TX_I2CM
#define USART7_RX_DMA_EN         RTE_FC7_DMA_RX_I2CS

// DMA Channel defines
#define USART0_TX_DMAREQ         DMAREQ_FLEXCOMM0_TX
#define USART0_RX_DMAREQ         DMAREQ_FLEXCOMM0_RX
#define USART1_TX_DMAREQ         DMAREQ_FLEXCOMM1_TX
#define USART1_RX_DMAREQ         DMAREQ_FLEXCOMM1_RX
#define USART2_TX_DMAREQ         DMAREQ_FLEXCOMM2_TX
#define USART2_RX_DMAREQ         DMAREQ_FLEXCOMM2_RX
#define USART3_TX_DMAREQ         DMAREQ_FLEXCOMM3_TX
#define USART3_RX_DMAREQ         DMAREQ_FLEXCOMM3_RX
#define USART4_TX_DMAREQ         DMAREQ_FLEXCOMM4_TX
#define USART4_RX_DMAREQ         DMAREQ_FLEXCOMM4_RX
#define USART5_TX_DMAREQ         DMAREQ_FLEXCOMM5_TX
#define USART5_RX_DMAREQ         DMAREQ_FLEXCOMM5_RX
#define USART6_TX_DMAREQ         DMAREQ_FLEXCOMM6_TX
#define USART6_RX_DMAREQ         DMAREQ_FLEXCOMM6_RX
#define USART7_TX_DMAREQ         DMAREQ_FLEXCOMM7_TX
#define USART7_RX_DMAREQ         DMAREQ_FLEXCOMM7_RX




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

// DMA
#define USART_TX_DMA_USED  (((RTE_USART0 != 0) && (USART0_TX_DMA_EN != 0)) || \
                            ((RTE_USART1 != 0) && (USART1_TX_DMA_EN != 0)) || \
                            ((RTE_USART2 != 0) && (USART2_TX_DMA_EN != 0)) || \
                            ((RTE_USART3 != 0) && (USART3_TX_DMA_EN != 0)) || \
                            ((RTE_USART4 != 0) && (USART4_TX_DMA_EN != 0)) || \
                            ((RTE_USART5 != 0) && (USART5_TX_DMA_EN != 0)) || \
                            ((RTE_USART6 != 0) && (USART6_TX_DMA_EN != 0)) || \
                            ((RTE_USART7 != 0) && (USART7_TX_DMA_EN != 0)))

// DMA
#define USART_RX_DMA_USED  (((RTE_USART0 != 0) && (USART0_RX_DMA_EN != 0)) || \
                            ((RTE_USART1 != 0) && (USART1_RX_DMA_EN != 0)) || \
                            ((RTE_USART2 != 0) && (USART2_RX_DMA_EN != 0)) || \
                            ((RTE_USART3 != 0) && (USART3_RX_DMA_EN != 0)) || \
                            ((RTE_USART4 != 0) && (USART4_RX_DMA_EN != 0)) || \
                            ((RTE_USART5 != 0) && (USART5_RX_DMA_EN != 0)) || \
                            ((RTE_USART6 != 0) && (USART6_RX_DMA_EN != 0)) || \
                            ((RTE_USART7 != 0) && (USART7_RX_DMA_EN != 0)))

#define USART_DMA_USED      (USART_TX_DMA_USED | USART_RX_DMA_USED)


// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
  uint32_t            rx_num;            // Total number of data to be received
  uint32_t            tx_num;            // Total number of data to be send
  uint8_t  * volatile rx_buf;            // Pointer to in data buffer
  uint8_t  * volatile tx_buf;            // Pointer to out data buffer
  uint32_t   volatile rx_cnt;            // Number of data received
  uint32_t   volatile tx_cnt;            // Number of data sent
#if USART_RX_DMA_USED
  uint32_t   volatile rx_num_pending;    // Number of data pending to receive (DMA)
#endif
#if USART_TX_DMA_USED
  uint32_t   volatile tx_num_pending;    // Number of data pending to transmit (DMA)
#endif
  uint8_t             tx_def_val;        // Transmit default value (used in USART_SYNC_MASTER_MODE_RX)
  uint8_t             rx_dump_val;       // Receive dump value (used in USART_SYNC_MASTER_MODE_TX)
  uint8_t    volatile send_active;       // Send active flag
  uint8_t             sync_mode;         // Synchronous mode
  uint8_t             receiver_enabled;  // Receiver enable flag
#if USART_RX_DMA_USED
  uint8_t    volatile rx_dma_int_flag;   // Receive DMA interrupt flag
  uint32_t            rx_dest_inc;       // Receive destination increment
#endif
#if USART_TX_DMA_USED
  uint8_t    volatile tx_dma_int_flag;   // Transmit DMA interrupt flag
  uint32_t            tx_src_inc;        // Transmit source increment
#endif
} USART_TRANSFER_INFO;

typedef volatile struct _USART_STATUS {
  uint8_t  tx_underflow;                 // Transmit data underflow detected (cleared on start of next send operation)
  uint8_t  rx_busy;                      // Receiver busy flag
  uint8_t  rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t  rx_break;                     // Break detected on receive (cleared on start of next receive operation)
  uint8_t  rx_framing_error;             // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t  rx_parity_error;              // Parity error detected on receive (cleared on start of next receive operation)
} USART_STATUS;

// USART Information (Run-Time)
typedef struct _USART_INFO {
  ARM_USART_SignalEvent_t cb_event;      // Event callback
  USART_STATUS            status;        // USART status flags
  USART_TRANSFER_INFO     xfer;          // Transfer information
  uint8_t                 mode;          // USART mode
  uint8_t                 flags;         // USART driver flags
  uint32_t                baudrate;      // Baudrate
} USART_INFO;

// USART DMA
typedef const struct _USART_DMA {
  DMA_CHID_T              chid;          // DMA channel ID
  DMA_CALLBACK_T          callback;      // DMA channel callback
} USART_DMA;

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
  ARM_USART_CAPABILITIES      capabilities;// Capabilities
  LPC_USART_T                *reg;         // Pointer to USART peripheral
  USART_PINS                  pins;        // USART pins configuration
  USART_DMA                  *dma_tx;      // DMA transmit channel
  USART_DMA                  *dma_rx;      // DMA receive channel
  CHIP_SYSCON_CLOCK_T         clk_msk;     // Clock mask
  CHIP_SYSCON_PERIPH_RESET_T  reset_msk;   // USART Reset
  uint8_t                     oversample;  // Oversample
  uint8_t                     txlvl;       // Transmit trigger level
  uint8_t                     rxlvl;       // Receive trigger level
  IRQn_Type                   irq_num;     // USART IRQ Number
  USART_INFO                 *info;        // Run-Time Information
} const USART_RESOURCES;

#endif /* __USART_LPC5411x_H */
