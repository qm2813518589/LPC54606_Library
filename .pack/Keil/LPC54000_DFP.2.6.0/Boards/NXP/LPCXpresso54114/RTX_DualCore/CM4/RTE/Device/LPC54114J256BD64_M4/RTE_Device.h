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
 * $Date:        4. January 2016
 * $Revision:    V1.0.0
 *
 * Project:      RTE Device Configuration for NXP LPC5411x
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

// <h> Flexcomm Interface Serial Communication [Driver_ I2C, I2S, SPI, USART]
// <i> Configuration settings for Flexcomm Interface Serial Communication

//   <e> Flexcomm Interface 0
//   <i> Flexcomm Interface 0 provides I2C, SPI and USART peripheral functions
#define   RTE_FC0                           0
//     <o> Peripheral Select <0=>I2C0 <1=>SPI0 <2=>USART0
//     <i> Select Flexcomm Interface 0 peripheral function:
//     <i> - I2C0 enables Driver_I2C0 in component ::CMSIS Driver:I2C
//     <i> - SPI0 enables Driver_SPI0 in component ::CMSIS Driver:SPI
//     <i> - USART0 enables Driver_USART0 in component ::CMSIS Driver:USART
#define   RTE_FC0_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC0_SCK Pin <0=> Not used <1=> PIO0_4 <2=> PIO0_20
#define   RTE_FC0_SCK_ID                    0
#if      (RTE_FC0_SCK_ID == 0)
  #define RTE_FC0_SCK_EN                    0
#elif    (RTE_FC0_SCK_ID == 1)
  #define RTE_FC0_SCK_PORT                  0
  #define RTE_FC0_SCK_PIN                   4
  #define RTE_FC0_SCK_FUNC                  1
#elif    (RTE_FC0_SCK_ID == 2)
  #define RTE_FC0_SCK_PORT                  0
  #define RTE_FC0_SCK_PIN                   20
  #define RTE_FC0_SCK_FUNC                  2
#else
  #error "Invalid Flexcomm Interface 0 FC0_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC0_SCK_EN
  #define RTE_FC0_SCK_EN                    1
#endif

//       <o>FC0_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_0 <2=> PIO0_22
#define   RTE_FC0_RXD_SDA_MOSI_ID           1
#if      (RTE_FC0_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC0_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC0_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC0_RXD_SDA_MOSI_PORT         0
  #define RTE_FC0_RXD_SDA_MOSI_PIN          0
  #define RTE_FC0_RXD_SDA_MOSI_FUNC         1
#elif    (RTE_FC0_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC0_RXD_SDA_MOSI_PORT         0
  #define RTE_FC0_RXD_SDA_MOSI_PIN          22
  #define RTE_FC0_RXD_SDA_MOSI_FUNC         2
#else
  #error "Invalid Flexcomm Interface 0 FC0_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC0_RXD_SDA_MOSI_EN
  #define RTE_FC0_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC0_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_1 <2=> PIO0_21
#define   RTE_FC0_TXD_SCL_MISO_ID           1
#if      (RTE_FC0_TXD_SCL_MISO_ID == 0)
  #define RTE_FC0_TXD_SCL_MISO_EN           0
#elif    (RTE_FC0_TXD_SCL_MISO_ID == 1)
  #define RTE_FC0_TXD_SCL_MISO_PORT         0
  #define RTE_FC0_TXD_SCL_MISO_PIN          1
  #define RTE_FC0_TXD_SCL_MISO_FUNC         1
#elif    (RTE_FC0_TXD_SCL_MISO_ID == 2)
  #define RTE_FC0_TXD_SCL_MISO_PORT         0
  #define RTE_FC0_TXD_SCL_MISO_PIN          21
  #define RTE_FC0_TXD_SCL_MISO_FUNC         2
#else
  #error "Invalid Flexcomm Interface 0 FC0_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC0_TXD_SCL_MISO_EN
  #define RTE_FC0_TXD_SCL_MISO_EN           1
#endif

//       <o>FC0_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_2
#define   RTE_FC0_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC0_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC0_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC0_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC0_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC0_CTS_SDA_SSEL0_PIN         2
  #define RTE_FC0_CTS_SDA_SSEL0_FUNC        1
#else
  #error "Invalid Flexcomm Interface 0 FC0_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC0_CTS_SDA_SSEL0_EN
  #define RTE_FC0_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC0_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_3
#define   RTE_FC0_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC0_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC0_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC0_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC0_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC0_RTS_SCL_SSEL1_PIN         3
  #define RTE_FC0_RTS_SCL_SSEL1_FUNC        1
#else
  #error "Invalid Flexcomm Interface 0 FC0_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC0_RTS_SCL_SSEL1_EN
  #define RTE_FC0_RTS_SCL_SSEL1_EN          1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC0_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC0_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 1
//   <i> Flexcomm Interface 1 provides I2C, SPI and USART peripheral functions
#define   RTE_FC1                           0
//     <o> Peripheral Select <0=>I2C1 <1=>SPI1 <2=>USART1
//     <i> Select Flexcomm Interface 1 peripheral function:
//     <i> - I2C1 enables Driver_I2C1 in component ::CMSIS Driver:I2C
//     <i> - SPI1 enables Driver_SPI1 in component ::CMSIS Driver:SPI
//     <i> - USART1 enables Driver_USART1 in component ::CMSIS Driver:USART
#define   RTE_FC1_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC1_SCK Pin <0=> Not used <1=> PIO0_14 <2=> PIO1_10
#define   RTE_FC1_SCK_ID                    0
#if      (RTE_FC1_SCK_ID == 0)
  #define RTE_FC1_SCK_EN                    0
#elif    (RTE_FC1_SCK_ID == 1)
  #define RTE_FC1_SCK_PORT                  0
  #define RTE_FC1_SCK_PIN                   14
  #define RTE_FC1_SCK_FUNC                  5
#elif    (RTE_FC1_SCK_ID == 2)
  #define RTE_FC1_SCK_PORT                  1
  #define RTE_FC1_SCK_PIN                   10
  #define RTE_FC1_SCK_FUNC                  4
#else
  #error "Invalid Flexcomm Interface 1 FC1_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC1_SCK_EN
  #define RTE_FC1_SCK_EN                    1
#endif

//       <o>FC1_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_29
#define   RTE_FC1_RXD_SDA_MOSI_ID           0
#if      (RTE_FC1_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC1_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC1_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC1_RXD_SDA_MOSI_PORT         0
  #define RTE_FC1_RXD_SDA_MOSI_PIN          29
  #define RTE_FC1_RXD_SDA_MOSI_FUNC         1
#else
  #error "Invalid Flexcomm Interface 1 FC1_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC1_RXD_SDA_MOSI_EN
  #define RTE_FC1_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC1_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_30
#define   RTE_FC1_TXD_SCL_MISO_ID           0
#if      (RTE_FC1_TXD_SCL_MISO_ID == 0)
  #define RTE_FC1_TXD_SCL_MISO_EN           0
#elif    (RTE_FC1_TXD_SCL_MISO_ID == 1)
  #define RTE_FC1_TXD_SCL_MISO_PORT         0
  #define RTE_FC1_TXD_SCL_MISO_PIN          30
  #define RTE_FC1_TXD_SCL_MISO_FUNC         1
#else
  #error "Invalid Flexcomm Interface 1 FC1_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC1_TXD_SCL_MISO_EN
  #define RTE_FC1_TXD_SCL_MISO_EN           1
#endif

//       <o>FC1_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_24
#define   RTE_FC1_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC1_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC1_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC1_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC1_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC1_CTS_SDA_SSEL0_PIN         24
  #define RTE_FC1_CTS_SDA_SSEL0_FUNC        1
#else
  #error "Invalid Flexcomm Interface 1 FC1_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC1_CTS_SDA_SSEL0_EN
  #define RTE_FC1_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC1_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_23
#define   RTE_FC1_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC1_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC1_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC1_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC1_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC1_RTS_SCL_SSEL1_PIN         23
  #define RTE_FC1_RTS_SCL_SSEL1_FUNC        1
#else
  #error "Invalid Flexcomm Interface 1 FC1_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC1_RTS_SCL_SSEL1_EN
  #define RTE_FC1_RTS_SCL_SSEL1_EN          1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC1_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC1_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 2
//   <i> Flexcomm Interface 2 provides I2C, SPI and USART peripheral functions
#define   RTE_FC2                           0
//     <o> Peripheral Select <0=>I2C2 <1=>SPI2 <2=>USART2
//     <i> Select Flexcomm Interface 2 peripheral function:
//     <i> - I2C2 enables Driver_I2C2 in component ::CMSIS Driver:I2C
//     <i> - SPI2 enables Driver_SPI2 in component ::CMSIS Driver:SPI
//     <i> - USART2 enables Driver_USART2 in component ::CMSIS Driver:USART
#define   RTE_FC2_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC2_SCK Pin <0=> Not used <1=> PIO0_10
#define   RTE_FC2_SCK_ID                    0
#if      (RTE_FC2_SCK_ID == 0)
  #define RTE_FC2_SCK_EN                    0
#elif    (RTE_FC2_SCK_ID == 1)
  #define RTE_FC2_SCK_PORT                  0
  #define RTE_FC2_SCK_PIN                   10
  #define RTE_FC2_SCK_FUNC                  1
#else
  #error "Invalid Flexcomm Interface 2 FC2_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC2_SCK_EN
  #define RTE_FC2_SCK_EN                    1
#endif

//       <o>FC2_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_8 <2=> PIO1_14
#define   RTE_FC2_RXD_SDA_MOSI_ID           0
#if      (RTE_FC2_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC2_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC2_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC2_RXD_SDA_MOSI_PORT         0
  #define RTE_FC2_RXD_SDA_MOSI_PIN          8
  #define RTE_FC2_RXD_SDA_MOSI_FUNC         1
#elif    (RTE_FC2_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC2_RXD_SDA_MOSI_PORT         1
  #define RTE_FC2_RXD_SDA_MOSI_PIN          14
  #define RTE_FC2_RXD_SDA_MOSI_FUNC         2
#else
  #error "Invalid Flexcomm Interface 2 FC2_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC2_RXD_SDA_MOSI_EN
  #define RTE_FC2_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC2_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_9
#define   RTE_FC2_TXD_SCL_MISO_ID           0
#if      (RTE_FC2_TXD_SCL_MISO_ID == 0)
  #define RTE_FC2_TXD_SCL_MISO_EN           0
#elif    (RTE_FC2_TXD_SCL_MISO_ID == 1)
  #define RTE_FC2_TXD_SCL_MISO_PORT         0
  #define RTE_FC2_TXD_SCL_MISO_PIN          9
  #define RTE_FC2_TXD_SCL_MISO_FUNC         1
#else
  #error "Invalid Flexcomm Interface 2 FC2_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC2_TXD_SCL_MISO_EN
  #define RTE_FC2_TXD_SCL_MISO_EN           1
#endif

//       <o>FC2_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_31
#define   RTE_FC2_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC2_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC2_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC2_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC2_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC2_CTS_SDA_SSEL0_PIN         31
  #define RTE_FC2_CTS_SDA_SSEL0_FUNC        2
#else
  #error "Invalid Flexcomm Interface 2 FC2_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC2_CTS_SDA_SSEL0_EN
  #define RTE_FC2_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC2_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO1_0
#define   RTE_FC2_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC2_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC2_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC2_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC2_RTS_SCL_SSEL1_PORT        1
  #define RTE_FC2_RTS_SCL_SSEL1_PIN         0
  #define RTE_FC2_RTS_SCL_SSEL1_FUNC        2
#else
  #error "Invalid Flexcomm Interface 2 FC2_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC2_RTS_SCL_SSEL1_EN
  #define RTE_FC2_RTS_SCL_SSEL1_EN          1
#endif

//       <o>FC2_SSEL2 Pin <0=> Not used <1=> PIO0_3
#define   RTE_FC2_SSEL2_ID                  0
#if      (RTE_FC2_SSEL2_ID == 0)
  #define RTE_FC2_SSEL2_EN                  0
#elif    (RTE_FC2_SSEL2_ID == 1)
  #define RTE_FC2_SSEL2_PORT                0
  #define RTE_FC2_SSEL2_PIN                 3
  #define RTE_FC2_SSEL2_FUNC                3
#else
  #error "Invalid Flexcomm Interface 2 FC2_SSEL2 Pin Configuration!"
#endif

#ifndef   RTE_FC2_SSEL2_EN
  #define RTE_FC2_SSEL2_EN                  1
#endif

//       <o>FC2_SSEL3 Pin <0=> Not used <1=> PIO0_2
#define   RTE_FC2_SSEL3_ID                  0
#if      (RTE_FC2_SSEL3_ID == 0)
  #define RTE_FC2_SSEL3_EN                  0
#elif    (RTE_FC2_SSEL3_ID == 1)
  #define RTE_FC2_SSEL3_PORT                0
  #define RTE_FC2_SSEL3_PIN                 2
  #define RTE_FC2_SSEL3_FUNC                2
#else
  #error "Invalid Flexcomm Interface 2 FC2_SSEL3 Pin Configuration!"
#endif

#ifndef   RTE_FC2_SSEL3_EN
  #define RTE_FC2_SSEL3_EN                  1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC2_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC2_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 3
//   <i> Flexcomm Interface 3 provides I2C, SPI and USART peripheral functions
#define   RTE_FC3                           0
//     <o> Peripheral Select <0=>I2C3 <1=>SPI3 <2=>USART3
//     <i> Select Flexcomm Interface 3 peripheral function:
//     <i> - I2C3 enables Driver_I2C3 in component ::CMSIS Driver:I2C
//     <i> - SPI3 enables Driver_SPI3 in component ::CMSIS Driver:SPI
//     <i> - USART3 enables Driver_USART3 in component ::CMSIS Driver:USART
#define   RTE_FC3_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC3_SCK Pin <0=> Not used <1=> PIO0_11 <2=> PIO1_3
#define   RTE_FC3_SCK_ID                    0
#if      (RTE_FC3_SCK_ID == 0)
  #define RTE_FC3_SCK_EN                    0
#elif    (RTE_FC3_SCK_ID == 1)
  #define RTE_FC3_SCK_PORT                  0
  #define RTE_FC3_SCK_PIN                   11
  #define RTE_FC3_SCK_FUNC                  1
#elif    (RTE_FC3_SCK_ID == 2)
  #define RTE_FC3_SCK_PORT                  1
  #define RTE_FC3_SCK_PIN                   3
  #define RTE_FC3_SCK_FUNC                  5
#else
  #error "Invalid Flexcomm Interface 3 FC3_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC3_SCK_EN
  #define RTE_FC3_SCK_EN                    1
#endif

//       <o>FC3_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_12 <2=> PIO1_9
#define   RTE_FC3_RXD_SDA_MOSI_ID           0
#if      (RTE_FC3_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC3_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC3_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC3_RXD_SDA_MOSI_PORT         0
  #define RTE_FC3_RXD_SDA_MOSI_PIN          12
  #define RTE_FC3_RXD_SDA_MOSI_FUNC         1
#elif    (RTE_FC3_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC3_RXD_SDA_MOSI_PORT         1
  #define RTE_FC3_RXD_SDA_MOSI_PIN          9
  #define RTE_FC3_RXD_SDA_MOSI_FUNC         2
#else
  #error "Invalid Flexcomm Interface 3 FC3_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC3_RXD_SDA_MOSI_EN
  #define RTE_FC3_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC3_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_13 <2=> PIO1_10
#define   RTE_FC3_TXD_SCL_MISO_ID           0
#if      (RTE_FC3_TXD_SCL_MISO_ID == 0)
  #define RTE_FC3_TXD_SCL_MISO_EN           0
#elif    (RTE_FC3_TXD_SCL_MISO_ID == 1)
  #define RTE_FC3_TXD_SCL_MISO_PORT         0
  #define RTE_FC3_TXD_SCL_MISO_PIN          13
  #define RTE_FC3_TXD_SCL_MISO_FUNC         1
#elif    (RTE_FC3_TXD_SCL_MISO_ID == 2)
  #define RTE_FC3_TXD_SCL_MISO_PORT         1
  #define RTE_FC3_TXD_SCL_MISO_PIN          10
  #define RTE_FC3_TXD_SCL_MISO_FUNC         2
#else
  #error "Invalid Flexcomm Interface 3 FC3_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC3_TXD_SCL_MISO_EN
  #define RTE_FC3_TXD_SCL_MISO_EN           1
#endif

//       <o>FC3_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_0 <2=> PIO0_9 <3=> PIO0_14
#define   RTE_FC3_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC3_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC3_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC3_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC3_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC3_CTS_SDA_SSEL0_PIN         0
  #define RTE_FC3_CTS_SDA_SSEL0_FUNC        2
#elif    (RTE_FC3_CTS_SDA_SSEL0_ID == 2)
  #define RTE_FC3_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC3_CTS_SDA_SSEL0_PIN         9
  #define RTE_FC3_CTS_SDA_SSEL0_FUNC        5
#elif    (RTE_FC3_CTS_SDA_SSEL0_ID == 3)
  #define RTE_FC3_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC3_CTS_SDA_SSEL0_PIN         14
  #define RTE_FC3_CTS_SDA_SSEL0_FUNC        1
#else
  #error "Invalid Flexcomm Interface 3 FC3_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC3_CTS_SDA_SSEL0_EN
  #define RTE_FC3_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC3_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_1 <2=> PIO0_15
#define   RTE_FC3_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC3_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC3_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC3_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC3_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC3_RTS_SCL_SSEL1_PIN         1
  #define RTE_FC3_RTS_SCL_SSEL1_FUNC        2
#elif    (RTE_FC3_RTS_SCL_SSEL1_ID == 2)
  #define RTE_FC3_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC3_RTS_SCL_SSEL1_PIN         15
  #define RTE_FC3_RTS_SCL_SSEL1_FUNC        1
#else
  #error "Invalid Flexcomm Interface 3 FC3_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC3_RTS_SCL_SSEL1_EN
  #define RTE_FC3_RTS_SCL_SSEL1_EN          1
#endif

//       <o>FC3_SSEL2 Pin <0=> Not used <1=> PIO0_4 <2=> PIO0_16
#define   RTE_FC3_SSEL2_ID                  0
#if      (RTE_FC3_SSEL2_ID == 0)
  #define RTE_FC3_SSEL2_EN                  0
#elif    (RTE_FC3_SSEL2_ID == 1)
  #define RTE_FC3_SSEL2_PORT                0
  #define RTE_FC3_SSEL2_PIN                 4
  #define RTE_FC3_SSEL2_FUNC                2
#elif    (RTE_FC3_SSEL2_ID == 2)
  #define RTE_FC3_SSEL2_PORT                0
  #define RTE_FC3_SSEL2_PIN                 16
  #define RTE_FC3_SSEL2_FUNC                1
#else
  #error "Invalid Flexcomm Interface 3 FC3_SSEL2 Pin Configuration!"
#endif

#ifndef   RTE_FC3_SSEL2_EN
  #define RTE_FC3_SSEL2_EN                  1
#endif

//       <o>FC3_SSEL3 Pin <0=> Not used <1=> PIO0_17
#define   RTE_FC3_SSEL3_ID                  0
#if      (RTE_FC3_SSEL3_ID == 0)
  #define RTE_FC3_SSEL3_EN                  0
#elif    (RTE_FC3_SSEL3_ID == 1)
  #define RTE_FC3_SSEL3_PORT                0
  #define RTE_FC3_SSEL3_PIN                 17
  #define RTE_FC3_SSEL3_FUNC                1
#else
  #error "Invalid Flexcomm Interface 3 FC3_SSEL3 Pin Configuration!"
#endif

#ifndef   RTE_FC3_SSEL3_EN
  #define RTE_FC3_SSEL3_EN                  1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC3_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC3_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 4
//   <i> Flexcomm Interface 4 provides I2C, SPI and USART peripheral functions
#define   RTE_FC4                           0
//     <o> Peripheral Select <0=>I2C4 <1=>SPI4 <2=>USART4
//     <i> Select Flexcomm Interface 4 peripheral function:
//     <i> - I2C4 enables Driver_I2C4 in component ::CMSIS Driver:I2C
//     <i> - SPI4 enables Driver_SPI4 in component ::CMSIS Driver:SPI
//     <i> - USART4 enables Driver_USART4 in component ::CMSIS Driver:USART
#define   RTE_FC4_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC4_SCK Pin <0=> Not used <1=> PIO0_15 <2=> PIO1_11
#define   RTE_FC4_SCK_ID                    0
#if      (RTE_FC4_SCK_ID == 0)
  #define RTE_FC4_SCK_EN                    0
#elif    (RTE_FC4_SCK_ID == 1)
  #define RTE_FC4_SCK_PORT                  0
  #define RTE_FC4_SCK_PIN                   15
  #define RTE_FC4_SCK_FUNC                  5
#elif    (RTE_FC4_SCK_ID == 2)
  #define RTE_FC4_SCK_PORT                  1
  #define RTE_FC4_SCK_PIN                   11
  #define RTE_FC4_SCK_FUNC                  4
#else
  #error "Invalid Flexcomm Interface 4 FC4_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC4_SCK_EN
  #define RTE_FC4_SCK_EN                    1
#endif

//       <o>FC4_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO1_2
#define   RTE_FC4_RXD_SDA_MOSI_ID           0
#if      (RTE_FC4_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC4_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC4_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC4_RXD_SDA_MOSI_PORT         1
  #define RTE_FC4_RXD_SDA_MOSI_PIN          2
  #define RTE_FC4_RXD_SDA_MOSI_FUNC         5
#else
  #error "Invalid Flexcomm Interface 4 FC4_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC4_RXD_SDA_MOSI_EN
  #define RTE_FC4_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC4_TXD_SCL_MISO Pin <0=> Not used <1=> PIO1_1
#define   RTE_FC4_TXD_SCL_MISO_ID           0
#if      (RTE_FC4_TXD_SCL_MISO_ID == 0)
  #define RTE_FC4_TXD_SCL_MISO_EN           0
#elif    (RTE_FC4_TXD_SCL_MISO_ID == 1)
  #define RTE_FC4_TXD_SCL_MISO_PORT         1
  #define RTE_FC4_TXD_SCL_MISO_PIN          1
  #define RTE_FC4_TXD_SCL_MISO_FUNC         5
#else
  #error "Invalid Flexcomm Interface 4 FC4_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC4_TXD_SCL_MISO_EN
  #define RTE_FC4_TXD_SCL_MISO_EN           1
#endif

//       <o>FC4_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_26
#define   RTE_FC4_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC4_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC4_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC4_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC4_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC4_CTS_SDA_SSEL0_PIN         26
  #define RTE_FC4_CTS_SDA_SSEL0_FUNC        1
#else
  #error "Invalid Flexcomm Interface 4 FC4_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC4_CTS_SDA_SSEL0_EN
  #define RTE_FC4_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC4_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_25
#define   RTE_FC4_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC4_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC4_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC4_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC4_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC4_RTS_SCL_SSEL1_PIN         25
  #define RTE_FC4_RTS_SCL_SSEL1_FUNC        1
#else
  #error "Invalid Flexcomm Interface 4 FC4_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC4_RTS_SCL_SSEL1_EN
  #define RTE_FC4_RTS_SCL_SSEL1_EN          1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC4_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC4_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 5
//   <i> Flexcomm Interface 5 provides I2C, SPI and USART peripheral functions
#define   RTE_FC5                           0
//     <o> Peripheral Select <0=>I2C5 <1=>SPI5 <2=>USART5
//     <i> Select Flexcomm Interface 5 peripheral function:
//     <i> - I2C5 enables Driver_I2C5 in component ::CMSIS Driver:I2C
//     <i> - SPI5 enables Driver_SPI5 in component ::CMSIS Driver:SPI
//     <i> - USART5 enables Driver_USART5 in component ::CMSIS Driver:USART
#define   RTE_FC5_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC5_SCK Pin <0=> Not used <1=> PIO0_19
#define   RTE_FC5_SCK_ID                    0
#if      (RTE_FC5_SCK_ID == 0)
  #define RTE_FC5_SCK_EN                    0
#elif    (RTE_FC5_SCK_ID == 1)
  #define RTE_FC5_SCK_PORT                  0
  #define RTE_FC5_SCK_PIN                   19
  #define RTE_FC5_SCK_FUNC                  1
#else
  #error "Invalid Flexcomm Interface 5 FC5_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC5_SCK_EN
  #define RTE_FC5_SCK_EN                    1
#endif

//       <o>FC5_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_20 <2=> PIO1_12
#define   RTE_FC5_RXD_SDA_MOSI_ID           0
#if      (RTE_FC5_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC5_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC5_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC5_RXD_SDA_MOSI_PORT         0
  #define RTE_FC5_RXD_SDA_MOSI_PIN          20
  #define RTE_FC5_RXD_SDA_MOSI_FUNC         1
#elif    (RTE_FC5_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC5_RXD_SDA_MOSI_PORT         1
  #define RTE_FC5_RXD_SDA_MOSI_PIN          12
  #define RTE_FC5_RXD_SDA_MOSI_FUNC         2
#else
  #error "Invalid Flexcomm Interface 5 FC5_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC5_RXD_SDA_MOSI_EN
  #define RTE_FC5_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC5_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_18 <2=> PIO1_13
#define   RTE_FC5_TXD_SCL_MISO_ID           0
#if      (RTE_FC5_TXD_SCL_MISO_ID == 0)
  #define RTE_FC5_TXD_SCL_MISO_EN           0
#elif    (RTE_FC5_TXD_SCL_MISO_ID == 1)
  #define RTE_FC5_TXD_SCL_MISO_PORT         0
  #define RTE_FC5_TXD_SCL_MISO_PIN          18
  #define RTE_FC5_TXD_SCL_MISO_FUNC         1
#elif    (RTE_FC5_TXD_SCL_MISO_ID == 2)
  #define RTE_FC5_TXD_SCL_MISO_PORT         1
  #define RTE_FC5_TXD_SCL_MISO_PIN          13
  #define RTE_FC5_TXD_SCL_MISO_FUNC         2
#else
  #error "Invalid Flexcomm Interface 5 FC5_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC5_TXD_SCL_MISO_EN
  #define RTE_FC5_TXD_SCL_MISO_EN           1
#endif

//       <o>FC5_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_28
#define   RTE_FC5_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC5_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC5_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC5_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC5_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC5_CTS_SDA_SSEL0_PIN         28
  #define RTE_FC5_CTS_SDA_SSEL0_FUNC        1
#else
  #error "Invalid Flexcomm Interface 5 FC5_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC5_CTS_SDA_SSEL0_EN
  #define RTE_FC5_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC5_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_27
#define   RTE_FC5_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC5_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC5_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC5_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC5_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC5_RTS_SCL_SSEL1_PIN         27
  #define RTE_FC5_RTS_SCL_SSEL1_FUNC        1
#else
  #error "Invalid Flexcomm Interface 5 FC5_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC5_RTS_SCL_SSEL1_EN
  #define RTE_FC5_RTS_SCL_SSEL1_EN          1
#endif

//       <o>FC5_SSEL2 Pin <0=> Not used <1=> PIO1_1
#define   RTE_FC5_SSEL2_ID                  0
#if      (RTE_FC5_SSEL2_ID == 0)
  #define RTE_FC5_SSEL2_EN                  0
#elif    (RTE_FC5_SSEL2_ID == 1)
  #define RTE_FC5_SSEL2_PORT                1
  #define RTE_FC5_SSEL2_PIN                 1
  #define RTE_FC5_SSEL2_FUNC                4
#else
  #error "Invalid Flexcomm Interface 5 FC5_SSEL2 Pin Configuration!"
#endif

#ifndef   RTE_FC5_SSEL2_EN
  #define RTE_FC5_SSEL2_EN                  1
#endif

//       <o>FC5_SSEL3 Pin <0=> Not used <1=> PIO1_2
#define   RTE_FC5_SSEL3_ID                  0
#if      (RTE_FC5_SSEL3_ID == 0)
  #define RTE_FC5_SSEL3_EN                  0
#elif    (RTE_FC5_SSEL3_ID == 1)
  #define RTE_FC5_SSEL3_PORT                1
  #define RTE_FC5_SSEL3_PIN                 2
  #define RTE_FC5_SSEL3_FUNC                4
#else
  #error "Invalid Flexcomm Interface 5 FC5_SSEL3 Pin Configuration!"
#endif

#ifndef   RTE_FC5_SSEL3_EN
  #define RTE_FC5_SSEL3_EN                  1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC5_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC5_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 6
//   <i> Flexcomm Interface 6 provides I2C, I2S, SPI and USART peripheral functions
#define   RTE_FC6                           0
//     <o> Peripheral Select <0=>I2C6 <3=>I2S6 <1=>SPI6 <2=>USART6
//     <i> Select Flexcomm Interface 6 peripheral function:
//     <i> - I2C6 enables Driver_I2C6 in component ::CMSIS Driver:I2C
//     <i> - I2S6 enables Driver_SAI6 in component ::CMSIS Driver:SAI
//     <i> - SPI6 enables Driver_SPI6 in component ::CMSIS Driver:SPI
//     <i> - USART6 enables Driver_USART6 in component ::CMSIS Driver:USART
#define   RTE_FC6_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC6_SCK Pin <0=> Not used <1=> PIO0_7
#define   RTE_FC6_SCK_ID                    0
#if      (RTE_FC6_SCK_ID == 0)
  #define RTE_FC6_SCK_EN                    0
#elif    (RTE_FC6_SCK_ID == 1)
  #define RTE_FC6_SCK_PORT                  0
  #define RTE_FC6_SCK_PIN                   7
  #define RTE_FC6_SCK_FUNC                  1
#else
  #error "Invalid Flexcomm Interface 6 FC6_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC6_SCK_EN
  #define RTE_FC6_SCK_EN                    1
#endif

//       <o>FC6_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO0_5 <2=> PIO0_11
#define   RTE_FC6_RXD_SDA_MOSI_ID           0
#if      (RTE_FC6_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC6_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC6_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC6_RXD_SDA_MOSI_PORT         0
  #define RTE_FC6_RXD_SDA_MOSI_PIN          5
  #define RTE_FC6_RXD_SDA_MOSI_FUNC         1
#elif    (RTE_FC6_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC6_RXD_SDA_MOSI_PORT         0
  #define RTE_FC6_RXD_SDA_MOSI_PIN          11
  #define RTE_FC6_RXD_SDA_MOSI_FUNC         2
#else
  #error "Invalid Flexcomm Interface 6 FC6_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC6_RXD_SDA_MOSI_EN
  #define RTE_FC6_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC6_TXD_SCL_MISO Pin <0=> Not used <1=> PIO0_6 <2=> PIO0_12 <3=> PIO1_10
#define   RTE_FC6_TXD_SCL_MISO_ID           0
#if      (RTE_FC6_TXD_SCL_MISO_ID == 0)
  #define RTE_FC6_TXD_SCL_MISO_EN           0
#elif    (RTE_FC6_TXD_SCL_MISO_ID == 1)
  #define RTE_FC6_TXD_SCL_MISO_PORT         0
  #define RTE_FC6_TXD_SCL_MISO_PIN          6
  #define RTE_FC6_TXD_SCL_MISO_FUNC         1
#elif    (RTE_FC6_TXD_SCL_MISO_ID == 2)
  #define RTE_FC6_TXD_SCL_MISO_PORT         0
  #define RTE_FC6_TXD_SCL_MISO_PIN          12
  #define RTE_FC6_TXD_SCL_MISO_FUNC         2
#elif    (RTE_FC6_TXD_SCL_MISO_ID == 3)
  #define RTE_FC6_TXD_SCL_MISO_PORT         1
  #define RTE_FC6_TXD_SCL_MISO_PIN          10
  #define RTE_FC6_TXD_SCL_MISO_FUNC         2
#else
  #error "Invalid Flexcomm Interface 6 FC6_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC6_TXD_SCL_MISO_EN
  #define RTE_FC6_TXD_SCL_MISO_EN           1
#endif

//       <o>FC6_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO0_16 <2=> PIO0_25
#define   RTE_FC6_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC6_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC6_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC6_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC6_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC6_CTS_SDA_SSEL0_PIN         16
  #define RTE_FC6_CTS_SDA_SSEL0_FUNC        2
#elif    (RTE_FC6_CTS_SDA_SSEL0_ID == 2)
  #define RTE_FC6_CTS_SDA_SSEL0_PORT        0
  #define RTE_FC6_CTS_SDA_SSEL0_PIN         25
  #define RTE_FC6_CTS_SDA_SSEL0_FUNC        2
#else
  #error "Invalid Flexcomm Interface 6 FC6_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC6_CTS_SDA_SSEL0_EN
  #define RTE_FC6_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC6_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO0_17 <2=> PIO1_11
#define   RTE_FC6_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC6_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC6_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC6_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC6_RTS_SCL_SSEL1_PORT        0
  #define RTE_FC6_RTS_SCL_SSEL1_PIN         17
  #define RTE_FC6_RTS_SCL_SSEL1_FUNC        2
#elif    (RTE_FC6_RTS_SCL_SSEL1_ID == 2)
  #define RTE_FC6_RTS_SCL_SSEL1_PORT        1
  #define RTE_FC6_RTS_SCL_SSEL1_PIN         11
  #define RTE_FC6_RTS_SCL_SSEL1_FUNC        2
#else
  #error "Invalid Flexcomm Interface 6 FC6_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC6_RTS_SCL_SSEL1_EN
  #define RTE_FC6_RTS_SCL_SSEL1_EN          1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC6_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC6_DMA_TX_I2CM               0
//     </h>
//   </e>

//   <e> Flexcomm Interface 7
//   <i> Flexcomm Interface 7 provides I2C, I2S, SPI and USART peripheral functions
#define   RTE_FC7                           0
//     <o> Peripheral Select <0=>I2C7 <3=>I2S7 <1=>SPI7 <2=>USART7
//     <i> Select Flexcomm Interface 7 peripheral function:
//     <i> - I2C7 enables Driver_I2C7 in component ::CMSIS Driver:I2C
//     <i> - I2S7 enables Driver_SAI7 in component ::CMSIS Driver:SAI
//     <i> - SPI7 enables Driver_SPI7 in component ::CMSIS Driver:SPI
//     <i> - USART7 enables Driver_USART7 in component ::CMSIS Driver:USART
#define   RTE_FC7_PERSEL                    0

//     <h>Pin Configuration
//       <o>FC7_SCK Pin <0=> Not used <1=> PIO1_6 <2=> PIO1_12
#define   RTE_FC7_SCK_ID                    0
#if      (RTE_FC7_SCK_ID == 0)
  #define RTE_FC7_SCK_EN                    0
#elif    (RTE_FC7_SCK_ID == 1)
  #define RTE_FC7_SCK_PORT                  1
  #define RTE_FC7_SCK_PIN                   6
  #define RTE_FC7_SCK_FUNC                  2
#elif    (RTE_FC7_SCK_ID == 2)
  #define RTE_FC7_SCK_PORT                  1
  #define RTE_FC7_SCK_PIN                   12
  #define RTE_FC7_SCK_FUNC                  4
#else
  #error "Invalid Flexcomm Interface 7 FC7_SCK Pin Configuration!"
#endif

#ifndef   RTE_FC7_SCK_EN
  #define RTE_FC7_SCK_EN                    1
#endif

//       <o>FC7_RXD_SDA_MOSI Pin <0=> Not used <1=> PIO1_7 <2=> PIO1_13
#define   RTE_FC7_RXD_SDA_MOSI_ID           0
#if      (RTE_FC7_RXD_SDA_MOSI_ID == 0)
  #define RTE_FC7_RXD_SDA_MOSI_EN           0
#elif    (RTE_FC7_RXD_SDA_MOSI_ID == 1)
  #define RTE_FC7_RXD_SDA_MOSI_PORT         1
  #define RTE_FC7_RXD_SDA_MOSI_PIN          7
  #define RTE_FC7_RXD_SDA_MOSI_FUNC         2
#elif    (RTE_FC7_RXD_SDA_MOSI_ID == 2)
  #define RTE_FC7_RXD_SDA_MOSI_PORT         1
  #define RTE_FC7_RXD_SDA_MOSI_PIN          13
  #define RTE_FC7_RXD_SDA_MOSI_FUNC         4
#else
  #error "Invalid Flexcomm Interface 7 FC7_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC7_RXD_SDA_MOSI_EN
  #define RTE_FC7_RXD_SDA_MOSI_EN           1
#endif

//       <o>FC7_TXD_SCL_MISO Pin <0=> Not used <1=> PIO1_8 <2=> PIO1_14
#define   RTE_FC7_TXD_SCL_MISO_ID           0
#if      (RTE_FC7_TXD_SCL_MISO_ID == 0)
  #define RTE_FC7_TXD_SCL_MISO_EN           0
#elif    (RTE_FC7_TXD_SCL_MISO_ID == 1)
  #define RTE_FC7_TXD_SCL_MISO_PORT         1
  #define RTE_FC7_TXD_SCL_MISO_PIN          8
  #define RTE_FC7_TXD_SCL_MISO_FUNC         2
#elif    (RTE_FC7_TXD_SCL_MISO_ID == 2)
  #define RTE_FC7_TXD_SCL_MISO_PORT         1
  #define RTE_FC7_TXD_SCL_MISO_PIN          14
  #define RTE_FC7_TXD_SCL_MISO_FUNC         4
#else
  #error "Invalid Flexcomm Interface 7 FC7_TXD_SCL_MISO Pin Configuration!"
#endif

#ifndef   RTE_FC7_TXD_SCL_MISO_EN
  #define RTE_FC7_TXD_SCL_MISO_EN           1
#endif

//       <o>FC7_CTS_SDA_SSEL0 Pin <0=> Not used <1=> PIO1_5 <2=> PIO1_15
#define   RTE_FC7_CTS_SDA_SSEL0_ID          0
#if      (RTE_FC7_CTS_SDA_SSEL0_ID == 0)
  #define RTE_FC7_CTS_SDA_SSEL0_EN          0
#elif    (RTE_FC7_CTS_SDA_SSEL0_ID == 1)
  #define RTE_FC7_CTS_SDA_SSEL0_PORT        1
  #define RTE_FC7_CTS_SDA_SSEL0_PIN         5
  #define RTE_FC7_CTS_SDA_SSEL0_FUNC        2
#elif    (RTE_FC7_CTS_SDA_SSEL0_ID == 2)
  #define RTE_FC7_CTS_SDA_SSEL0_PORT        1
  #define RTE_FC7_CTS_SDA_SSEL0_PIN         15
  #define RTE_FC7_CTS_SDA_SSEL0_FUNC        4
#else
  #error "Invalid Flexcomm Interface 7 FC7_CTS_SDA_SSEL0 Pin Configuration!"
#endif

#ifndef   RTE_FC7_CTS_SDA_SSEL0_EN
  #define RTE_FC7_CTS_SDA_SSEL0_EN          1
#endif

//       <o>FC7_RTS_SCL_SSEL1 Pin <0=> Not used <1=> PIO1_4 <2=> PIO1_16
#define   RTE_FC7_RTS_SCL_SSEL1_ID          0
#if      (RTE_FC7_RTS_SCL_SSEL1_ID == 0)
  #define RTE_FC7_RTS_SCL_SSEL1_EN          0
#elif    (RTE_FC7_RTS_SCL_SSEL1_ID == 1)
  #define RTE_FC7_RTS_SCL_SSEL1_PORT        1
  #define RTE_FC7_RTS_SCL_SSEL1_PIN         4
  #define RTE_FC7_RTS_SCL_SSEL1_FUNC        2
#elif    (RTE_FC7_RTS_SCL_SSEL1_ID == 2)
  #define RTE_FC7_RTS_SCL_SSEL1_PORT        1
  #define RTE_FC7_RTS_SCL_SSEL1_PIN         16
  #define RTE_FC7_RTS_SCL_SSEL1_FUNC        4
#else
  #error "Invalid Flexcomm Interface 7 FC7_RTS_SCL_SSEL1 Pin Configuration!"
#endif

#ifndef   RTE_FC7_RTS_SCL_SSEL1_EN
  #define RTE_FC7_RTS_SCL_SSEL1_EN          1
#endif

//       <o>FC7_SSEL2 Pin <0=> Not used <1=> PIO1_3
#define   RTE_FC7_SSEL2_ID                  0
#if      (RTE_FC7_SSEL2_ID == 0)
  #define RTE_FC7_SSEL2_EN                  0
#elif    (RTE_FC7_SSEL2_ID == 1)
  #define RTE_FC7_SSEL2_PORT                1
  #define RTE_FC7_SSEL2_PIN                 3
  #define RTE_FC7_SSEL2_FUNC                2
#else
  #error "Invalid Flexcomm Interface 7 FC7_SSEL2 Pin Configuration!"
#endif

#ifndef   RTE_FC7_SSEL2_EN
  #define RTE_FC7_SSEL2_EN                  1
#endif

//       <o>FC7_SSEL3 Pin <0=> Not used <1=> PIO1_2
#define   RTE_FC7_SSEL3_ID                  0
#if      (RTE_FC7_SSEL3_ID == 0)
  #define RTE_FC7_SSEL3_EN                  0
#elif    (RTE_FC7_SSEL3_ID == 1)
  #define RTE_FC7_SSEL3_PORT                1
  #define RTE_FC7_SSEL3_PIN                 2
  #define RTE_FC7_SSEL3_FUNC                2
#else
  #error "Invalid Flexcomm Interface 7 FC7_SSEL3 Pin Configuration!"
#endif

#ifndef   RTE_FC7_SSEL3_EN
  #define RTE_FC7_SSEL3_EN                  1
#endif
//     </h>

//     <h>DMA Configuration
//       <o>RX / I2C Slave channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC7_DMA_RX_I2CS               0

//       <o>TX / I2C Master channel <0=>Disable <1=>Enable
//       <i>When using DMA data transfer mode, make sure that application calls function Chip_DMASERVICE_Isr to handle DMA IRQ requests.
//       <i>Two possible ways are as follows:
//       <i>a.) Replace all occurences of DMA_IRQHandler in device startup code with Chip_DMASERVICE_Isr
//       <i>b.) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr
#define   RTE_FC7_DMA_TX_I2CM               0
//     </h>
//   </e>
// </h>

// <e> USB Controller [Driver_USBD0]
// <i> Configuration settings for Driver_USBD0 in component ::Drivers:USB Device
#define   RTE_USB_USB                       0

//   <h> Pin Configuration
//     <o> USB_VBUS <0=>P1_6 <1=>P1_11
#define   RTE_USB_VBUS_ID                   0
#if      (RTE_USB_VBUS_ID == 0)
  #define RTE_USB_VBUS_PORT                 1
  #define RTE_USB_VBUS_BIT                  6
  #define RTE_USB_VBUS_FUNC                 7
#elif    (RTE_USB_VBUS_ID == 1)
  #define RTE_USB_VBUS_PORT                 1
  #define RTE_USB_VBUS_BIT                  11
  #define RTE_USB_VBUS_FUNC                 7
#else
  #error "Invalid USB_VBUS Pin Configuration in RTE_Device.h file!"
#endif

//     <o> USB_UP_LED <0=>Not used <1=>P1_3 <2=>P1_9
//     <i> GoodLink LED control signal.
#define   RTE_USB_UP_LED_ID                 0
#if      (RTE_USB_UP_LED_ID == 0)
  #define RTE_USB_UP_LED_PIN_EN             0
#elif    (RTE_USB_UP_LED_ID == 1)
  #define RTE_USB_UP_LED_PIN_EN             1
  #define RTE_USB_UP_LED_PORT               1
  #define RTE_USB_UP_LED_BIT                3
  #define RTE_USB_UP_LED_FUNC               1
#elif    (RTE_USB_UP_LED_ID == 2)
  #define RTE_USB_UP_LED_PIN_EN             1
  #define RTE_USB_UP_LED_PORT               1
  #define RTE_USB_UP_LED_BIT                9
  #define RTE_USB_UP_LED_FUNC               6
#else
  #error "Invalid USB_UP_LED Pin Configuration in RTE_Device.h file!"
#endif

//   </h> Pin Configuration
// </e> USB0 Controller [Driver_USBD0]

#endif  /* __RTE_DEVICE_H */
