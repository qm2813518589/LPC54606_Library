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
 * Project:      SPI Driver Definitions for NXP LPC5411x
 * -------------------------------------------------------------------------- */

#ifndef __SPI_LPC5411X_H
#define __SPI_LPC5411X_H

#include <string.h>

#include "chip.h"
#include "Driver_SPI.h"

#include "RTE_Components.h"
#include "RTE_Device.h"

#if ((defined(RTE_Drivers_SPI0) || \
      defined(RTE_Drivers_SPI1) || \
      defined(RTE_Drivers_SPI2) || \
      defined(RTE_Drivers_SPI3) || \
      defined(RTE_Drivers_SPI4) || \
      defined(RTE_Drivers_SPI5) || \
      defined(RTE_Drivers_SPI6) || \
      defined(RTE_Drivers_SPI7))   \
     && ((RTE_FC0 == 0) || (RTE_FC0_PERSEL != 1)) \
     && ((RTE_FC1 == 0) || (RTE_FC1_PERSEL != 1)) \
     && ((RTE_FC2 == 0) || (RTE_FC2_PERSEL != 1)) \
     && ((RTE_FC3 == 0) || (RTE_FC3_PERSEL != 1)) \
     && ((RTE_FC4 == 0) || (RTE_FC4_PERSEL != 1)) \
     && ((RTE_FC5 == 0) || (RTE_FC5_PERSEL != 1)) \
     && ((RTE_FC6 == 0) || (RTE_FC6_PERSEL != 1)) \
     && ((RTE_FC7 == 0) || (RTE_FC7_PERSEL != 1)))

#error "Flexcomm Interface not configured for SPI in RTE_Device.h!"
#endif

/* Enable SPI instances */
#if (RTE_FC0 != 0) && (RTE_FC0_PERSEL == 1) /* SPI0 */
  #define RTE_SPI0               1

  #if (RTE_FC0_SCK_EN == 0)
    #error "Mandatory SPI0 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC0_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI0 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC0_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI0 MOSI pin is not configured in RTE_Device.h!"
  #endif

  #define SPI0_MOSI_PORT         RTE_FC0_RXD_SDA_MOSI_PORT
  #define SPI0_MOSI_PIN          RTE_FC0_RXD_SDA_MOSI_PIN
  #define SPI0_MOSI_FUNC         RTE_FC0_RXD_SDA_MOSI_FUNC

  #define SPI0_MISO_PORT         RTE_FC0_TXD_SCL_MISO_PORT
  #define SPI0_MISO_PIN          RTE_FC0_TXD_SCL_MISO_PIN
  #define SPI0_MISO_FUNC         RTE_FC0_TXD_SCL_MISO_FUNC

  #define SPI0_SCK_PORT          RTE_FC0_SCK_PORT
  #define SPI0_SCK_PIN           RTE_FC0_SCK_PIN
  #define SPI0_SCK_FUNC          RTE_FC0_SCK_FUNC

  #if ((RTE_FC0_CTS_SDA_SSEL0_EN +RTE_FC0_RTS_SCL_SSEL1_EN) > 1)
    #error "Select only one pin for SPI0 SSEL (FC0_CTS_SDA_SSEL0 or FC0_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif

  #if (RTE_FC0_CTS_SDA_SSEL0_EN != 0)
    #define SPI0_SSEL_PORT       RTE_FC0_CTS_SDA_SSEL0_PORT
    #define SPI0_SSEL_PIN        RTE_FC0_CTS_SDA_SSEL0_PIN
    #define SPI0_SSEL_FUNC       RTE_FC0_CTS_SDA_SSEL0_FUNC
    #define SPI0_SSEL_NUM        0
  #endif
  #if (RTE_FC0_RTS_SCL_SSEL1_EN != 0)
    #define SPI0_SSEL_PORT       RTE_FC0_RTS_SCL_SSEL1_PORT
    #define SPI0_SSEL_PIN        RTE_FC0_RTS_SCL_SSEL1_PIN
    #define SPI0_SSEL_FUNC       RTE_FC0_RTS_SCL_SSEL1_FUNC
    #define SPI0_SSEL_NUM        1
  #endif

  #if (RTE_FC0_DMA_RX_I2CS != 0) || (RTE_FC0_DMA_TX_I2CM != 0)
    #if (RTE_FC0_DMA_RX_I2CS != 0) && (RTE_FC0_DMA_TX_I2CM != 0)
      #define SPI0_DMA_EN        1
    #else
      #error "SPI0 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC1 != 0) && (RTE_FC1_PERSEL == 1) /* SPI1 */
  #define RTE_SPI1               1

  #if (RTE_FC1_SCK_EN == 0)
    #error "Mandatory SPI1 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC1_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI1 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC1_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI1 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI1_MOSI_PORT         RTE_FC1_RXD_SDA_MOSI_PORT
  #define SPI1_MOSI_PIN          RTE_FC1_RXD_SDA_MOSI_PIN
  #define SPI1_MOSI_FUNC         RTE_FC1_RXD_SDA_MOSI_FUNC

  #define SPI1_MISO_PORT         RTE_FC1_TXD_SCL_MISO_PORT
  #define SPI1_MISO_PIN          RTE_FC1_TXD_SCL_MISO_PIN
  #define SPI1_MISO_FUNC         RTE_FC1_TXD_SCL_MISO_FUNC

  #define SPI1_SCK_PORT          RTE_FC1_SCK_PORT
  #define SPI1_SCK_PIN           RTE_FC1_SCK_PIN
  #define SPI1_SCK_FUNC          RTE_FC1_SCK_FUNC

  #if ((RTE_FC1_CTS_SDA_SSEL0_EN + RTE_FC1_RTS_SCL_SSEL1_EN) > 1)
    #error "Select only one pin for SPI1 SSEL (FC1_CTS_SDA_SSEL0 or FC1_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif

  #if (RTE_FC1_CTS_SDA_SSEL0_EN != 0)
    #define SPI1_SSEL_PORT       RTE_FC1_CTS_SDA_SSEL0_PORT
    #define SPI1_SSEL_PIN        RTE_FC1_CTS_SDA_SSEL0_PIN
    #define SPI1_SSEL_FUNC       RTE_FC1_CTS_SDA_SSEL0_FUNC
    #define SPI1_SSEL_NUM        0
  #endif
  #if (RTE_FC1_RTS_SCL_SSEL1_EN != 0)
    #define SPI1_SSEL_PORT       RTE_FC1_RTS_SCL_SSEL1_PORT
    #define SPI1_SSEL_PIN        RTE_FC1_RTS_SCL_SSEL1_PIN
    #define SPI1_SSEL_FUNC       RTE_FC1_RTS_SCL_SSEL1_FUNC
    #define SPI1_SSEL_NUM        1
  #endif

  #if (RTE_FC1_DMA_RX_I2CS != 0) || (RTE_FC1_DMA_TX_I2CM != 0)
    #if (RTE_FC1_DMA_RX_I2CS != 0) && (RTE_FC1_DMA_TX_I2CM != 0)
      #define SPI1_DMA_EN        1
    #else
      #error "SPI1 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC2 != 0) && (RTE_FC2_PERSEL == 1) /* SPI2 */
  #define RTE_SPI2               1

  #if (RTE_FC2_SCK_EN == 0)
    #error "Mandatory SPI2 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC2_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI2 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC2_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI2 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI2_MOSI_PORT         RTE_FC2_RXD_SDA_MOSI_PORT
  #define SPI2_MOSI_PIN          RTE_FC2_RXD_SDA_MOSI_PIN
  #define SPI2_MOSI_FUNC         RTE_FC2_RXD_SDA_MOSI_FUNC

  #define SPI2_MISO_PORT         RTE_FC2_TXD_SCL_MISO_PORT
  #define SPI2_MISO_PIN          RTE_FC2_TXD_SCL_MISO_PIN
  #define SPI2_MISO_FUNC         RTE_FC2_TXD_SCL_MISO_FUNC

  #define SPI2_SCK_PORT          RTE_FC2_SCK_PORT
  #define SPI2_SCK_PIN           RTE_FC2_SCK_PIN
  #define SPI2_SCK_FUNC          RTE_FC2_SCK_FUNC

  #if ((RTE_FC2_CTS_SDA_SSEL0_EN + RTE_FC2_RTS_SCL_SSEL1_EN + RTE_FC2_SSEL2_EN + RTE_FC2_SSEL3_EN) > 1)
    #error "Select only one pin for SPI2 SSEL (FC2_CTS_SDA_SSEL0, FC2_RTS_SCL_SSEL1, FC2_SSEL2, FC2_SSEL3) in RTE_Device.h!"
  #endif

  #if (RTE_FC2_CTS_SDA_SSEL0_EN != 0)
    #define SPI2_SSEL_PORT       RTE_FC2_CTS_SDA_SSEL0_PORT
    #define SPI2_SSEL_PIN        RTE_FC2_CTS_SDA_SSEL0_PIN
    #define SPI2_SSEL_FUNC       RTE_FC2_CTS_SDA_SSEL0_FUNC
    #define SPI2_SSEL_NUM        0
  #endif
  #if (RTE_FC2_RTS_SCL_SSEL1_EN != 0)
    #define SPI2_SSEL_PORT       RTE_FC2_RTS_SCL_SSEL1_PORT
    #define SPI2_SSEL_PIN        RTE_FC2_RTS_SCL_SSEL1_PIN
    #define SPI2_SSEL_FUNC       RTE_FC2_RTS_SCL_SSEL1_FUNC
    #define SPI2_SSEL_NUM        1
  #endif

  #if (RTE_FC2_DMA_RX_I2CS != 0) || (RTE_FC2_DMA_TX_I2CM != 0)
    #if (RTE_FC2_DMA_RX_I2CS != 0) && (RTE_FC2_DMA_TX_I2CM != 0)
      #define SPI2_DMA_EN        1
    #else
      #error "SPI2 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC3 != 0) && (RTE_FC3_PERSEL == 1) /* SPI3 */
  #define RTE_SPI3               1

  #if (RTE_FC3_SCK_EN == 0)
    #error "Mandatory SPI3 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC3_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI3 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC3_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI3 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI3_MOSI_PORT         RTE_FC3_RXD_SDA_MOSI_PORT
  #define SPI3_MOSI_PIN          RTE_FC3_RXD_SDA_MOSI_PIN
  #define SPI3_MOSI_FUNC         RTE_FC3_RXD_SDA_MOSI_FUNC

  #define SPI3_MISO_PORT         RTE_FC3_TXD_SCL_MISO_PORT
  #define SPI3_MISO_PIN          RTE_FC3_TXD_SCL_MISO_PIN
  #define SPI3_MISO_FUNC         RTE_FC3_TXD_SCL_MISO_FUNC

  #define SPI3_SCK_PORT          RTE_FC3_SCK_PORT
  #define SPI3_SCK_PIN           RTE_FC3_SCK_PIN
  #define SPI3_SCK_FUNC          RTE_FC3_SCK_FUNC

  #if ((RTE_FC3_CTS_SDA_SSEL0_EN + RTE_FC3_RTS_SCL_SSEL1_EN + RTE_FC3_SSEL2_EN + RTE_FC3_SSEL3_EN) > 1)
    #error "Select only one pin for SPI3 SSEL (FC3_CTS_SDA_SSEL0, FC3_RTS_SCL_SSEL1, FC3_SSEL2, FC3_SSEL3) in RTE_Device.h!"
  #endif

  #if (RTE_FC3_CTS_SDA_SSEL0_EN != 0)
    #define SPI3_SSEL_PORT       RTE_FC3_CTS_SDA_SSEL0_PORT
    #define SPI3_SSEL_PIN        RTE_FC3_CTS_SDA_SSEL0_PIN
    #define SPI3_SSEL_FUNC       RTE_FC3_CTS_SDA_SSEL0_FUNC
    #define SPI3_SSEL_NUM        0
  #endif
  #if (RTE_FC3_RTS_SCL_SSEL1_EN != 0)
    #define SPI3_SSEL_PORT       RTE_FC3_RTS_SCL_SSEL1_PORT
    #define SPI3_SSEL_PIN        RTE_FC3_RTS_SCL_SSEL1_PIN
    #define SPI3_SSEL_FUNC       RTE_FC3_RTS_SCL_SSEL1_FUNC
    #define SPI3_SSEL_NUM        1
  #endif
  #if (RTE_FC3_SSEL2_EN != 0)
    #define SPI3_SSEL_PORT       RTE_FC3_SSEL2_PORT
    #define SPI3_SSEL_PIN        RTE_FC3_SSEL2_PIN
    #define SPI3_SSEL_FUNC       RTE_FC3_SSEL2_FUNC
    #define SPI3_SSEL_NUM        2
  #endif
  #if (RTE_FC3_SSEL3_EN != 0)
    #define SPI3_SSEL_PORT       RTE_FC3_SSEL3_PORT
    #define SPI3_SSEL_PIN        RTE_FC3_SSEL3_PIN
    #define SPI3_SSEL_FUNC       RTE_FC3_SSEL3_FUNC
    #define SPI3_SSEL_NUM        3
  #endif

  #if (RTE_FC3_DMA_RX_I2CS != 0) || (RTE_FC3_DMA_TX_I2CM != 0)
    #if (RTE_FC3_DMA_RX_I2CS != 0) && (RTE_FC3_DMA_TX_I2CM != 0)
      #define SPI3_DMA_EN        1
    #else
      #error "SPI3 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC4 != 0) && (RTE_FC4_PERSEL == 1) /* SPI4 */
  #define RTE_SPI4               1

  #if (RTE_FC4_SCK_EN == 0)
    #error "Mandatory SPI4 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC4_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI4 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC4_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI4 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI4_MOSI_PORT         RTE_FC4_RXD_SDA_MOSI_PORT
  #define SPI4_MOSI_PIN          RTE_FC4_RXD_SDA_MOSI_PIN
  #define SPI4_MOSI_FUNC         RTE_FC4_RXD_SDA_MOSI_FUNC

  #define SPI4_MISO_PORT         RTE_FC4_TXD_SCL_MISO_PORT
  #define SPI4_MISO_PIN          RTE_FC4_TXD_SCL_MISO_PIN
  #define SPI4_MISO_FUNC         RTE_FC4_TXD_SCL_MISO_FUNC

  #define SPI4_SCK_PORT          RTE_FC4_SCK_PORT
  #define SPI4_SCK_PIN           RTE_FC4_SCK_PIN
  #define SPI4_SCK_FUNC          RTE_FC4_SCK_FUNC

  #if ((RTE_FC4_CTS_SDA_SSEL0_EN + RTE_FC4_RTS_SCL_SSEL1_EN) > 1)
    #error "Select only one pin for SPI4 SSEL (FC4_CTS_SDA_SSEL0 or FC4_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif

  #if (RTE_FC4_CTS_SDA_SSEL0_EN != 0)
    #define SPI4_SSEL_PORT       RTE_FC4_CTS_SDA_SSEL0_PORT
    #define SPI4_SSEL_PIN        RTE_FC4_CTS_SDA_SSEL0_PIN
    #define SPI4_SSEL_FUNC       RTE_FC4_CTS_SDA_SSEL0_FUNC
    #define SPI4_SSEL_NUM        0
  #endif
  #if (RTE_FC4_RTS_SCL_SSEL1_EN != 0)
    #define SPI4_SSEL_PORT       RTE_FC4_RTS_SCL_SSEL1_PORT
    #define SPI4_SSEL_PIN        RTE_FC4_RTS_SCL_SSEL1_PIN
    #define SPI4_SSEL_FUNC       RTE_FC4_RTS_SCL_SSEL1_FUNC
    #define SPI4_SSEL_NUM        1
  #endif

  #if (RTE_FC4_DMA_RX_I2CS != 0) || (RTE_FC4_DMA_TX_I2CM != 0)
    #if (RTE_FC4_DMA_RX_I2CS != 0) && (RTE_FC4_DMA_TX_I2CM != 0)
      #define SPI4_DMA_EN        1
    #else
      #error "SPI4 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC5 != 0) && (RTE_FC5_PERSEL == 1) /* SPI5 */
  #define RTE_SPI5               1

  #if (RTE_FC5_SCK_EN == 0)
    #error "Mandatory SPI5 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC5_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI5 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC5_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI5 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI5_MOSI_PORT         RTE_FC5_RXD_SDA_MOSI_PORT
  #define SPI5_MOSI_PIN          RTE_FC5_RXD_SDA_MOSI_PIN
  #define SPI5_MOSI_FUNC         RTE_FC5_RXD_SDA_MOSI_FUNC

  #define SPI5_MISO_PORT         RTE_FC5_TXD_SCL_MISO_PORT
  #define SPI5_MISO_PIN          RTE_FC5_TXD_SCL_MISO_PIN
  #define SPI5_MISO_FUNC         RTE_FC5_TXD_SCL_MISO_FUNC

  #define SPI5_SCK_PORT          RTE_FC5_SCK_PORT
  #define SPI5_SCK_PIN           RTE_FC5_SCK_PIN
  #define SPI5_SCK_FUNC          RTE_FC5_SCK_FUNC

  #if ((RTE_FC5_CTS_SDA_SSEL0_EN + RTE_FC5_RTS_SCL_SSEL1_EN + RTE_FC5_SSEL2_EN + RTE_FC5_SSEL3_EN) > 1)
    #error "Select only one pin for SPI5 SSEL (FC5_CTS_SDA_SSEL0, FC5_RTS_SCL_SSEL1, FC5_SSEL2, FC5_SSEL3) in RTE_Device.h!"
  #endif

  #if (RTE_FC5_CTS_SDA_SSEL0_EN != 0)
    #define SPI5_SSEL_PORT       RTE_FC5_CTS_SDA_SSEL0_PORT
    #define SPI5_SSEL_PIN        RTE_FC5_CTS_SDA_SSEL0_PIN
    #define SPI5_SSEL_FUNC       RTE_FC5_CTS_SDA_SSEL0_FUNC
    #define SPI5_SSEL_NUM        0
  #endif
  #if (RTE_FC5_RTS_SCL_SSEL1_EN != 0)
    #define SPI5_SSEL_PORT       RTE_FC5_RTS_SCL_SSEL1_PORT
    #define SPI5_SSEL_PIN        RTE_FC5_RTS_SCL_SSEL1_PIN
    #define SPI5_SSEL_FUNC       RTE_FC5_RTS_SCL_SSEL1_FUNC
    #define SPI5_SSEL_NUM        1
  #endif
  #if (RTE_FC5_SSEL2_EN != 0)
    #define SPI5_SSEL_PORT       RTE_FC5_SSEL2_PORT
    #define SPI5_SSEL_PIN        RTE_FC5_SSEL2_PIN
    #define SPI5_SSEL_FUNC       RTE_FC5_SSEL2_FUNC
    #define SPI5_SSEL_NUM        2
  #endif
  #if (RTE_FC5_SSEL3_EN != 0)
    #define SPI5_SSEL_PORT       RTE_FC5_SSEL3_PORT
    #define SPI5_SSEL_PIN        RTE_FC5_SSEL3_PIN
    #define SPI5_SSEL_FUNC       RTE_FC5_SSEL3_FUNC
    #define SPI5_SSEL_NUM        3
  #endif

  #if (RTE_FC5_DMA_RX_I2CS != 0) || (RTE_FC5_DMA_TX_I2CM != 0)
    #if (RTE_FC5_DMA_RX_I2CS != 0) && (RTE_FC5_DMA_TX_I2CM != 0)
      #define SPI5_DMA_EN        1
    #else
      #error "SPI5 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC6 != 0) && (RTE_FC6_PERSEL == 1) /* SPI6 */
  #define RTE_SPI6               1

  #if (RTE_FC6_SCK_EN == 0)
    #error "Mandatory SPI6 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC6_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI6 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC6_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI6 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI6_MOSI_PORT         RTE_FC6_RXD_SDA_MOSI_PORT
  #define SPI6_MOSI_PIN          RTE_FC6_RXD_SDA_MOSI_PIN
  #define SPI6_MOSI_FUNC         RTE_FC6_RXD_SDA_MOSI_FUNC

  #define SPI6_MISO_PORT         RTE_FC6_TXD_SCL_MISO_PORT
  #define SPI6_MISO_PIN          RTE_FC6_TXD_SCL_MISO_PIN
  #define SPI6_MISO_FUNC         RTE_FC6_TXD_SCL_MISO_FUNC

  #define SPI6_SCK_PORT          RTE_FC6_SCK_PORT
  #define SPI6_SCK_PIN           RTE_FC6_SCK_PIN
  #define SPI6_SCK_FUNC          RTE_FC6_SCK_FUNC

  #if ((RTE_FC6_CTS_SDA_SSEL0_EN + RTE_FC6_RTS_SCL_SSEL1_EN) > 1)
    #error "Select only one pin for SPI6 SSEL (FC6_CTS_SDA_SSEL0 or FC6_RTS_SCL_SSEL1) in RTE_Device.h!"
  #endif

  #if (RTE_FC6_CTS_SDA_SSEL0_EN != 0)
    #define SPI6_SSEL_PORT       RTE_FC6_CTS_SDA_SSEL0_PORT
    #define SPI6_SSEL_PIN        RTE_FC6_CTS_SDA_SSEL0_PIN
    #define SPI6_SSEL_FUNC       RTE_FC6_CTS_SDA_SSEL0_FUNC
    #define SPI6_SSEL_NUM        0
  #endif
  #if (RTE_FC6_RTS_SCL_SSEL1_EN != 0)
    #define SPI6_SSEL_PORT       RTE_FC6_RTS_SCL_SSEL1_PORT
    #define SPI6_SSEL_PIN        RTE_FC6_RTS_SCL_SSEL1_PIN
    #define SPI6_SSEL_FUNC       RTE_FC6_RTS_SCL_SSEL1_FUNC
    #define SPI6_SSEL_NUM        1
  #endif

  #if (RTE_FC6_DMA_RX_I2CS != 0) || (RTE_FC6_DMA_TX_I2CM != 0)
    #if (RTE_FC6_DMA_RX_I2CS != 0) && (RTE_FC6_DMA_TX_I2CM != 0)
      #define SPI6_DMA_EN        1
    #else
      #error "SPI6 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if (RTE_FC7 != 0) && (RTE_FC7_PERSEL == 1) /* SPI7 */
  #define RTE_SPI7               1

  #if (RTE_FC7_SCK_EN == 0)
    #error "Mandatory SPI7 SCK pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC7_RXD_SDA_MOSI_EN == 0)
    #error "Mandatory SPI7 MOSI pin is not configured in RTE_Device.h!"
  #endif
  #if (RTE_FC7_TXD_SCL_MISO_EN == 0)
    #error "Mandatory SPI7 MISO pin is not configured in RTE_Device.h!"
  #endif

  #define SPI7_MOSI_PORT         RTE_FC7_RXD_SDA_MOSI_PORT
  #define SPI7_MOSI_PIN          RTE_FC7_RXD_SDA_MOSI_PIN
  #define SPI7_MOSI_FUNC         RTE_FC7_RXD_SDA_MOSI_FUNC

  #define SPI7_MISO_PORT         RTE_FC7_TXD_SCL_MISO_PORT
  #define SPI7_MISO_PIN          RTE_FC7_TXD_SCL_MISO_PIN
  #define SPI7_MISO_FUNC         RTE_FC7_TXD_SCL_MISO_FUNC

  #define SPI7_SCK_PORT          RTE_FC7_SCK_PORT
  #define SPI7_SCK_PIN           RTE_FC7_SCK_PIN
  #define SPI7_SCK_FUNC          RTE_FC7_SCK_FUNC

  #if ((RTE_FC7_CTS_SDA_SSEL0_EN + RTE_FC7_RTS_SCL_SSEL1_EN + RTE_FC7_SSEL2_EN + RTE_FC7_SSEL3_EN) > 1)
    #error "Select only one pin for SPI7 SSEL (FC7_CTS_SDA_SSEL0, FC7_RTS_SCL_SSEL1, FC7_SSEL2, FC7_SSEL3) in RTE_Device.h!"
  #endif

  #if (RTE_FC7_CTS_SDA_SSEL0_EN != 0)
    #define SPI7_SSEL_PORT       RTE_FC7_CTS_SDA_SSEL0_PORT
    #define SPI7_SSEL_PIN        RTE_FC7_CTS_SDA_SSEL0_PIN
    #define SPI7_SSEL_FUNC       RTE_FC7_CTS_SDA_SSEL0_FUNC
    #define SPI7_SSEL_NUM        0
  #endif
  #if (RTE_FC7_RTS_SCL_SSEL1_EN != 0)
    #define SPI7_SSEL_PORT       RTE_FC7_RTS_SCL_SSEL1_PORT
    #define SPI7_SSEL_PIN        RTE_FC7_RTS_SCL_SSEL1_PIN
    #define SPI7_SSEL_FUNC       RTE_FC7_RTS_SCL_SSEL1_FUNC
    #define SPI7_SSEL_NUM        1
  #endif
  #if (RTE_FC7_SSEL2_EN != 0)
    #define SPI7_SSEL_PORT       RTE_FC7_SSEL2_PORT
    #define SPI7_SSEL_PIN        RTE_FC7_SSEL2_PIN
    #define SPI7_SSEL_FUNC       RTE_FC7_SSEL2_FUNC
    #define SPI7_SSEL_NUM        2
  #endif
  #if (RTE_FC7_SSEL3_EN != 0)
    #define SPI7_SSEL_PORT       RTE_FC7_SSEL3_PORT
    #define SPI7_SSEL_PIN        RTE_FC7_SSEL3_PIN
    #define SPI7_SSEL_FUNC       RTE_FC7_SSEL3_FUNC
    #define SPI7_SSEL_NUM        3
  #endif

  #if (RTE_FC7_DMA_RX_I2CS != 0) || (RTE_FC7_DMA_TX_I2CM != 0)
    #if (RTE_FC7_DMA_RX_I2CS != 0) && (RTE_FC7_DMA_TX_I2CM != 0)
      #define SPI7_DMA_EN        1
    #else
      #error "SPI7 requires Rx and Tx channel configured in RTE_Device.h when using DMA data transfer!"
    #endif
  #endif
#endif

#if defined(SPI0_DMA_EN) || defined(SPI1_DMA_EN) || defined(SPI2_DMA_EN) || defined(SPI3_DMA_EN) || \
    defined(SPI4_DMA_EN) || defined(SPI5_DMA_EN) || defined(SPI6_DMA_EN) || defined(SPI7_DMA_EN)
  #define SPI_DMA_ENABLE         1
#endif


/* SPI instances */
#define SPI0_FLEXCOMM            0
#define SPI1_FLEXCOMM            1
#define SPI2_FLEXCOMM            2
#define SPI3_FLEXCOMM            3
#define SPI4_FLEXCOMM            4
#define SPI5_FLEXCOMM            5
#define SPI6_FLEXCOMM            6
#define SPI7_FLEXCOMM            7

/* SYSCON clock defines */
#define SPI0_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM0
#define SPI1_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM1
#define SPI2_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM2
#define SPI3_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM3
#define SPI4_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM3
#define SPI5_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM4
#define SPI6_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM5
#define SPI7_SYSCON_CLOCK        SYSCON_CLOCK_FLEXCOMM6

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
#define SPI_XFER_DMA   (uint8_t)(1U << 5)   // Transfers data using DMA

/* Transfer status flags */
#define XFER_CTRL_XPENDING  ((uint8_t)0x01) // Transfer pending
#define XFER_CTRL_ADDR_DONE ((uint8_t)0x02) // Addressing done

/* FIFO control register access pointer */
#define SPI_FIFOCTRL(x) *((uint16_t volatile *)((uint32_t)&(x->FIFOWR) + 2))

/* SPI Transfer Information (Run-Time) */
typedef struct spi_transfer_info {
  uint32_t              num;                // Total number of transfers
  uint8_t              *rx_buf;             // Rx data pointer
  uint8_t              *tx_buf;             // Tx data pointer
  uint32_t              rx_cnt;             // Rx data counter
  uint32_t              tx_cnt;             // Tx data counter
  uint32_t              rx_pend;            // Count of Rx pending transfers
  uint32_t              tx_pend;            // Count of Tx pending transfers
  uint32_t              def_val;            // Default transfer value
  uint32_t              dummy;              // Data dump variable
  uint32_t              ctrl;               // FIFO control value
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

/* SPI DMA Information */
typedef const struct spi_dma {
  DMA_CHID_T     ch;                        // Channel ID
  DMA_CALLBACK_T callback;                  // Channel callback function
} SPI_DMA;

/* SPI pin description */
typedef struct spi_pin {
  uint8_t port;                             // Port number
  uint8_t pin;                              // Pin number
  uint8_t func;                             // Pin function
} SPI_PIN;

/* SPI SSEL pin description */
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
  SPI_DMA              *dma_tx;             // Tx DMA info
  SPI_DMA              *dma_rx;             // Rx DMA info
  SPI_INFO             *info;               // Run-Time control information
} const SPI_RESOURCES;

#endif /* __SPI_LPC5411X_H */
