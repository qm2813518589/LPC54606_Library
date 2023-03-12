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
 * Project:      RTE Device Configuration for NXP LPC5410x
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H


// <e> USART0 (Universal synchronous asynchronous receiver transmitter)
#define RTE_USART0                      0

//   <o> U0_TXD Pin <0=>PIO0_1 <1=>PIO0_21
//   <i> USART0 Serial Output pin
#define RTE_USART0_TXD_ID               0
#if    (RTE_USART0_TXD_ID == 0)
#define RTE_USART0_TXD_PORT             0
#define RTE_USART0_TXD_BIT              1
#define RTE_USART0_TXD_FUNC             1
#elif  (RTE_USART0_TXD_ID == 1)
#define RTE_USART0_TXD_PORT             0
#define RTE_USART0_TXD_BIT              21
#define RTE_USART0_TXD_FUNC             2
#else
#error "Invalid USART0_TX Pin Configuration!"
#endif

//   <o> U0_RXD Pin <0=>PIO0_0 <1=>PIO0_22
//   <i> USART0 Serial Input pin
#define RTE_USART0_RXD_ID               0
#if    (RTE_USART0_RXD_ID == 0)
#define RTE_USART0_RXD_PORT             0
#define RTE_USART0_RXD_BIT              0
#define RTE_USART0_RXD_FUNC             1
#elif  (RTE_USART0_RXD_ID == 1)
#define RTE_USART0_RXD_PORT             0
#define RTE_USART0_RXD_BIT              22
#define RTE_USART0_RXD_FUNC             2
#else
#error "Invalid USART0_RX Pin Configuration!"
#endif

//     <o> U0_SCLK Pin <0=>Not used <1=>PIO0_4 <2=>PIO0_20
#define RTE_USART0_SCLK_ID              0
#if    (RTE_USART0_SCLK_ID == 0)
#define RTE_USART0_SCLK_PIN_EN          0
#elif  (RTE_USART0_SCLK_ID == 1)
#define RTE_USART0_SCLK_PORT            0
#define RTE_USART0_SCLK_BIT             4
#define RTE_USART0_SCLK_FUNC            1
#define RTE_USART0_SCLK_PIN_EN          1
#elif  (RTE_USART0_SCLK_ID == 2)
#define RTE_USART0_SCLK_PORT            0
#define RTE_USART0_SCLK_BIT             20
#define RTE_USART0_SCLK_FUNC            2
#define RTE_USART0_SCLK_PIN_EN          1
#else
#error "Invalid USART0_SCLK Pin Configuration!"
#endif

//   <h> Modem Lines
//     <o> U0_CTS Pin <0=>Not used <1=>PIO0_2
#define RTE_USART0_CTS_ID               0
#if    (RTE_USART0_CTS_ID == 0)
#define RTE_USART0_CTS_PIN_EN           0
#elif  (RTE_USART0_CTS_ID == 1)
#define RTE_USART0_CTS_PORT             0
#define RTE_USART0_CTS_BIT              2
#define RTE_USART0_CTS_FUNC             1
#define RTE_USART0_CTS_PIN_EN           1
#else
#error "Invalid USART0_CTS Pin Configuration!"
#endif

//     <o> U0_RTS Pin <0=>Not used <1=>PIO0_3
#define RTE_USART0_RTS_ID               0
#if    (RTE_USART0_RTS_ID == 0)
#define RTE_USART0_RTS_PIN_EN           0
#elif  (RTE_USART0_RTS_ID == 1)
#define RTE_USART0_RTS_PORT             0
#define RTE_USART0_RTS_BIT              3
#define RTE_USART0_RTS_FUNC             1
#define RTE_USART0_RTS_PIN_EN           1
#else
#error "Invalid USART0_RTS Pin Configuration!"
#endif

//   </h>
// </e>

// <e> USART1 (Universal synchronous asynchronous receiver transmitter)
#define RTE_USART1                      0

//   <o> U1_TXD Pin <0=>PIO0_6 <1=>PIO0_12 <2=>PIO1_10
//   <i> USART1 Serial Output pin
#define RTE_USART1_TXD_ID               0
#if    (RTE_USART1_TXD_ID == 0)
#define RTE_USART1_TXD_PORT             0
#define RTE_USART1_TXD_BIT              6
#define RTE_USART1_TXD_FUNC             1
#elif  (RTE_USART1_TXD_ID == 1)
#define RTE_USART1_TXD_PORT             0
#define RTE_USART1_TXD_BIT              12
#define RTE_USART1_TXD_FUNC             2
#elif  (RTE_USART1_TXD_ID == 2)
#define RTE_USART1_TXD_PORT             1
#define RTE_USART1_TXD_BIT              10
#define RTE_USART1_TXD_FUNC             2
#else
#error "Invalid USART1_TX Pin Configuration!"
#endif

//   <o> U1_RXD Pin <0=>PIO0_5 <1=>PIO0_11
//   <i> USART1 Serial Input pin
#define RTE_USART1_RXD_ID               0
#if    (RTE_USART1_RXD_ID == 0)
#define RTE_USART1_RXD_PORT             0
#define RTE_USART1_RXD_BIT              5
#define RTE_USART1_RXD_FUNC             1
#elif  (RTE_USART1_RXD_ID == 1)
#define RTE_USART1_RXD_PORT             0
#define RTE_USART1_RXD_BIT              11
#define RTE_USART1_RXD_FUNC             2
#else
#error "Invalid USART1_RX Pin Configuration!"
#endif

//     <o> U1_SCLK Pin <0=>Not used <1=>PIO0_7
#define RTE_USART1_SCLK_ID              0
#if    (RTE_USART1_SCLK_ID == 0)
#define RTE_USART1_SCLK_PIN_EN          0
#elif  (RTE_USART1_SCLK_ID == 1)
#define RTE_USART1_SCLK_PORT            0
#define RTE_USART1_SCLK_BIT             7
#define RTE_USART1_SCLK_FUNC            1
#define RTE_USART1_SCLK_PIN_EN          1
#else
#error "Invalid USART1_SCLK Pin Configuration!"
#endif

//   <h> Modem Lines
//     <o> U1_CTS Pin <0=>Not used <1=>PIO0_16 <2=>PIO0_25
#define RTE_USART1_CTS_ID               0
#if    (RTE_USART1_CTS_ID == 0)
#define RTE_USART1_CTS_PIN_EN           0
#elif  (RTE_USART1_CTS_ID == 1)
#define RTE_USART1_CTS_PORT             0
#define RTE_USART1_CTS_BIT              16
#define RTE_USART1_CTS_FUNC             2
#define RTE_USART1_CTS_PIN_EN           1
#elif  (RTE_USART1_CTS_ID == 2)
#define RTE_USART1_CTS_PORT             0
#define RTE_USART1_CTS_BIT              25
#define RTE_USART1_CTS_FUNC             2
#define RTE_USART1_CTS_PIN_EN           1
#else
#error "Invalid USART1_CTS Pin Configuration!"
#endif

//     <o> U1_RTS Pin <0=>Not used <1=>PIO0_17 <2=>PIO1_11
#define RTE_USART1_RTS_ID               0
#if    (RTE_USART1_RTS_ID == 0)
#define RTE_USART1_RTS_PIN_EN           0
#elif  (RTE_USART1_RTS_ID == 1)
#define RTE_USART1_RTS_PORT             0
#define RTE_USART1_RTS_BIT              17
#define RTE_USART1_RTS_FUNC             2
#define RTE_USART1_RTS_PIN_EN           1
#elif  (RTE_USART1_RTS_ID == 2)
#define RTE_USART1_RTS_PORT             1
#define RTE_USART1_RTS_BIT              11
#define RTE_USART1_RTS_FUNC             2
#define RTE_USART1_RTS_PIN_EN           1
#else
#error "Invalid USART1_RTS Pin Configuration!"
#endif

//   </h>
// </e>

// <e> USART2 (Universal synchronous asynchronous receiver transmitter)
#define RTE_USART2                      0

//   <o> U2_TXD Pin <0=>PIO0_9
//   <i> USART2 Serial Output pin
#define RTE_USART2_TXD_ID               0
#if    (RTE_USART2_TXD_ID == 0)
#define RTE_USART2_TXD_PORT             0
#define RTE_USART2_TXD_BIT              9
#define RTE_USART2_TXD_FUNC             1
#else
#error "Invalid USART2_TX Pin Configuration!"
#endif

//   <o> U2_RXD Pin <0=>PIO0_8 <1=>PIO1_14
//   <i> USART2 Serial Input pin
#define RTE_USART2_RXD_ID               0
#if    (RTE_USART2_RXD_ID == 0)
#define RTE_USART2_RXD_PORT             0
#define RTE_USART2_RXD_BIT              8
#define RTE_USART2_RXD_FUNC             1
#elif  (RTE_USART2_RXD_ID == 1)
#define RTE_USART2_RXD_PORT             1
#define RTE_USART2_RXD_BIT              14
#define RTE_USART2_RXD_FUNC             2
#else
#error "Invalid USART2_RX Pin Configuration!"
#endif

//     <o> U2_SCLK Pin <0=>Not used <1=>PIO0_10
#define RTE_USART2_SCLK_ID              0
#if    (RTE_USART2_SCLK_ID == 0)
#define RTE_USART2_SCLK_PIN_EN          0
#elif  (RTE_USART2_SCLK_ID == 1)
#define RTE_USART2_SCLK_PORT            0
#define RTE_USART2_SCLK_BIT             10
#define RTE_USART2_SCLK_FUNC            1
#define RTE_USART2_SCLK_PIN_EN          1
#else
#error "Invalid USART2_SCLK Pin Configuration!"
#endif

//   <h> Modem Lines
//     <o> U2_CTS Pin <0=>Not used <1=>PIO0_31
#define RTE_USART2_CTS_ID               0
#if    (RTE_USART2_CTS_ID == 0)
#define RTE_USART2_CTS_PIN_EN           0
#elif  (RTE_USART2_CTS_ID == 1)
#define RTE_USART2_CTS_PORT             0
#define RTE_USART2_CTS_BIT              31
#define RTE_USART2_CTS_FUNC             2
#define RTE_USART2_CTS_PIN_EN           1
#else
#error "Invalid USART2_CTS Pin Configuration!"
#endif

//     <o> U2_RTS Pin <0=>Not used <1=>PIO1_0
#define RTE_USART2_RTS_ID               0
#if    (RTE_USART2_RTS_ID == 0)
#define RTE_USART2_RTS_PIN_EN           0
#elif  (RTE_USART2_RTS_ID == 1)
#define RTE_USART2_RTS_PORT             1
#define RTE_USART2_RTS_BIT              0
#define RTE_USART2_RTS_FUNC             2
#define RTE_USART2_RTS_PIN_EN           1
#else
#error "Invalid USART2_RTS Pin Configuration!"
#endif

//   </h>
// </e>

// <e> USART3 (Universal synchronous asynchronous receiver transmitter)
#define RTE_USART3                      0

//   <o> U3_TXD Pin <0=>PIO0_18 <1=>PIO1_13
//   <i> USART3 Serial Output pin
#define RTE_USART3_TXD_ID               0
#if    (RTE_USART3_TXD_ID == 0)
#define RTE_USART3_TXD_PORT             0
#define RTE_USART3_TXD_BIT              18
#define RTE_USART3_TXD_FUNC             1
#elif  (RTE_USART3_TXD_ID == 1)
#define RTE_USART3_TXD_PORT             1
#define RTE_USART3_TXD_BIT              13
#define RTE_USART3_TXD_FUNC             2
#else
#error "Invalid USART3_TX Pin Configuration!"
#endif

//   <o> U3_RXD Pin <0=>PIO0_20 <1=>PIO1_12
//   <i> USART3 Serial Input pin
#define RTE_USART3_RXD_ID               0
#if    (RTE_USART3_RXD_ID == 0)
#define RTE_USART3_RXD_PORT             0
#define RTE_USART3_RXD_BIT              20
#define RTE_USART3_RXD_FUNC             1
#elif  (RTE_USART3_RXD_ID == 1)
#define RTE_USART3_RXD_PORT             1
#define RTE_USART3_RXD_BIT              12
#define RTE_USART3_RXD_FUNC             2
#else
#error "Invalid USART3_RX Pin Configuration!"
#endif

//     <o> U3_SCLK Pin <0=>Not used <1=>PIO0_19
#define RTE_USART3_SCLK_ID              0
#if    (RTE_USART3_SCLK_ID == 0)
#define RTE_USART3_SCLK_PIN_EN          0
#elif  (RTE_USART3_SCLK_ID == 1)
#define RTE_USART3_SCLK_PORT            0
#define RTE_USART3_SCLK_BIT             19
#define RTE_USART3_SCLK_FUNC            1
#define RTE_USART3_SCLK_PIN_EN          1
#else
#error "Invalid USART3_SCLK Pin Configuration!"
#endif

// </e>


// <e> SPI0 (Serial Peripheral Interface 0) [Driver_SPI0]
// <i> Configuration settings for Driver_SPI0 in component ::Drivers:SPI
#define RTE_SPI0                        0

//   <o> SPI0_SSEL Pin <0=>Not used <1=>PIO0_0  <2=>PIO0_1  <3=>PIO0_4  <4=>PIO0_9
//                                  <5=>PIO0_14 <6=>PIO0_15 <7=>PIO0_16 <8=>PIO0_17
//   <i> Slave Select pin for SPI0
#define   RTE_SPI0_SSEL_PIN_SEL         5
#if      (RTE_SPI0_SSEL_PIN_SEL == 0)
#define   RTE_SPI0_SSEL_PIN_EN          0
#elif    (RTE_SPI0_SSEL_PIN_SEL == 1)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             0
  #define RTE_SPI0_SSEL_FUNC            2
  #define RTE_SPI0_SSEL_NUM             0
#elif    (RTE_SPI0_SSEL_PIN_SEL == 2)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             1
  #define RTE_SPI0_SSEL_FUNC            2
  #define RTE_SPI0_SSEL_NUM             1
#elif    (RTE_SPI0_SSEL_PIN_SEL == 3)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             4
  #define RTE_SPI0_SSEL_FUNC            2
  #define RTE_SPI0_SSEL_NUM             2
#elif    (RTE_SPI0_SSEL_PIN_SEL == 4)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             9
  #define RTE_SPI0_SSEL_FUNC            5
  #define RTE_SPI0_SSEL_NUM             0
#elif    (RTE_SPI0_SSEL_PIN_SEL == 5)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             14
  #define RTE_SPI0_SSEL_FUNC            1
  #define RTE_SPI0_SSEL_NUM             0
#elif    (RTE_SPI0_SSEL_PIN_SEL == 6)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             15
  #define RTE_SPI0_SSEL_FUNC            1
  #define RTE_SPI0_SSEL_NUM             1
#elif    (RTE_SPI0_SSEL_PIN_SEL == 7)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             16
  #define RTE_SPI0_SSEL_FUNC            1
  #define RTE_SPI0_SSEL_NUM             2
#elif    (RTE_SPI0_SSEL_PIN_SEL == 4)
  #define RTE_SPI0_SSEL_PORT            0
  #define RTE_SPI0_SSEL_PIN             17
  #define RTE_SPI0_SSEL_FUNC            1
  #define RTE_SPI0_SSEL_NUM             3
#else
  #error "Invalid SPI0 SPI0_SSEL Pin Configuration!"
#endif
#ifndef   RTE_SPI0_SSEL_PIN_EN
#define   RTE_SPI0_SSEL_PIN_EN          1
#endif

//   <o> SPI0_SCK Pin <0=>PIO0_11 <1=>PIO1_3
//   <i> Serial clock pin for SPI0
#define   RTE_SPI0_SCK_PIN_SEL          0
#if      (RTE_SPI0_SCK_PIN_SEL == 0)
  #define RTE_SPI0_SCK_PORT             0
  #define RTE_SPI0_SCK_PIN              11
  #define RTE_SPI0_SCK_FUNC             1
#elif    (RTE_SPI0_SCK_PIN_SEL == 1)
  #define RTE_SPI0_SCK_PORT             1
  #define RTE_SPI0_SCK_PIN              3
  #define RTE_SPI0_SCK_FUNC             5
#else
  #error "Invalid SPI0 SPI0_SCK Pin Configuration!"
#endif

//   <o> SPI0_MOSI Pin <0=>PIO0_12 <1=>PIO1_9
//   <i> Master Out Slave In for SSP0
#define   RTE_SPI0_MOSI_PIN_SEL         0
#if      (RTE_SPI0_MOSI_PIN_SEL == 0)
  #define RTE_SPI0_MOSI_PORT            0
  #define RTE_SPI0_MOSI_PIN             12
  #define RTE_SPI0_MOSI_FUNC            1
#elif    (RTE_SPI0_MOSI_PIN_SEL == 1)
  #define RTE_SPI0_MOSI_PORT            1
  #define RTE_SPI0_MOSI_PIN             9
  #define RTE_SPI0_MOSI_FUNC            2
#else
  #error "Invalid SPI0 SPI0_MOSI Pin Configuration!"
#endif

//   <o> SPI0_MISO Pin <0=>PIO0_13 <1=>PIO1_4
//   <i> Master In Slave Out pin for SPI0
#define   RTE_SPI0_MISO_PIN_SEL         0
#if      (RTE_SPI0_MISO_PIN_SEL == 0)
  #define RTE_SPI0_MISO_PORT            0
  #define RTE_SPI0_MISO_PIN             13
  #define RTE_SPI0_MISO_FUNC            1
#elif    (RTE_SPI0_MISO_PIN_SEL == 1)
  #define RTE_SPI0_MISO_PORT            1
  #define RTE_SPI0_MISO_PIN             4
  #define RTE_SPI0_MISO_FUNC            5
#else
  #error "Invalid SPI0 SPI0_MISO Pin Configuration!"
#endif

// </e>


// <e> SPI1 (Serial Peripheral Interface 1) [Driver_SPI1]
// <i> Configuration settings for Driver_SPI1 in component ::Drivers:SPI
#define RTE_SPI1                        0

//   <o> SPI1_SSEL Pin <0=>Not used <1=>PIO1_2  <2=>PIO1_3  <3=>PIO1_4  <4=>PIO1_5
//                                  <5=>PIO1_15 <6=>PIO1_16
//   <i> Slave Select pin for SPI1
#define   RTE_SPI1_SSEL_PIN_SEL         4

#if      (RTE_SPI1_SSEL_PIN_SEL == 0)
#define   RTE_SPI1_SSEL_PIN_EN          0
#elif    (RTE_SPI1_SSEL_PIN_SEL == 1)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             2
  #define RTE_SPI1_SSEL_FUNC            2
  #define RTE_SPI1_SSEL_NUM             3
#elif    (RTE_SPI1_SSEL_PIN_SEL == 2)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             3
  #define RTE_SPI1_SSEL_FUNC            2
  #define RTE_SPI1_SSEL_NUM             2
#elif    (RTE_SPI1_SSEL_PIN_SEL == 3)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             4
  #define RTE_SPI1_SSEL_FUNC            2
  #define RTE_SPI1_SSEL_NUM             1
#elif    (RTE_SPI1_SSEL_PIN_SEL == 4)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             5
  #define RTE_SPI1_SSEL_FUNC            2
  #define RTE_SPI1_SSEL_NUM             0
#elif    (RTE_SPI1_SSEL_PIN_SEL == 5)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             15
  #define RTE_SPI1_SSEL_FUNC            4
  #define RTE_SPI1_SSEL_NUM             0
#elif    (RTE_SPI1_SSEL_PIN_SEL == 6)
  #define RTE_SPI1_SSEL_PORT            1
  #define RTE_SPI1_SSEL_PIN             16
  #define RTE_SPI1_SSEL_FUNC            4
  #define RTE_SPI1_SSEL_NUM             1
#else
  #error "Invalid SPI1 SPI1_SSEL Pin Configuration!"
#endif
#ifndef   RTE_SPI1_SSEL_PIN_EN
#define   RTE_SPI1_SSEL_PIN_EN          1
#endif

//   <o> SPI1_SCK Pin <0=>PIO1_6 <1=>PIO1_12
//   <i> Serial clock pin for SPI1
#define   RTE_SPI1_SCK_PIN_SEL          0
#if      (RTE_SPI1_SCK_PIN_SEL == 0)
  #define RTE_SPI1_SCK_PORT             1
  #define RTE_SPI1_SCK_PIN              6
  #define RTE_SPI1_SCK_FUNC             2
#elif    (RTE_SPI1_SCK_PIN_SEL == 1)
  #define RTE_SPI1_SCK_PORT             1
  #define RTE_SPI1_SCK_PIN              12
  #define RTE_SPI1_SCK_FUNC             4
#else
  #error "Invalid SPI1 SPI1_SCK Pin Configuration!"
#endif

//   <o> SPI1_MOSI Pin <0=>PIO1_7 <1=>PIO1_13
//   <i> Master Out Slave In for SSP1
#define   RTE_SPI1_MOSI_PIN_SEL         0
#if      (RTE_SPI1_MOSI_PIN_SEL == 0)
  #define RTE_SPI1_MOSI_PORT            1
  #define RTE_SPI1_MOSI_PIN             7
  #define RTE_SPI1_MOSI_FUNC            2
#elif    (RTE_SPI1_MOSI_PIN_SEL == 1)
  #define RTE_SPI1_MOSI_PORT            1
  #define RTE_SPI1_MOSI_PIN             13
  #define RTE_SPI1_MOSI_FUNC            4
#else
  #error "Invalid SPI1 SPI1_MOSI Pin Configuration!"
#endif

//   <o> SPI1_MISO Pin <0=>PIO1_8 <1=>PIO1_14
//   <i> Master In Slave Out pin for SPI1
#define   RTE_SPI1_MISO_PIN_SEL         0
#if      (RTE_SPI1_MISO_PIN_SEL == 0)
  #define RTE_SPI1_MISO_PORT            1
  #define RTE_SPI1_MISO_PIN             8
  #define RTE_SPI1_MISO_FUNC            2
#elif    (RTE_SPI1_MISO_PIN_SEL == 1)
  #define RTE_SPI1_MISO_PORT            1
  #define RTE_SPI1_MISO_PIN             14
  #define RTE_SPI1_MISO_FUNC            4
#else
  #error "Invalid SPI1 SPI1_MISO Pin Configuration!"
#endif

// </e>


// <e> I2C0 (Inter-integrated Circuit Interface 0) [Driver_I2C0]
// <i> Configuration settings for Driver_I2C0 in component ::Drivers:I2C
#define RTE_I2C0                        0

//   <o> I2C0_SCL Pin <0=>PIO0_23
#define RTE_I2C0_SCL_PORT_ID            0
#if    (RTE_I2C0_SCL_PORT_ID == 0)
#define RTE_I2C0_SCL_PORT               0
#define RTE_I2C0_SCL_PIN                23
#define RTE_I2C0_SCL_FUNC               1
#else
#error "Invalid I2C0_SCL Pin Configuration!"
#endif

//   <o> I2C0_SDA Pin <0=>PIO0_24
#define RTE_I2C0_SDA_PORT_ID            0
#if    (RTE_I2C0_SDA_PORT_ID == 0)
#define RTE_I2C0_SDA_PORT               0
#define RTE_I2C0_SDA_PIN                24
#define RTE_I2C0_SDA_FUNC               1
#else
#error "Invalid I2C0_SDA Pin Configuration!"
#endif

// </e>


// <e> I2C1 (Inter-integrated Circuit Interface 1) [Driver_I2C1]
// <i> Configuration settings for Driver_I2C1 in component ::Drivers:I2C
#define RTE_I2C1                        0

//   <o> I2C1_SCL Pin <0=>PIO0_25
#define RTE_I2C1_SCL_PORT_ID            0
#if    (RTE_I2C1_SCL_PORT_ID == 0)
#define RTE_I2C1_SCL_PORT               0
#define RTE_I2C1_SCL_PIN                25
#define RTE_I2C1_SCL_FUNC               1
#else
#error "Invalid I2C1_SCL Pin Configuration!"
#endif

//   <o> I2C1_SDA Pin <0=>PIO0_26
#define RTE_I2C1_SDA_PORT_ID            0
#if    (RTE_I2C1_SDA_PORT_ID == 0)
#define RTE_I2C1_SDA_PORT               0
#define RTE_I2C1_SDA_PIN                26
#define RTE_I2C1_SDA_FUNC               1
#else
#error "Invalid I2C1_SDA Pin Configuration!"
#endif

// </e>


// <e> I2C2 (Inter-integrated Circuit Interface 2) [Driver_I2C2]
// <i> Configuration settings for Driver_I2C2 in component ::Drivers:I2C
#define RTE_I2C2                        0

//   <o> I2C2_SCL Pin <0=>PIO0_27
#define RTE_I2C2_SCL_PORT_ID            0
#if    (RTE_I2C2_SCL_PORT_ID == 0)
#define RTE_I2C2_SCL_PORT               0
#define RTE_I2C2_SCL_PIN                27
#define RTE_I2C2_SCL_FUNC               1
#else
#error "Invalid I2C2_SCL Pin Configuration!"
#endif

//   <o> I2C2_SDA Pin <0=>PIO0_28
#define RTE_I2C2_SDA_PORT_ID            0
#if    (RTE_I2C2_SDA_PORT_ID == 0)
#define RTE_I2C2_SDA_PORT               0
#define RTE_I2C2_SDA_PIN                28
#define RTE_I2C2_SDA_FUNC               1
#else
#error "Invalid I2C2_SDA Pin Configuration!"
#endif

// </e>

#endif  /* __RTE_DEVICE_H */
