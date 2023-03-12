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
 * Project:      I2C Driver Definitions for NXP LPC5411x
 * -------------------------------------------------------------------------- */

#ifndef __I2C_LPC5411X_H
#define __I2C_LPC5411X_H

#include <string.h>

#include "chip.h"
#include "Driver_I2C.h"

#include "RTE_Components.h"
#include "RTE_Device.h"

#if ((defined(RTE_Drivers_I2C0) || \
      defined(RTE_Drivers_I2C1) || \
      defined(RTE_Drivers_I2C2) || \
      defined(RTE_Drivers_I2C3) || \
      defined(RTE_Drivers_I2C4) || \
      defined(RTE_Drivers_I2C5) || \
      defined(RTE_Drivers_I2C6) || \
      defined(RTE_Drivers_I2C7))   \
     && ((RTE_FC0 == 0) || (RTE_FC0_PERSEL != 0)) \
     && ((RTE_FC1 == 0) || (RTE_FC1_PERSEL != 0)) \
     && ((RTE_FC2 == 0) || (RTE_FC2_PERSEL != 0)) \
     && ((RTE_FC3 == 0) || (RTE_FC3_PERSEL != 0)) \
     && ((RTE_FC4 == 0) || (RTE_FC4_PERSEL != 0)) \
     && ((RTE_FC5 == 0) || (RTE_FC5_PERSEL != 0)) \
     && ((RTE_FC6 == 0) || (RTE_FC6_PERSEL != 0)) \
     && ((RTE_FC7 == 0) || (RTE_FC7_PERSEL != 0)))

#error "Flexcomm Interface not configured for I2C in RTE_Device.h!"
#endif

/* Enable I2C instances */
#if (RTE_FC0 != 0) && (RTE_FC0_PERSEL == 0) /* I2C0 */
  #define RTE_I2C0               1

  #if (RTE_FC0_RXD_SDA_MOSI_EN == 0) && (RTE_FC0_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C0 SDA (FC0_RXD_SDA_MOSI or FC0_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC0_RXD_SDA_MOSI_EN != 0) && (RTE_FC0_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C0 SDA (FC0_RXD_SDA_MOSI or FC0_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC0_RXD_SDA_MOSI_EN != 0)
    #define I2C0_SDA_PORT        RTE_FC0_RXD_SDA_MOSI_PORT
    #define I2C0_SDA_PIN         RTE_FC0_RXD_SDA_MOSI_PIN
    #define I2C0_SDA_FUNC        RTE_FC0_RXD_SDA_MOSI_FUNC
  #else
    #define I2C0_SDA_PORT        RTE_FC0_CTS_SDA_SSEL0_PORT
    #define I2C0_SDA_PIN         RTE_FC0_CTS_SDA_SSEL0_PIN
    #define I2C0_SDA_FUNC        RTE_FC0_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC0_TXD_SCL_MISO_EN == 0) && (RTE_FC0_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C0 SCL (FC0_TXD_SCL_MISO or FC0_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC0_TXD_SCL_MISO_EN != 0) && (RTE_FC0_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C0 SCL (FC0_TXD_SCL_MISO or FC0_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC0_TXD_SCL_MISO_EN != 0)
    #define I2C0_SCL_PORT        RTE_FC0_TXD_SCL_MISO_PORT
    #define I2C0_SCL_PIN         RTE_FC0_TXD_SCL_MISO_PIN
    #define I2C0_SCL_FUNC        RTE_FC0_TXD_SCL_MISO_FUNC
  #else
    #define I2C0_SCL_PORT        RTE_FC0_RTS_SCL_SSEL1_PORT
    #define I2C0_SCL_PIN         RTE_FC0_RTS_SCL_SSEL1_PIN
    #define I2C0_SCL_FUNC        RTE_FC0_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC0_DMA_RX_I2CS != 0)
    #define I2C0_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC0_DMA_TX_I2CM != 0)
    #define I2C0_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC1 != 0) && (RTE_FC1_PERSEL == 0) /* I2C1 */
  #define RTE_I2C1               1

  #if (RTE_FC1_RXD_SDA_MOSI_EN == 0) && (RTE_FC1_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C1 SDA (FC1_RXD_SDA_MOSI or FC1_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC1_RXD_SDA_MOSI_EN != 0) && (RTE_FC1_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C1 SDA (FC1_RXD_SDA_MOSI or FC1_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC1_RXD_SDA_MOSI_EN != 0)
    #define I2C1_SDA_PORT        RTE_FC1_RXD_SDA_MOSI_PORT
    #define I2C1_SDA_PIN         RTE_FC1_RXD_SDA_MOSI_PIN
    #define I2C1_SDA_FUNC        RTE_FC1_RXD_SDA_MOSI_FUNC
  #else
    #define I2C1_SDA_PORT        RTE_FC1_CTS_SDA_SSEL0_PORT
    #define I2C1_SDA_PIN         RTE_FC1_CTS_SDA_SSEL0_PIN
    #define I2C1_SDA_FUNC        RTE_FC1_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC1_TXD_SCL_MISO_EN == 0) && (RTE_FC1_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C1 SCL (FC1_TXD_SCL_MISO or FC1_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC1_TXD_SCL_MISO_EN != 0) && (RTE_FC1_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C1 SCL (FC1_TXD_SCL_MISO or FC0_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC1_TXD_SCL_MISO_EN != 0)
    #define I2C1_SCL_PORT        RTE_FC1_TXD_SCL_MISO_PORT
    #define I2C1_SCL_PIN         RTE_FC1_TXD_SCL_MISO_PIN
    #define I2C1_SCL_FUNC        RTE_FC1_TXD_SCL_MISO_FUNC
  #else
    #define I2C1_SCL_PORT        RTE_FC1_RTS_SCL_SSEL1_PORT
    #define I2C1_SCL_PIN         RTE_FC1_RTS_SCL_SSEL1_PIN
    #define I2C1_SCL_FUNC        RTE_FC1_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC1_DMA_RX_I2CS != 0)
    #define I2C1_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC1_DMA_TX_I2CM != 0)
    #define I2C1_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC2 != 0) && (RTE_FC2_PERSEL == 0) /* I2C2 */
  #define RTE_I2C2               1

  #if (RTE_FC2_RXD_SDA_MOSI_EN == 0) && (RTE_FC2_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C2 SDA (FC2_RXD_SDA_MOSI or FC2_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC2_RXD_SDA_MOSI_EN != 0) && (RTE_FC2_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C2 SDA (FC2_RXD_SDA_MOSI or FC2_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC2_RXD_SDA_MOSI_EN != 0)
    #define I2C2_SDA_PORT        RTE_FC2_RXD_SDA_MOSI_PORT
    #define I2C2_SDA_PIN         RTE_FC2_RXD_SDA_MOSI_PIN
    #define I2C2_SDA_FUNC        RTE_FC2_RXD_SDA_MOSI_FUNC
  #else
    #define I2C2_SDA_PORT        RTE_FC2_CTS_SDA_SSEL0_PORT
    #define I2C2_SDA_PIN         RTE_FC2_CTS_SDA_SSEL0_PIN
    #define I2C2_SDA_FUNC        RTE_FC2_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC2_TXD_SCL_MISO_EN == 0) && (RTE_FC2_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C2 SCL (FC2_TXD_SCL_MISO or FC2_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC2_TXD_SCL_MISO_EN != 0) && (RTE_FC2_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C2 SCL (FC2_TXD_SCL_MISO or FC2_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC2_TXD_SCL_MISO_EN != 0)
    #define I2C2_SCL_PORT        RTE_FC2_TXD_SCL_MISO_PORT
    #define I2C2_SCL_PIN         RTE_FC2_TXD_SCL_MISO_PIN
    #define I2C2_SCL_FUNC        RTE_FC2_TXD_SCL_MISO_FUNC
  #else
    #define I2C2_SCL_PORT        RTE_FC2_RTS_SCL_SSEL1_PORT
    #define I2C2_SCL_PIN         RTE_FC2_RTS_SCL_SSEL1_PIN
    #define I2C2_SCL_FUNC        RTE_FC2_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC2_DMA_RX_I2CS != 0)
    #define I2C2_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC2_DMA_TX_I2CM != 0)
    #define I2C2_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC3 != 0) && (RTE_FC3_PERSEL == 0) /* I2C3 */
  #define RTE_I2C3               1

  #if (RTE_FC3_RXD_SDA_MOSI_EN == 0) && (RTE_FC3_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C3 SDA (FC3_RXD_SDA_MOSI or FC3_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC3_RXD_SDA_MOSI_EN != 0) && (RTE_FC3_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C3 SDA (FC3_RXD_SDA_MOSI or FC3_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC3_RXD_SDA_MOSI_EN != 0)
    #define I2C3_SDA_PORT        RTE_FC3_RXD_SDA_MOSI_PORT
    #define I2C3_SDA_PIN         RTE_FC3_RXD_SDA_MOSI_PIN
    #define I2C3_SDA_FUNC        RTE_FC3_RXD_SDA_MOSI_FUNC
  #else
    #define I2C3_SDA_PORT        RTE_FC3_CTS_SDA_SSEL0_PORT
    #define I2C3_SDA_PIN         RTE_FC3_CTS_SDA_SSEL0_PIN
    #define I2C3_SDA_FUNC        RTE_FC3_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC3_TXD_SCL_MISO_EN == 0) && (RTE_FC3_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C3 SCL (FC3_TXD_SCL_MISO or FC3_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC3_TXD_SCL_MISO_EN != 0) && (RTE_FC3_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C3 SCL (FC3_TXD_SCL_MISO or FC3_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC3_TXD_SCL_MISO_EN != 0)
    #define I2C3_SCL_PORT        RTE_FC3_TXD_SCL_MISO_PORT
    #define I2C3_SCL_PIN         RTE_FC3_TXD_SCL_MISO_PIN
    #define I2C3_SCL_FUNC        RTE_FC3_TXD_SCL_MISO_FUNC
  #else
    #define I2C3_SCL_PORT        RTE_FC3_RTS_SCL_SSEL1_PORT
    #define I2C3_SCL_PIN         RTE_FC3_RTS_SCL_SSEL1_PIN
    #define I2C3_SCL_FUNC        RTE_FC3_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC3_DMA_RX_I2CS != 0)
    #define I2C3_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC3_DMA_TX_I2CM != 0)
    #define I2C3_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC4 != 0) && (RTE_FC4_PERSEL == 0) /* I2C4 */
  #define RTE_I2C4               1

  #if (RTE_FC4_RXD_SDA_MOSI_EN == 0) && (RTE_FC4_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C4 SDA (FC4_RXD_SDA_MOSI or FC4_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC4_RXD_SDA_MOSI_EN != 0) && (RTE_FC4_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C4 SDA (FC4_RXD_SDA_MOSI or FC4_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC4_RXD_SDA_MOSI_EN != 0)
    #define I2C4_SDA_PORT        RTE_FC4_RXD_SDA_MOSI_PORT
    #define I2C4_SDA_PIN         RTE_FC4_RXD_SDA_MOSI_PIN
    #define I2C4_SDA_FUNC        RTE_FC4_RXD_SDA_MOSI_FUNC
  #else
    #define I2C4_SDA_PORT        RTE_FC4_CTS_SDA_SSEL0_PORT
    #define I2C4_SDA_PIN         RTE_FC4_CTS_SDA_SSEL0_PIN
    #define I2C4_SDA_FUNC        RTE_FC4_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC4_TXD_SCL_MISO_EN == 0) && (RTE_FC4_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C4 SCL (FC4_TXD_SCL_MISO or FC4_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC4_TXD_SCL_MISO_EN != 0) && (RTE_FC4_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C4 SCL (FC4_TXD_SCL_MISO or FC4_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC4_TXD_SCL_MISO_EN != 0)
    #define I2C4_SCL_PORT        RTE_FC4_TXD_SCL_MISO_PORT
    #define I2C4_SCL_PIN         RTE_FC4_TXD_SCL_MISO_PIN
    #define I2C4_SCL_FUNC        RTE_FC4_TXD_SCL_MISO_FUNC
  #else
    #define I2C4_SCL_PORT        RTE_FC4_RTS_SCL_SSEL1_PORT
    #define I2C4_SCL_PIN         RTE_FC4_RTS_SCL_SSEL1_PIN
    #define I2C4_SCL_FUNC        RTE_FC4_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC4_DMA_RX_I2CS != 0)
    #define I2C4_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC4_DMA_TX_I2CM != 0)
    #define I2C4_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC5 != 0) && (RTE_FC5_PERSEL == 0) /* I2C5 */
  #define RTE_I2C5               1

  #if (RTE_FC5_RXD_SDA_MOSI_EN == 0) && (RTE_FC5_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C5 SDA (FC5_RXD_SDA_MOSI or FC5_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC5_RXD_SDA_MOSI_EN != 0) && (RTE_FC5_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C5 SDA (FC5_RXD_SDA_MOSI or FC5_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC5_RXD_SDA_MOSI_EN != 0)
    #define I2C5_SDA_PORT        RTE_FC5_RXD_SDA_MOSI_PORT
    #define I2C5_SDA_PIN         RTE_FC5_RXD_SDA_MOSI_PIN
    #define I2C5_SDA_FUNC        RTE_FC5_RXD_SDA_MOSI_FUNC
  #else
    #define I2C5_SDA_PORT        RTE_FC5_CTS_SDA_SSEL0_PORT
    #define I2C5_SDA_PIN         RTE_FC5_CTS_SDA_SSEL0_PIN
    #define I2C5_SDA_FUNC        RTE_FC5_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC5_TXD_SCL_MISO_EN == 0) && (RTE_FC5_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C5 SCL (FC5_TXD_SCL_MISO or FC5_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC5_TXD_SCL_MISO_EN != 0) && (RTE_FC5_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C5 SCL (FC5_TXD_SCL_MISO or FC5_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC5_TXD_SCL_MISO_EN != 0)
    #define I2C5_SCL_PORT        RTE_FC5_TXD_SCL_MISO_PORT
    #define I2C5_SCL_PIN         RTE_FC5_TXD_SCL_MISO_PIN
    #define I2C5_SCL_FUNC        RTE_FC5_TXD_SCL_MISO_FUNC
  #else
    #define I2C5_SCL_PORT        RTE_FC5_RTS_SCL_SSEL1_PORT
    #define I2C5_SCL_PIN         RTE_FC5_RTS_SCL_SSEL1_PIN
    #define I2C5_SCL_FUNC        RTE_FC5_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC5_DMA_RX_I2CS != 0)
    #define I2C5_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC5_DMA_TX_I2CM != 0)
    #define I2C5_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC6 != 0) && (RTE_FC6_PERSEL == 0) /* I2C6 */
  #define RTE_I2C6               1

  #if (RTE_FC6_RXD_SDA_MOSI_EN == 0) && (RTE_FC6_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C6 SDA (FC6_RXD_SDA_MOSI or FC6_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC6_RXD_SDA_MOSI_EN != 0) && (RTE_FC6_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C6 SDA (FC6_RXD_SDA_MOSI or FC6_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC6_RXD_SDA_MOSI_EN != 0)
    #define I2C6_SDA_PORT        RTE_FC6_RXD_SDA_MOSI_PORT
    #define I2C6_SDA_PIN         RTE_FC6_RXD_SDA_MOSI_PIN
    #define I2C6_SDA_FUNC        RTE_FC6_RXD_SDA_MOSI_FUNC
  #else
    #define I2C6_SDA_PORT        RTE_FC6_CTS_SDA_SSEL0_PORT
    #define I2C6_SDA_PIN         RTE_FC6_CTS_SDA_SSEL0_PIN
    #define I2C6_SDA_FUNC        RTE_FC6_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC6_TXD_SCL_MISO_EN == 0) && (RTE_FC6_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C6 SCL (FC6_TXD_SCL_MISO or FC6_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC6_TXD_SCL_MISO_EN != 0) && (RTE_FC6_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C6 SCL (FC6_TXD_SCL_MISO or FC6_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC6_TXD_SCL_MISO_EN != 0)
    #define I2C6_SCL_PORT        RTE_FC6_TXD_SCL_MISO_PORT
    #define I2C6_SCL_PIN         RTE_FC6_TXD_SCL_MISO_PIN
    #define I2C6_SCL_FUNC        RTE_FC6_TXD_SCL_MISO_FUNC
  #else
    #define I2C6_SCL_PORT        RTE_FC6_RTS_SCL_SSEL1_PORT
    #define I2C6_SCL_PIN         RTE_FC6_RTS_SCL_SSEL1_PIN
    #define I2C6_SCL_FUNC        RTE_FC6_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC6_DMA_RX_I2CS != 0)
    #define I2C6_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC6_DMA_TX_I2CM != 0)
    #define I2C6_DMA_MASTER_EN   1
  #endif
#endif

#if (RTE_FC7 != 0) && (RTE_FC7_PERSEL == 0) /* I2C7 */
  #define RTE_I2C7               1

  #if (RTE_FC7_RXD_SDA_MOSI_EN == 0) && (RTE_FC7_CTS_SDA_SSEL0_EN == 0)
    #error "Select pin for I2C7 SDA (FC7_RXD_SDA_MOSI or FC7_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC7_RXD_SDA_MOSI_EN != 0) && (RTE_FC7_CTS_SDA_SSEL0_EN != 0)
    #error "Select only one pin for I2C7 SDA (FC7_RXD_SDA_MOSI or FC7_CTS_SDA_SSEL0) in RTE_Device.h!"
  #endif

  #if (RTE_FC7_RXD_SDA_MOSI_EN != 0)
    #define I2C7_SDA_PORT        RTE_FC7_RXD_SDA_MOSI_PORT
    #define I2C7_SDA_PIN         RTE_FC7_RXD_SDA_MOSI_PIN
    #define I2C7_SDA_FUNC        RTE_FC7_RXD_SDA_MOSI_FUNC
  #else
    #define I2C7_SDA_PORT        RTE_FC7_CTS_SDA_SSEL0_PORT
    #define I2C7_SDA_PIN         RTE_FC7_CTS_SDA_SSEL0_PIN
    #define I2C7_SDA_FUNC        RTE_FC7_CTS_SDA_SSEL0_FUNC
  #endif

  #if (RTE_FC7_TXD_SCL_MISO_EN == 0) && (RTE_FC7_RTS_SCL_SSEL1_EN == 0)
    #error "Select pin for I2C7 SCL (FC7_TXD_SCL_MISO or FC7_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  #if (RTE_FC7_TXD_SCL_MISO_EN != 0) && (RTE_FC7_RTS_SCL_SSEL1_EN != 0)
    #error "Select only one pin for I2C7 SCL (FC7_TXD_SCL_MISO or FC7_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif
  
  #if (RTE_FC7_TXD_SCL_MISO_EN != 0)
    #define I2C7_SCL_PORT        RTE_FC7_TXD_SCL_MISO_PORT
    #define I2C7_SCL_PIN         RTE_FC7_TXD_SCL_MISO_PIN
    #define I2C7_SCL_FUNC        RTE_FC7_TXD_SCL_MISO_FUNC
  #else
    #define I2C7_SCL_PORT        RTE_FC7_RTS_SCL_SSEL1_PORT
    #define I2C7_SCL_PIN         RTE_FC7_RTS_SCL_SSEL1_PIN
    #define I2C7_SCL_FUNC        RTE_FC7_RTS_SCL_SSEL1_FUNC
  #endif

  #if (RTE_FC7_DMA_RX_I2CS != 0)
    #define I2C7_DMA_SLAVE_EN    1
  #endif
  #if (RTE_FC7_DMA_TX_I2CM != 0)
    #define I2C7_DMA_MASTER_EN   1
  #endif
#endif

#if defined(I2C0_DMA_MASTER_EN) || defined(I2C1_DMA_MASTER_EN) || \
    defined(I2C2_DMA_MASTER_EN) || defined(I2C3_DMA_MASTER_EN) || \
    defined(I2C4_DMA_MASTER_EN) || defined(I2C5_DMA_MASTER_EN) || \
    defined(I2C6_DMA_MASTER_EN) || defined(I2C7_DMA_MASTER_EN)
  #define I2C_DMA_MASTER_EN      1
#endif

#if defined(I2C0_DMA_SLAVE_EN) || defined(I2C1_DMA_SLAVE_EN) || \
    defined(I2C2_DMA_SLAVE_EN) || defined(I2C3_DMA_SLAVE_EN) || \
    defined(I2C4_DMA_SLAVE_EN) || defined(I2C5_DMA_SLAVE_EN) || \
    defined(I2C6_DMA_SLAVE_EN) || defined(I2C7_DMA_SLAVE_EN)
  #define I2C_DMA_SLAVE_EN       1
#endif


/* I2C instances */
#define I2C0_FLEXCOMM            0
#define I2C1_FLEXCOMM            1
#define I2C2_FLEXCOMM            2
#define I2C3_FLEXCOMM            3
#define I2C4_FLEXCOMM            4
#define I2C5_FLEXCOMM            5
#define I2C6_FLEXCOMM            6
#define I2C7_FLEXCOMM            7

/* SYSCON clock defines */
#define I2C0_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM0
#define I2C1_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM1
#define I2C2_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM2
#define I2C3_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM3
#define I2C4_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM4
#define I2C5_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM5
#define I2C6_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM6
#define I2C7_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM7

/* IOCON settings for I2C pins */
#define I2C_PIN_IOCON_GPIO      (IOCON_FUNC0      | \
                                 IOCON_GPIO_MODE  | \
                                 IOCON_DIGITAL_EN | \
                                 IOCON_INPFILT_OFF)

#define I2C_PIN_IOCON_STANDARD  (IOCON_FUNC1      | \
                                 IOCON_DIGITAL_EN | \
                                 IOCON_INPFILT_OFF)

#define I2C_PIN_IOCON_FAST      I2C_PIN_IOCON_STANDARD

#define I2C_PIN_IOCON_FAST_PLUS (IOCON_FUNC1       | \
                                 IOCON_DIGITAL_EN  | \
                                 IOCON_INPFILT_OFF | \
                                 (1UL <<  9)       | \
                                 (1UL << 10)       )

#define I2C_PIN_IOCON_HIGH      I2C_PIN_IOCON_FAST_PLUS

/* I2C interrupt mask */
#define I2C_MASTER_INTERRUPTS   (I2C_INTENSET_MSTPENDING  | \
                                 I2C_INTENSET_MSTRARBLOSS | \
                                 I2C_INTENSET_MSTSTSTPERR)

#define I2C_SLAVE_INTERRUPTS    (I2C_INTENSET_SLVPENDING | \
                                 I2C_INTENSET_SLVNOTSTR  | \
                                 I2C_INTENSET_SLVDESEL)

/* I2C Driver state flags */
#define I2C_INIT       (uint8_t)(1U << 0)   // Driver initialized
#define I2C_POWER      (uint8_t)(1U << 1)   // Driver power on
#define I2C_SLAVE10BIT (uint8_t)(1U << 2)   // Slave uses 10-bit addressing

/* Transfer status flags */
#define XFER_CTRL_XPENDING  ((uint8_t)0x01) // Transfer pending
#define XFER_CTRL_ADDR_DONE ((uint8_t)0x02) // Addressing done

/* I2C Transfer Information (Run-Time) */
typedef struct I2C_XFER_INFO {
  uint8_t              *data;               // Data pointer
  uint32_t              num;                // Number of data to transfer
  uint32_t              cnt;                // Data transfer counter
  uint16_t              addr;               // Device address
  uint16_t              pend;               // Pending data transfers
  uint8_t               ctrl;               // Transfer control flags
} I2C_TRANSFER_INFO;

/* I2C Control Information */
typedef struct i2c_info {
  ARM_I2C_SignalEvent_t cb_event;           // Event callback
  ARM_I2C_STATUS        status;             // Status flags
  I2C_TRANSFER_INFO     xfer;
  uint8_t               flags;              // Control and state flags
} I2C_INFO;

/* I2C DMA Information */
typedef const struct i2c_dma {
  DMA_CHID_T     ch;                        // Channel ID
  DMA_CALLBACK_T callback;                  // Channel callback function
} I2C_DMA;

/* I2C Pin Description */
typedef struct i2c_pin {
  uint8_t port;                             // Port number
  uint8_t pin;                              // Pin number
  uint8_t func;                             // Pin function
} I2C_PIN;

/* I2C Resource Configuration */
typedef struct i2c_resources {
  LPC_I2C_T            *reg;                // I2C register interface
  IRQn_Type             irqn;               // I2C Event IRQ Number
  CHIP_SYSCON_CLOCK_T   clk_msk;            // APB clock control bit mask
  I2C_PIN               scl;                // I2C SCL pin
  I2C_PIN               sda;                // I2C SDA pin
  I2C_DMA              *dma_master;         // Master DMA info
  I2C_DMA              *dma_slave;          // Slave DMA info
  I2C_INFO             *info;               // Run-Time control information
} const I2C_RESOURCES;

#endif /* __I2C_LPC5411X_H */
