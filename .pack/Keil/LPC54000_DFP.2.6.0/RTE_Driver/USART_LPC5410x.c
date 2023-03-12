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
 * $Date:        1. February 2016
 * $Revision:    V1.0
 *
 * Driver:       Driver_USART0, Driver_USART1, Driver_USART2, Driver_USART3
 * Configured:   via RTE_Device.h configuration file
 * Project:      USART Driver for NXP LPC5410x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                   Value   UART Interface
 *   ---------------------                   -----   --------------
 *   Connect to hardware via Driver_USART# = 0       use USART0
 *   Connect to hardware via Driver_USART# = 1       use USART1
 *   Connect to hardware via Driver_USART# = 2       use USART2
 *   Connect to hardware via Driver_USART# = 3       use USART3
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    - Initial release
 */
#include "USART_LPC5410x.h"

#include "RTE_Device.h"
#include "RTE_Components.h"

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

#if ((!RTE_USART0) && (!RTE_USART1) && (!RTE_USART2) && (!RTE_USART3))
#error "USART not enabled in RTE_Device.h!"
#endif

// User Defines
// Can be user defined by C preprocessor


// USART Fractional divider
// Note:
//  - USART fractional divider is common for all USARTs.
//  - Changing fractional divider effects all USART baudrate settings
// USART clock  = async bridge clk / (1 + USARTx_FRGCTRL_MUL / 255)
// USARTx_FRGCTRL_MUL: 0 - 255: user defined mul values
//                     256    : fractional divider will be calculated,
//                              all USARTx baurates will be effected
#ifndef USARTx_FRGCTRL_MUL
#define USARTx_FRGCTRL_MUL        256
#endif

// Oversample selection
// USARTx_OVERSAMPLE: 5 - 16
#ifndef USART0_OVERSAMPLE
#define USART0_OVERSAMPLE         16
#endif
#ifndef USART1_OVERSAMPLE
#define USART1_OVERSAMPLE         16
#endif
#ifndef USART2_OVERSAMPLE
#define USART2_OVERSAMPLE         16
#endif
#ifndef USART3_OVERSAMPLE
#define USART3_OVERSAMPLE         16
#endif

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };

// USART0
#if (RTE_USART0)
static USART_INFO USART0_Info = { 0 };
static PINMUX_GRP_T USART0_pin_tx  = { RTE_USART0_TXD_PORT,   RTE_USART0_TXD_BIT,   RTE_USART0_TXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
static PINMUX_GRP_T USART0_pin_rx  = { RTE_USART0_RXD_PORT,   RTE_USART0_RXD_BIT,   RTE_USART0_RXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#if (RTE_USART0_SCLK_PIN_EN == 1)
static PINMUX_GRP_T USART0_pin_clk = { RTE_USART0_SCLK_PORT,  RTE_USART0_SCLK_BIT,  RTE_USART0_SCLK_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART0_CTS_PIN_EN == 1)
static PINMUX_GRP_T USART0_pin_cts = { RTE_USART0_CTS_PORT,   RTE_USART0_CTS_BIT,   RTE_USART0_CTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART0_RTS_PIN_EN == 1)
static PINMUX_GRP_T USART0_pin_rts = { RTE_USART0_RTS_PORT,   RTE_USART0_RTS_BIT,   RTE_USART0_RTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif

static const USART_RESOURCES USART0_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#if (RTE_USART0_SCLK_PIN_EN == 1)
    1,  // supports Synchronous Master mode
    1,  // supports Synchronous Slave mode
#else
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
#endif
    0,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#if (RTE_USART0_RTS_PIN_EN == 1)
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#if (RTE_USART0_RTS_PIN_EN == 1)
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#if (RTE_USART0_RTS_PIN_EN == 1)
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#if (RTE_USART0_CTS_PIN_EN == 1)
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#if (RTE_USART0_CTS_PIN_EN == 1)
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },
    LPC_USART0,
  {     // USART Pin Configuration
    &USART0_pin_tx,
    &USART0_pin_rx,
#if (RTE_USART0_SCLK_PIN_EN == 1)
    &USART0_pin_clk,
#else
    NULL,
#endif
#if (RTE_USART0_CTS_PIN_EN == 1)
    &USART0_pin_cts,
#else
    NULL,
#endif
#if (RTE_USART0_RTS_PIN_EN == 1)
    &USART0_pin_rts,
#else
    NULL,
#endif
  },
    RESET_USART0,
    USART0_OVERSAMPLE,
    UART0_IRQn,
    &USART0_Info,
};
#endif

// USART1
#if (RTE_USART1)
static USART_INFO USART1_Info = { 0 };
static PINMUX_GRP_T USART1_pin_tx  = { RTE_USART1_TXD_PORT,   RTE_USART1_TXD_BIT,   RTE_USART1_TXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
static PINMUX_GRP_T USART1_pin_rx  = { RTE_USART1_RXD_PORT,   RTE_USART1_RXD_BIT,   RTE_USART1_RXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#if (RTE_USART1_SCLK_PIN_EN == 1)
static PINMUX_GRP_T USART1_pin_clk = { RTE_USART1_SCLK_PORT,  RTE_USART1_SCLK_BIT,  RTE_USART1_SCLK_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART1_CTS_PIN_EN == 1)
static PINMUX_GRP_T USART1_pin_cts = { RTE_USART1_CTS_PORT,   RTE_USART1_CTS_BIT,   RTE_USART1_CTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART1_RTS_PIN_EN == 1)
static PINMUX_GRP_T USART1_pin_rts = { RTE_USART1_RTS_PORT,   RTE_USART1_RTS_BIT,   RTE_USART1_RTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif

static const USART_RESOURCES USART1_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#if (RTE_USART1_SCLK_PIN_EN == 1)
    1,  // supports Synchronous Master mode
    1,  // supports Synchronous Slave mode
#else
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
#endif
    0,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#if (RTE_USART1_RTS_PIN_EN == 1)
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#if (RTE_USART1_RTS_PIN_EN == 1)
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#if (RTE_USART1_RTS_PIN_EN == 1)
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#if (RTE_USART1_CTS_PIN_EN == 1)
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#if (RTE_USART1_CTS_PIN_EN == 1)
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },
    LPC_USART1,
  {     // USART Pin Configuration
    &USART1_pin_tx,
    &USART1_pin_rx,
#if (RTE_USART1_SCLK_PIN_EN == 1)
    &USART1_pin_clk,
#else
    NULL,
#endif
#if (RTE_USART1_CTS_PIN_EN == 1)
    &USART1_pin_cts,
#else
    NULL,
#endif
#if (RTE_USART1_RTS_PIN_EN == 1)
    &USART1_pin_rts,
#else
    NULL,
#endif
  },
    RESET_USART1,
    USART1_OVERSAMPLE,
    UART1_IRQn,
    &USART1_Info,
};
#endif

// USART2 
#if (RTE_USART2)
static USART_INFO USART2_Info = { 0 };
static PINMUX_GRP_T USART2_pin_tx  = { RTE_USART2_TXD_PORT,   RTE_USART2_TXD_BIT,   RTE_USART2_TXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
static PINMUX_GRP_T USART2_pin_rx  = { RTE_USART2_RXD_PORT,   RTE_USART2_RXD_BIT,   RTE_USART2_RXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#if (RTE_USART2_SCLK_PIN_EN == 1)
static PINMUX_GRP_T USART2_pin_clk = { RTE_USART2_SCLK_PORT,  RTE_USART2_SCLK_BIT,  RTE_USART2_SCLK_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART2_CTS_PIN_EN == 1)
static PINMUX_GRP_T USART2_pin_cts = { RTE_USART2_CTS_PORT,   RTE_USART2_CTS_BIT,   RTE_USART2_CTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART2_RTS_PIN_EN == 1)
static PINMUX_GRP_T USART2_pin_rts = { RTE_USART2_RTS_PORT,   RTE_USART2_RTS_BIT,   RTE_USART2_RTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif

static const USART_RESOURCES USART2_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#if (RTE_USART2_SCLK_PIN_EN == 1)
    1,  // supports Synchronous Master mode
    1,  // supports Synchronous Slave mode
#else
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
#endif
    0,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#if (RTE_USART2_RTS_PIN_EN == 1)
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#if (RTE_USART2_RTS_PIN_EN == 1)
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#if (RTE_USART2_RTS_PIN_EN == 1)
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#if (RTE_USART2_CTS_PIN_EN == 1)
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#if (RTE_USART2_CTS_PIN_EN == 1)
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },
    LPC_USART2,
  {     // USART Pin Configuration
    &USART2_pin_tx,
    &USART2_pin_rx,
#if (RTE_USART2_SCLK_PIN_EN == 1)
    &USART2_pin_clk,
#else
    NULL,
#endif
#if (RTE_USART2_CTS_PIN_EN == 1)
    &USART2_pin_cts,
#else
    NULL,
#endif
#if (RTE_USART2_RTS_PIN_EN == 1)
    &USART2_pin_rts,
#else
    NULL,
#endif
  },
    RESET_USART2,
    USART2_OVERSAMPLE,
    UART2_IRQn,
    &USART2_Info,
};
#endif

// USART3
#if (RTE_USART3)
static USART_INFO USART3_Info = { 0 };
static PINMUX_GRP_T USART3_pin_tx  = { RTE_USART3_TXD_PORT,   RTE_USART3_TXD_BIT,   RTE_USART3_TXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
static PINMUX_GRP_T USART3_pin_rx  = { RTE_USART3_RXD_PORT,   RTE_USART3_RXD_BIT,   RTE_USART3_RXD_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#if (RTE_USART3_SCLK_PIN_EN == 1)
static PINMUX_GRP_T USART3_pin_clk = { RTE_USART3_SCLK_PORT,  RTE_USART3_SCLK_BIT,  RTE_USART3_SCLK_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART3_CTS_PIN_EN == 1)
static PINMUX_GRP_T USART3_pin_cts = { RTE_USART3_CTS_PORT,   RTE_USART3_CTS_BIT,   RTE_USART3_CTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif
#if (RTE_USART3_RTS_PIN_EN == 1)
static PINMUX_GRP_T USART3_pin_rts = { RTE_USART3_RTS_PORT,   RTE_USART3_RTS_BIT,   RTE_USART3_RTS_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF};
#endif

static const USART_RESOURCES USART3_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
#if (RTE_USART3_SCLK_PIN_EN == 1)
    1,  // supports Synchronous Master mode
    1,  // supports Synchronous Slave mode
#else
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
#endif
    0,  // supports UART Single-wire mode
    1,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
#if (RTE_USART3_RTS_PIN_EN == 1)
    1,  // RTS Flow Control available
#else
    0,  // RTS Flow Control available
#endif
#if (RTE_USART3_RTS_PIN_EN == 1)
    1,  // CTS Flow Control available
#else
    0,  // CTS Flow Control available
#endif
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    0,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
#if (RTE_USART3_RTS_PIN_EN == 1)
    1,  // RTS Line: 0=not available, 1=available
#else
    0,  // RTS Line: 0=not available, 1=available
#endif
#if (RTE_USART3_CTS_PIN_EN == 1)
    1,  // CTS Line: 0=not available, 1=available
#else
    0,  // CTS Line: 0=not available, 1=available
#endif
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
#if (RTE_USART3_CTS_PIN_EN == 1)
    1,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#else
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
#endif
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
  },
    LPC_USART3,
  {     // USART Pin Configuration
    &USART3_pin_tx,
    &USART3_pin_rx,
#if (RTE_USART3_SCLK_PIN_EN == 1)
    &USART3_pin_clk,
#else
    NULL,
#endif
#if (RTE_USART3_CTS_PIN_EN == 1)
    &USART3_pin_cts,
#else
    NULL,
#endif
#if (RTE_USART3_RTS_PIN_EN == 1)
    &USART3_pin_rts,
#else
    NULL,
#endif
  },
    RESET_USART3,
    USART3_OVERSAMPLE,
    UART3_IRQn,
    &USART3_Info,
};
#endif

// Function Prototypes
static int32_t USART_Receive (void            *data,
                              uint32_t         num,
                              USART_RESOURCES *usart);


// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION USARTx_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USARTx_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (USART_RESOURCES *usart)
  \brief       Get driver capabilities
  \param[in]   usart     Pointer to USART resources
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (USART_RESOURCES *usart) {
  return usart->capabilities;
}

/**
  \fn          int32_t USART_Initialize (ARM_USART_SignalEvent_t  cb_event
                                         USART_RESOURCES         *usart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Initialize (ARM_USART_SignalEvent_t  cb_event,
                                 USART_RESOURCES         *usart) {

  if (usart->info->flags & USART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // Initialize USART Run-time Resources
  usart->info->cb_event = cb_event;

  usart->info->rx_status.rx_busy          = 0U;
  usart->info->rx_status.rx_overflow      = 0U;
  usart->info->rx_status.rx_break         = 0U;
  usart->info->rx_status.rx_framing_error = 0U;
  usart->info->rx_status.rx_parity_error  = 0U;

  usart->info->xfer.send_active           = 0U;
  usart->info->xfer.tx_def_val            = 0U;

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

  // Configure TX pin
  if (usart->pins.txd != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.txd->port, usart->pins.txd->pin, usart->pins.txd->modefunc);
  }

  // Configure RX pin
  if (usart->pins.rxd != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rxd->port, usart->pins.rxd->pin, usart->pins.rxd->modefunc);
  }

  // Configure CTS pin
  if (usart->capabilities.cts) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.cts->port, usart->pins.cts->pin, usart->pins.cts->modefunc);
  }

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO0);
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO1);

  if (usart->capabilities.rts) {
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, usart->pins.cts->port, usart->pins.cts->pin);
  }

  usart->info->flags = USART_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (USART_RESOURCES *usart)
  \brief       De-initialize USART Interface.
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (USART_RESOURCES *usart) {

  // Reset TX pin configuration
  if (usart->pins.txd) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.txd->port, usart->pins.txd->pin, 0U);
  }

  // Reset RX pin configuration
  if (usart->pins.rxd) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rxd->port, usart->pins.rxd->pin, 0U);
  }

  // Reset CLK pin configuration
  if (usart->pins.sclk) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.sclk->port, usart->pins.sclk->pin, 0U);
  }

  // Reset CTS pin configuration
  if (usart->capabilities.cts) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.cts->port, usart->pins.cts->pin, 0U);
  }

  // Reset RTS pin configuration
  if (usart->capabilities.rts) {
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, usart->pins.cts->port, usart->pins.cts->pin);
    Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rts->port, usart->pins.rts->pin, 0U);
  }

  // Reset USART status flags
  usart->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (ARM_POWER_STATE  state,
                                   USART_RESOURCES *usart) {

  switch (state) {
    case ARM_POWER_OFF:
      // Disable USART IRQ
      NVIC_DisableIRQ(usart->irq_num);

      // Reset USART peripheral
      Chip_SYSCON_PeriphReset (usart->reset);

      // De-initialize USART peripheral
      Chip_UART_DeInit (usart->reg);

      // Clear pending USART interrupts in NVIC
      NVIC_ClearPendingIRQ(usart->irq_num);

      // Clear driver variables
      usart->info->rx_status.rx_busy          = 0U;
      usart->info->rx_status.rx_overflow      = 0U;
      usart->info->rx_status.rx_break         = 0U;
      usart->info->rx_status.rx_framing_error = 0U;
      usart->info->rx_status.rx_parity_error  = 0U;
      usart->info->xfer.send_active           = 0U;

      usart->info->flags &= ~USART_FLAG_POWERED;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) { return ARM_DRIVER_ERROR; }
      if ((usart->info->flags & USART_FLAG_POWERED)     != 0U) { return ARM_DRIVER_OK; }

      // USART peripheral initialize
      Chip_UART_Init (usart->reg);

      // Disable transmitter
      Chip_UART_TXDisable (usart->reg);


      // Disable interrupts
      Chip_UART_IntDisable (usart->reg, UART_INT_RXRDY    |
                                        UART_INT_TXRDY    |
                                        UART_INT_TXIDLE   |
                                        UART_INT_CTS      |
                                        UART_INT_TXDIS    |
                                        UART_INT_OVR      |
                                        UART_INT_BREAK    |
                                        UART_INT_START    |
                                        UART_INT_FRMERR   |
                                        UART_INT_PARERR   |
                                        UART_INT_RXNOISE  |
                                        UART_INT_ABAUDERR);

      Chip_UART_TXDisable (usart->reg);

      // Enable CTS interrupt
      if (usart->capabilities.cts) {
          Chip_UART_IntEnable (usart->reg, UART_INT_CTS);
      }

      // Clear driver variables
      usart->info->rx_status.rx_busy          = 0U;
      usart->info->rx_status.rx_overflow      = 0U;
      usart->info->rx_status.rx_break         = 0U;
      usart->info->rx_status.rx_framing_error = 0U;
      usart->info->rx_status.rx_parity_error  = 0U;

      usart->info->mode                       = 0U;
      usart->info->flags                      = 0U;
      usart->info->xfer.send_active           = 0U;
      usart->info->xfer.receiver_enabled      = 0U;

      usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

      // Clear and Enable USART IRQ
      NVIC_ClearPendingIRQ(usart->irq_num);
      NVIC_EnableIRQ(usart->irq_num);

      break;

    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void            *data,
                                         uint32_t         num,
                                         USART_RESOURCES *usart)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Send (const void            *data,
                                 uint32_t         num,
                                 USART_RESOURCES *usart) {
  int32_t stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (usart->info->xfer.send_active != 0U) {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Send active flag
  usart->info->xfer.send_active = 1U;

  // Synchronous mode
  if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
      (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {
    if (usart->info->xfer.sync_mode == 0U) {
      usart->info->xfer.sync_mode = USART_SYNC_MODE_TX;
      // Start dummy reads
      stat = USART_Receive (&usart->info->xfer.rx_dump_val, num, usart);
      if (stat != ARM_DRIVER_OK) { return ARM_DRIVER_ERROR; }

    }
  }

  // Save transmit buffer info
  usart->info->xfer.tx_buf = (uint8_t *)data;
  usart->info->xfer.tx_num = num;
  usart->info->xfer.tx_cnt = 0U;

  // Interrupt mode
  // Enable TX Ready interrupt
  Chip_UART_IntEnable (usart->reg, UART_INT_TXRDY);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Receive (void            *data,
                                      uint32_t         num,
                                      USART_RESOURCES *usart)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Receive (void            *data,
                              uint32_t         num,
                              USART_RESOURCES *usart) {

  int32_t stat;

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  // Check if receiver is busy
  if (usart->info->rx_status.rx_busy == 1U) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set RX busy flag
  usart->info->rx_status.rx_busy = 1U;

  // Save number of data to be received
  usart->info->xfer.rx_num = num;

  // Clear RX statuses
  usart->info->rx_status.rx_break          = 0U;
  usart->info->rx_status.rx_framing_error  = 0U;
  usart->info->rx_status.rx_overflow       = 0U;
  usart->info->rx_status.rx_parity_error   = 0U;

  // Save receive buffer info
  usart->info->xfer.rx_buf = (uint8_t *)data;
  usart->info->xfer.rx_cnt =  0U;

  // Interrupt mode
  if (usart->info->xfer.receiver_enabled != 0U) {
    // Enable RX Ready interrupt
    Chip_UART_IntEnable (usart->reg, UART_INT_RXRDY);
  }

  // Synchronous mode
  if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
      (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {
    if (usart->info->xfer.sync_mode == 0U) {
      usart->info->xfer.sync_mode = USART_SYNC_MODE_RX;
      // Send dummy data
      stat = USART_Send (&usart->info->xfer.tx_def_val, num, usart);
      if (stat != ARM_DRIVER_OK) { return ARM_DRIVER_ERROR; }
    }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Transfer (const void             *data_out,
                                             void             *data_in,
                                             uint32_t          num,
                                             USART_RESOURCES  *usart)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void             *data_out,
                                     void             *data_in,
                                     uint32_t          num,
                                     USART_RESOURCES  *usart) {
  int32_t status;

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured
    return ARM_DRIVER_ERROR;
  }

  if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
      (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {

    // Set xfer mode
    usart->info->xfer.sync_mode = USART_SYNC_MODE_TX_RX;

    // Receive
    status = USART_Receive (data_in, num, usart);
    if (status != ARM_DRIVER_OK) { return status; }

    // Send
    status = USART_Send (data_out, num, usart);
    if (status != ARM_DRIVER_OK) { return status; }

  } else {
    // Only in synchronous mode
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USART_GetTxCount (USART_RESOURCES *usart)
  \brief       Get transmitted data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (USART_RESOURCES *usart) {
  uint32_t cnt;

  cnt = usart->info->xfer.tx_cnt;
  return cnt;
}

/**
  \fn          uint32_t USART_GetRxCount (USART_RESOURCES *usart)
  \brief       Get received data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (USART_RESOURCES *usart) {
  uint32_t cnt;

  cnt = usart->info->xfer.rx_cnt;
  return cnt;
}

/**
  \fn          int32_t USART_Control (uint32_t          control,
                                      uint32_t          arg,
                                      USART_RESOURCES  *usart)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   usart    Pointer to USART resources
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (uint32_t          control,
                              uint32_t          arg,
                              USART_RESOURCES  *usart) {
#if (USARTx_FRGCTRL_MUL != 256)
  uint64_t div64;
  uint32_t div;
#endif
  uint32_t i, val, mode;
  uint32_t cfg;

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    // USART not powered
    return ARM_DRIVER_ERROR;
  }

  cfg = 0U;

  switch (control & ARM_USART_CONTROL_Msk) {
    // Control TX
    case ARM_USART_CONTROL_TX:
      // Check if TX line available
      if (usart->pins.txd == NULL) { return ARM_DRIVER_ERROR; }
      if (arg) {

        // Transmitter enable
        Chip_UART_TXEnable (usart->reg);

      } else {

        // Transmitter disable
        Chip_UART_TXDisable (usart->reg);
      }
      return ARM_DRIVER_OK;

    // Control RX
    case ARM_USART_CONTROL_RX:
      if (usart->pins.rxd == NULL) { return ARM_DRIVER_ERROR; }
      if (arg) {
        usart->info->xfer.receiver_enabled = 1U;

        // Enable RX interrupts (overrun, framing, parity error, break)
        val = UART_INT_OVR | UART_INT_FRMERR |  UART_INT_PARERR | UART_INT_BREAK;
        if (usart->info->rx_status.rx_busy != 0U) {
          // Enable RX Ready interrupt
          val |= UART_INT_RXRDY;
        }
        Chip_UART_IntEnable (usart->reg, val);

      } else {
        // Disable RX interrupts (receiver ready, overrun, framing, parity error, break)
        val = UART_INT_RXRDY | UART_INT_OVR | UART_INT_FRMERR |  UART_INT_PARERR | UART_INT_BREAK;
        Chip_UART_IntDisable (usart->reg, val);

        usart->info->xfer.receiver_enabled = 0U;
      }
      return ARM_DRIVER_OK;

    // Control break
    case ARM_USART_CONTROL_BREAK:
      if (arg) {
        if (usart->info->xfer.send_active != 0U) { return ARM_DRIVER_ERROR_BUSY; }

        // Set Send active flag
        usart->info->xfer.send_active = 1U;

        // Continuous break enable
        usart->reg->CTL |= UART_CTRL_TXBRKEN;
      } else {
        // Continuous break disable
        usart->reg->CTL &= ~UART_CTRL_TXBRKEN;

        // Clear Send active flag
        usart->info->xfer.send_active = 0U;
      }
      return ARM_DRIVER_OK;

    // Abort Send
    case ARM_USART_ABORT_SEND:
      // Disable TX Ready and TX Idle interrupts
      Chip_UART_IntDisable (usart->reg, UART_INT_TXRDY | UART_INT_TXIDLE);

      // Clear Send active flag
      usart->info->xfer.send_active = 0U;
      return ARM_DRIVER_OK;

    // Abort receive
    case ARM_USART_ABORT_RECEIVE:
      // Disable RX Ready interrupt
      Chip_UART_IntEnable (usart->reg, UART_INT_RXRDY);

      // Clear RX busy status
      usart->info->rx_status.rx_busy = 0U;
      return ARM_DRIVER_OK;

    // Abort transfer
    case ARM_USART_ABORT_TRANSFER:
      // Disable TX Ready, TX Idle and RX Idle interrupts
      Chip_UART_IntDisable (usart->reg, UART_INT_RXRDY | UART_INT_TXRDY | UART_INT_TXIDLE);

      // Clear busy statuses
      usart->info->rx_status.rx_busy = 0U;
      usart->info->xfer.send_active  = 0U;
      return ARM_DRIVER_OK;

    default: break;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS:
      mode = ARM_USART_MODE_ASYNCHRONOUS;
      break;
    case ARM_USART_MODE_SYNCHRONOUS_MASTER:
      // Enable synchronous master mode
      cfg |= UART_CFG_SYNCEN  | UART_CFG_SYNCMST;
      mode = ARM_USART_MODE_SYNCHRONOUS_MASTER;
      break;
    case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
      // Enable synchronous slave mode
      cfg |= UART_CFG_SYNCEN;
      mode = ARM_USART_MODE_SYNCHRONOUS_SLAVE;
      break;
    case ARM_USART_MODE_SINGLE_WIRE:
      // Single wire mode not supported
      return ARM_USART_ERROR_MODE;
    case ARM_USART_MODE_IRDA:
      // Enable IrDA mode
      cfg |= UART_CFG_IRDA_MODE;
      mode = ARM_USART_MODE_IRDA;
      break;
    case ARM_USART_MODE_SMART_CARD:
      // Smart card mode not supported
      return ARM_USART_ERROR_MODE;

    // Default TX value
    case ARM_USART_SET_DEFAULT_TX_VALUE:
      usart->info->xfer.tx_def_val = arg;
      return ARM_DRIVER_OK;

    // IrDA pulse
    case ARM_USART_SET_IRDA_PULSE:
      if (arg != 0U) {
        return ARM_DRIVER_ERROR;
      }
      return ARM_DRIVER_OK;

    case ARM_USART_SET_SMART_CARD_GUARD_TIME:
    case ARM_USART_SET_SMART_CARD_CLOCK:
    case ARM_USART_CONTROL_SMART_CARD_NACK:
    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  // Check if Receiver/Transmitter is busy
  if ( usart->info->rx_status.rx_busy ||
      (usart->info->xfer.send_active != 0U)) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  // USART Data bits
  val = (500000U * 16U) / arg;
  for (i=0U; i < 16U; i++) {
    if (((val >> i) & 0xFFFFFFF0U) == 0U) {
      if ((val >> i) < 2U) { val = 2U; }
      else                 { val = val >> i; }
      break;
    }
  }

  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_7: cfg |= UART_CFG_DATALEN_7; break;
    case ARM_USART_DATA_BITS_8: cfg |= UART_CFG_DATALEN_8; break;
    case ARM_USART_DATA_BITS_9: cfg |= UART_CFG_DATALEN_9; break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE: cfg |= UART_CFG_PARITY_NONE; break;
    case ARM_USART_PARITY_EVEN: cfg |= UART_CFG_PARITY_EVEN; break;
    case ARM_USART_PARITY_ODD:  cfg |= UART_CFG_PARITY_ODD;  break;
    default: return (ARM_USART_ERROR_PARITY);
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1: cfg |= UART_CFG_STOPLEN_1; break;
    case ARM_USART_STOP_BITS_2: cfg |= UART_CFG_STOPLEN_2; break;
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE:
      if (usart->pins.rts != NULL) {
        Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rts->port, usart->pins.rts->pin, 0U);
      }
      break;
    case ARM_USART_FLOW_CONTROL_RTS:
      if (usart->capabilities.flow_control_rts) {
        // Configure RTS pin
        Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rts->port, usart->pins.rts->pin, usart->pins.rts->modefunc);
        return ARM_USART_ERROR_FLOW_CONTROL;
      }
      break;
    case ARM_USART_FLOW_CONTROL_CTS:
      if (usart->capabilities.flow_control_cts) {
        // Reset RTS pin configuration
        if (usart->capabilities.rts) {
          Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rts->port, usart->pins.rts->pin, 0U);
        }
        cfg |= UART_CFG_CTSEN;
      } else {
        return ARM_USART_ERROR_FLOW_CONTROL;
      }
      break;
    case ARM_USART_FLOW_CONTROL_RTS_CTS:
      if (usart->capabilities.flow_control_rts &&
          usart->capabilities.flow_control_cts) {
        // Configure USART RTS alternate function
        Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.rts->port, usart->pins.rts->pin, usart->pins.rts->modefunc);
        cfg |= UART_CFG_CTSEN;
      } else {
        return ARM_USART_ERROR_FLOW_CONTROL;
      }
    break;
    default: { return ARM_USART_ERROR_FLOW_CONTROL; }
  }

  // Clock setting for synchronous mode
  if ((mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
      (mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {

    switch (control & ARM_USART_CPOL_Msk) {
      case ARM_USART_CPOL0:
        cfg |= UART_CFG_CLKPOL;
        break;
      case ARM_USART_CPOL1:
        break;
      default: return ARM_USART_ERROR_CPOL;
    }

    // USART clock phase
    if ((control & ARM_USART_CPHA_Msk) != ARM_USART_CPHA1) {
      return ARM_USART_ERROR_CPHA;
    }
  }

  // USART Baudrate
#if (USARTx_FRGCTRL_MUL != 256U)
  div64  = Chip_Clock_GetAsyncSyscon_ClockRate() << 24;
  div64 /= (usart->oversample * arg * (256U * USARTx_FRGCTRL_MUL));
  if (div64 > 0xFFFFFFFFU) { return ARM_USART_ERROR_BAUDRATE; }

  div = div64 >> 16U;
  if (div64 > 0x10000U) {
    // Round up
    div++;
  }
  LPC_ASYNC_SYSCON->FRGCTRL = ((USARTx_FRGCTRL_MUL & 0xFFU) << 8) | 0xFFU;
  Chip_UART_Div (usart->reg, div, usart->oversample);
#else
  Chip_UART_SetBaud (usart->reg, arg);
#endif

  // Configuration is OK - Mode is valid
  usart->info->mode = mode;

  // Configure CLK pin regarding mode
  if (usart->pins.sclk) {
    switch (usart->info->mode) {
      case ARM_USART_MODE_SYNCHRONOUS_MASTER:
      case ARM_USART_MODE_SYNCHRONOUS_SLAVE:
        // Pin function = USART UCLK (input)
        Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.sclk->port, usart->pins.sclk->pin, usart->pins.sclk->modefunc);
        break;
      default:
        // Pin function = GPIO
        Chip_IOCON_PinMuxSet(LPC_IOCON, usart->pins.sclk->port, usart->pins.sclk->pin, 0U);
    }
  }

  // Set Configuration register
  usart->reg->CFG = cfg | UART_CFG_ENABLE;

  // Set configured flag
  usart->info->flags |= USART_FLAG_CONFIGURED;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (USART_RESOURCES *usart)
  \brief       Get USART status.
  \param[in]   usart     Pointer to USART resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (USART_RESOURCES *usart) {
  ARM_USART_STATUS stat;

  stat.tx_busy          = (Chip_UART_GetStatus (usart->reg) & UART_TXIDLE ? (0U) : (1U));
  stat.rx_busy          = usart->info->rx_status.rx_busy;
  stat.tx_underflow     = 0U;
  stat.rx_overflow      = usart->info->rx_status.rx_overflow;
  stat.rx_break         = usart->info->rx_status.rx_break;
  stat.rx_framing_error = usart->info->rx_status.rx_framing_error;
  stat.rx_parity_error  = usart->info->rx_status.rx_parity_error;
  return stat;
}

/**
  \fn          int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL  control,
                                              USART_RESOURCES         *usart)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (ARM_USART_MODEM_CONTROL  control,
                                      USART_RESOURCES         *usart) {

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured
    return ARM_DRIVER_ERROR;
  }

  if (usart->capabilities.rts) {
    if (control == ARM_USART_RTS_CLEAR) {
      Chip_GPIO_SetPinOutLow (LPC_GPIO, usart->pins.rts->port, (1U << usart->pins.rts->pin));
    }
    if (control == ARM_USART_RTS_SET) {
      Chip_GPIO_SetPinOutHigh (LPC_GPIO, usart->pins.rts->port, (1U << usart->pins.rts->pin));
    }
  } else {
    return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (USART_RESOURCES *usart)
  \brief       Get USART Modem Status lines state.
  \param[in]   usart     Pointer to USART resources
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (USART_RESOURCES *usart) {
  ARM_USART_MODEM_STATUS modem_status;

  modem_status.cts = 0U;
  modem_status.dsr = 0U;
  modem_status.ri  = 0U;
  modem_status.dcd = 0U;

  if ((usart->info->flags & USART_FLAG_CONFIGURED) != 0U) {
    if (Chip_UART_GetStatus (usart->reg) & UART_STAT_CTS) {
      modem_status.cts = 1U;
    }
  }

  return modem_status;
}

/**
  \fn          void USART_IRQHandler (USART_RESOURCES *usart)
  \brief       USART Interrupt handler.
  \param[in]   usart     Pointer to USART resources
*/
static void USART_IRQHandler (USART_RESOURCES *usart) {
  uint32_t val, event, usart_int_stat;
  uint16_t data;

  // Get Interrupt status
  usart_int_stat = Chip_UART_GetIntStatus(usart->reg);

  event = 0U;

  // TX Ready interrupt
  if (usart_int_stat & UART_INT_TXRDY) {
    val = usart->info->xfer.tx_num - usart->info->xfer.tx_cnt;

    if (val != 0U) {
      if (((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER)  ||
           (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) &&
           (usart->info->xfer.sync_mode == USART_SYNC_MODE_RX)) {

        // Dummy write in synchronous receive only mode
        usart->reg->TXDAT = usart->info->xfer.tx_def_val;

      } else {
        data = *(usart->info->xfer.tx_buf++);
        if (usart->reg->CFG & UART_CFG_DATALEN_9) {
          // 9-bit data
          data |= (*(usart->info->xfer.tx_buf++)) << 8;
          
        }
        usart->reg->TXDAT = data;
        usart->info->xfer.tx_cnt++;
      }
    }

    // Check if all data is transmitted
    if (usart->info->xfer.tx_cnt == usart->info->xfer.tx_num) {
      // Disable TX Ready interrupt
      Chip_UART_IntDisable (usart->reg, UART_INT_TXRDY);

      // Clear TX busy flag
      usart->info->xfer.send_active = 0U;

      // Set send complete event
      if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
          (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {
        if ((usart->info->xfer.sync_mode == USART_SYNC_MODE_TX)    &&
            ((usart->info->xfer.receiver_enabled) == 0U)) {
          event |= ARM_USART_EVENT_SEND_COMPLETE;
        }
      } else {
        event |= ARM_USART_EVENT_SEND_COMPLETE;
      }

      // Enable USART TX IDLE interrupt
      Chip_UART_IntEnable (usart->reg, (1U << 3));
    }
  }

  // USART Transmitter IDLE interrupt
  if (usart_int_stat & UART_STAT_TXIDLE) {
    event |= ARM_USART_EVENT_TX_COMPLETE;
    // Disable USART TX IDLE interrupt
    Chip_UART_IntDisable (usart->reg, (1U << 3));
  }

  // USART Frame Error interrupt
  if (usart_int_stat & UART_STAT_FRM_ERRINT) {
    // Update Status
    usart->info->rx_status.rx_framing_error = 1U;

    Chip_UART_ClearStatus (usart->reg, UART_INTEN_FRAMERR);
    event |= ARM_USART_EVENT_RX_FRAMING_ERROR;
  }

  // USART Parity Error interrupt
  if (usart_int_stat & UART_STAT_PAR_ERRINT) {
    // Update Status
    usart->info->rx_status.rx_parity_error = 1U;

    Chip_UART_ClearStatus (usart->reg, UART_INTEN_PARITYERR);
    event |= ARM_USART_EVENT_RX_PARITY_ERROR;
  }

  // USART Break detect interrupt
  if (usart_int_stat & UART_STAT_DELTARXBRK) {
    // Update Status
    usart->info->rx_status.rx_break = 1U;

    if (usart->reg->STAT & UART_STAT_RXBRK) {
      event |= ARM_USART_EVENT_RX_BREAK;
    }
    Chip_UART_ClearStatus (usart->reg, UART_STAT_DELTARXBRK);
  }

  // RX Ready interrupt
  if (usart_int_stat & UART_INT_RXRDY) {
    val = usart->info->xfer.rx_num - usart->info->xfer.rx_cnt;

    if (((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER)  ||
         (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) &&
         (usart->info->xfer.sync_mode == USART_SYNC_MODE_TX)) {
      // Dummy read in synchronous transmit only mode
      usart->reg->RXDAT;
    } else {
      // Read data
      data = usart->reg->RXDAT;
      *(usart->info->xfer.rx_buf++) = (uint8_t)data;
      if (usart->reg->CFG & UART_CFG_DATALEN_9) {
        // 9-bit data
        *(usart->info->xfer.rx_buf++) = (uint8_t)(data >> 8);
      }
      usart->info->xfer.rx_cnt++;
    }

  // Check if requested amount of data is received
  if (usart->info->xfer.rx_cnt == usart->info->xfer.rx_num) {
    // Disable RX Ready interrupt
    Chip_UART_IntDisable (usart->reg, UART_INT_RXRDY);

      // Clear RX busy flag and set receive transfer complete event
      usart->info->rx_status.rx_busy = 0U;
      if ((usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_MASTER) ||
          (usart->info->mode == ARM_USART_MODE_SYNCHRONOUS_SLAVE )) {
        val = usart->info->xfer.sync_mode;
        usart->info->xfer.sync_mode = 0U;
        switch (val) {
          case USART_SYNC_MODE_TX:
            event |= ARM_USART_EVENT_SEND_COMPLETE;
            break;
          case USART_SYNC_MODE_RX:
            event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
            break;
          case USART_SYNC_MODE_TX_RX:
            event |= ARM_USART_EVENT_TRANSFER_COMPLETE;
            break;
          default: break;
        }
      } else {
        event |= ARM_USART_EVENT_RECEIVE_COMPLETE;
      }
    }
  }

  // Modem interrupt
  if (usart_int_stat & UART_STAT_DELTACTS) {
    Chip_UART_ClearStatus (usart->reg, UART_INTEN_DELTACTS);
    event |= ARM_USART_EVENT_CTS;
  }

  if ((usart->info->cb_event != NULL) && (event != 0U)) {
    usart->info->cb_event (event);
  }
}

#if (RTE_USART0)
// USART0 Driver Wrapper functions
static ARM_USART_CAPABILITIES USART0_GetCapabilities (void) {
  return USART_GetCapabilities (&USART0_Resources);
}
static int32_t USART0_Initialize (ARM_USART_SignalEvent_t cb_event) {
  return USART_Initialize (cb_event, &USART0_Resources);
}
static int32_t USART0_Uninitialize (void) {
  return USART_Uninitialize(&USART0_Resources);
}
static int32_t USART0_PowerControl (ARM_POWER_STATE state) {
  return USART_PowerControl (state, &USART0_Resources);
}
static int32_t USART0_Send (const void *data, uint32_t num) {
  return USART_Send (data, num, &USART0_Resources);
}
static int32_t USART0_Receive (void *data, uint32_t num) {
  return USART_Receive (data, num, &USART0_Resources);
}
static int32_t USART0_Transfer (const void      *data_out,
                                      void      *data_in,
                                      uint32_t   num) {
  return USART_Transfer (data_out, data_in, num, &USART0_Resources);
}
static uint32_t USART0_GetTxCount (void) {
  return USART_GetTxCount (&USART0_Resources);
}
static uint32_t USART0_GetRxCount (void) {
  return USART_GetRxCount (&USART0_Resources); 
}
static int32_t USART0_Control (uint32_t control, uint32_t arg) {
  return USART_Control (control, arg, &USART0_Resources);
}
static ARM_USART_STATUS USART0_GetStatus (void) {
  return USART_GetStatus (&USART0_Resources);
}
static int32_t USART0_SetModemControl (ARM_USART_MODEM_CONTROL control) {
  return USART_SetModemControl (control, &USART0_Resources);
}
static ARM_USART_MODEM_STATUS USART0_GetModemStatus (void) {
  return USART_GetModemStatus (&USART0_Resources);
}
void UART0_IRQHandler (void) {
  USART_IRQHandler (&USART0_Resources);
}

// USART0 Driver Control Block
ARM_DRIVER_USART Driver_USART0 = {
    USARTx_GetVersion,
    USART0_GetCapabilities,
    USART0_Initialize,
    USART0_Uninitialize,
    USART0_PowerControl,
    USART0_Send, 
    USART0_Receive,
    USART0_Transfer,
    USART0_GetTxCount,
    USART0_GetRxCount,
    USART0_Control,
    USART0_GetStatus,
    USART0_SetModemControl,
    USART0_GetModemStatus
};
#endif

#if (RTE_USART1)
// USART1 Driver Wrapper functions
static ARM_USART_CAPABILITIES USART1_GetCapabilities (void) {
  return USART_GetCapabilities (&USART1_Resources);
}
static int32_t USART1_Initialize (ARM_USART_SignalEvent_t cb_event) {
  return USART_Initialize (cb_event, &USART1_Resources);
}
static int32_t USART1_Uninitialize (void) {
  return USART_Uninitialize(&USART1_Resources);
}
static int32_t USART1_PowerControl (ARM_POWER_STATE state) {
  return USART_PowerControl (state, &USART1_Resources);
}
static int32_t USART1_Send (const void *data, uint32_t num) {
  return USART_Send (data, num, &USART1_Resources);
}
static int32_t USART1_Receive (void *data, uint32_t num) {
  return USART_Receive (data, num, &USART1_Resources);
}
static int32_t USART1_Transfer (const void      *data_out,
                                      void      *data_in,
                                      uint32_t   num) {
  return USART_Transfer (data_out, data_in, num, &USART1_Resources);
}
static uint32_t USART1_GetTxCount (void) {
  return USART_GetTxCount (&USART1_Resources);
}
static uint32_t USART1_GetRxCount (void) {
  return USART_GetRxCount (&USART1_Resources); 
}
static int32_t USART1_Control (uint32_t control, uint32_t arg) {
  return USART_Control (control, arg, &USART1_Resources);
}
static ARM_USART_STATUS USART1_GetStatus (void) {
  return USART_GetStatus (&USART1_Resources);
}
static int32_t USART1_SetModemControl (ARM_USART_MODEM_CONTROL control) {
  return USART_SetModemControl (control, &USART1_Resources);
}
static ARM_USART_MODEM_STATUS USART1_GetModemStatus (void) {
  return USART_GetModemStatus (&USART1_Resources);
}
void UART1_IRQHandler (void) {
  USART_IRQHandler (&USART1_Resources);
}

// USART1 Driver Control Block
ARM_DRIVER_USART Driver_USART1 = {
    USARTx_GetVersion,
    USART1_GetCapabilities,
    USART1_Initialize,
    USART1_Uninitialize,
    USART1_PowerControl,
    USART1_Send, 
    USART1_Receive,
    USART1_Transfer,
    USART1_GetTxCount,
    USART1_GetRxCount,
    USART1_Control,
    USART1_GetStatus,
    USART1_SetModemControl,
    USART1_GetModemStatus
};
#endif

#if (RTE_USART2)
// USART2 Driver Wrapper functions
static ARM_USART_CAPABILITIES USART2_GetCapabilities (void) {
  return USART_GetCapabilities (&USART2_Resources);
}
static int32_t USART2_Initialize (ARM_USART_SignalEvent_t cb_event) {
  return USART_Initialize (cb_event, &USART2_Resources);
}
static int32_t USART2_Uninitialize (void) {
  return USART_Uninitialize(&USART2_Resources);
}
static int32_t USART2_PowerControl (ARM_POWER_STATE state) {
  return USART_PowerControl (state, &USART2_Resources);
}
static int32_t USART2_Send (const void *data, uint32_t num) {
  return USART_Send (data, num, &USART2_Resources);
}
static int32_t USART2_Receive (void *data, uint32_t num) {
  return USART_Receive (data, num, &USART2_Resources);
}
static int32_t USART2_Transfer (const void      *data_out,
                                      void      *data_in,
                                      uint32_t   num) {
  return USART_Transfer (data_out, data_in, num, &USART2_Resources);
}
static uint32_t USART2_GetTxCount (void) {
  return USART_GetTxCount (&USART2_Resources);
}
static uint32_t USART2_GetRxCount (void) {
  return USART_GetRxCount (&USART2_Resources); 
}
static int32_t USART2_Control (uint32_t control, uint32_t arg) {
  return USART_Control (control, arg, &USART2_Resources);
}
static ARM_USART_STATUS USART2_GetStatus (void) {
  return USART_GetStatus (&USART2_Resources);
}
static int32_t USART2_SetModemControl (ARM_USART_MODEM_CONTROL control) {
  return USART_SetModemControl (control, &USART2_Resources);
}
static ARM_USART_MODEM_STATUS USART2_GetModemStatus (void) {
  return USART_GetModemStatus (&USART2_Resources);
}
void UART2_IRQHandler (void) {
  USART_IRQHandler (&USART2_Resources);
}

// USART2 Driver Control Block
ARM_DRIVER_USART Driver_USART2 = {
    USARTx_GetVersion,
    USART2_GetCapabilities,
    USART2_Initialize,
    USART2_Uninitialize,
    USART2_PowerControl,
    USART2_Send, 
    USART2_Receive,
    USART2_Transfer,
    USART2_GetTxCount,
    USART2_GetRxCount,
    USART2_Control,
    USART2_GetStatus,
    USART2_SetModemControl,
    USART2_GetModemStatus
};
#endif

#if (RTE_USART3)
// USART3 Driver Wrapper functions
static ARM_USART_CAPABILITIES USART3_GetCapabilities (void) {
  return USART_GetCapabilities (&USART3_Resources);
}
static int32_t USART3_Initialize (ARM_USART_SignalEvent_t cb_event) {
  return USART_Initialize (cb_event, &USART3_Resources);
}
static int32_t USART3_Uninitialize (void) {
  return USART_Uninitialize(&USART3_Resources);
}
static int32_t USART3_PowerControl (ARM_POWER_STATE state) {
  return USART_PowerControl (state, &USART3_Resources);
}
static int32_t USART3_Send (const void *data, uint32_t num) {
  return USART_Send (data, num, &USART3_Resources);
}
static int32_t USART3_Receive (void *data, uint32_t num) {
  return USART_Receive (data, num, &USART3_Resources);
}
static int32_t USART3_Transfer (const void      *data_out,
                                      void      *data_in,
                                      uint32_t   num) {
  return USART_Transfer (data_out, data_in, num, &USART3_Resources);
}
static uint32_t USART3_GetTxCount (void) {
  return USART_GetTxCount (&USART3_Resources);
}
static uint32_t USART3_GetRxCount (void) {
  return USART_GetRxCount (&USART3_Resources); 
}
static int32_t USART3_Control (uint32_t control, uint32_t arg) {
  return USART_Control (control, arg, &USART3_Resources);
}
static ARM_USART_STATUS USART3_GetStatus (void) {
  return USART_GetStatus (&USART3_Resources);
}
static int32_t USART3_SetModemControl (ARM_USART_MODEM_CONTROL control) {
  return USART_SetModemControl (control, &USART3_Resources);
}
static ARM_USART_MODEM_STATUS USART3_GetModemStatus (void) {
  return USART_GetModemStatus (&USART3_Resources);
}
void UART3_IRQHandler (void) {
  USART_IRQHandler (&USART3_Resources);
}

// USART3 Driver Control Block
ARM_DRIVER_USART Driver_USART3 = {
    USARTx_GetVersion,
    USART3_GetCapabilities,
    USART3_Initialize,
    USART3_Uninitialize,
    USART3_PowerControl,
    USART3_Send, 
    USART3_Receive,
    USART3_Transfer,
    USART3_GetTxCount,
    USART3_GetRxCount,
    USART3_Control,
    USART3_GetStatus,
    USART3_SetModemControl,
    USART3_GetModemStatus
};
#endif

