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
 * $Date:        2. February 2016
 * $Revision:    V1.0
 *
 * Driver:       Driver_USBD0
 * Configured:   via RTE_Device.h configuration file
 * Project:      USB Device Driver for NXP LPC5411x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                  Value
 *   ---------------------                  -----
 *   Connect to hardware via Driver_USBD# = 0
 * --------------------------------------------------------------------------
 * Defines used for driver configuration (at compile time):
 *
 *   USBD_MAX_ENDPOINT_NUM:  defines maximum number of IN/OUT Endpoint pairs 
 *                           that driver will support with Control Endpoint 0
 *                           not included, this value impacts driver memory
 *                           requirements
 *     - default value: 4
 *     - maximum value: 4
 *
 *   USBD_OUT_EP0_BUF_SZ:    defines Out Endpoint0 buffer size (in Bytes)
 *   USBD_IN_EP0_BUF_SZ:     defines In  Endpoint0 buffer size (in Bytes)
 *   USBD_OUT_EP1_BUF_SZ:    defines Out Endpoint1 buffer size (in Bytes)
 *   USBD_IN_EP1_BUF_SZ:     defines In  Endpoint1 buffer size (in Bytes)
 *   USBD_OUT_EP2_BUF_SZ:    defines Out Endpoint2 buffer size (in Bytes)
 *   USBD_IN_EP2_BUF_SZ:     defines In  Endpoint2 buffer size (in Bytes)
 *   USBD_OUT_EP3_BUF_SZ:    defines Out Endpoint3 buffer size (in Bytes)
 *   USBD_IN_EP3_BUF_SZ:     defines In  Endpoint3 buffer size (in Bytes)
 *   USBD_OUT_EP4_BUF_SZ:    defines Out Endpoint4 buffer size (in Bytes)
 *   USBD_IN_EP4_BUF_SZ:     defines In  Endpoint4 buffer size (in Bytes)
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */

#include <stdint.h>
#include <string.h>

#include "Driver_USBD.h"

#include "chip.h"
#include "USBD_LPC5411x.h"

#include "RTE_Device.h"
#include "RTE_Components.h"

#if      (RTE_USB_USB == 0)
#error   "USBD is not enabled in the RTE_Device.h!"
#endif

// Endpoint buffer must be 64Byte aligned
#define ALIGN_64(n)                    (n == 0U ? (0U) : (64U * (((n - 1U) / 64U) + 1U)))

#ifndef USBD_MAX_ENDPOINT_NUM
#define USBD_MAX_ENDPOINT_NUM          (4U)
#endif
#if    (USBD_MAX_ENDPOINT_NUM > 4)
#error  Too many Endpoints, maximum IN/OUT Endpoint pairs that this driver supports is 4 !!!
#endif

// Endpoint Bufer size definitions
#ifndef USBD_OUT_EP0_BUF_SZ
#define USBD_OUT_EP0_BUF_SZ            (64U)
#endif
#ifndef USBD_IN_EP0_BUF_SZ
#define USBD_IN_EP0_BUF_SZ             (64U)
#endif
#define USBD_OUT_EP0_BUF_SZ_64         (ALIGN_64(USBD_OUT_EP0_BUF_SZ))
#define USBD_IN_EP0_BUF_SZ_64          (ALIGN_64(USBD_IN_EP0_BUF_SZ))

#if (USBD_MAX_ENDPOINT_NUM > 0)
#ifndef USBD_OUT_EP1_BUF_SZ
#define USBD_OUT_EP1_BUF_SZ            (64U)
#endif
#ifndef USBD_IN_EP1_BUF_SZ
#define USBD_IN_EP1_BUF_SZ             (64U)
#endif                     
#else
#define USBD_OUT_EP1_BUF_SZ            (0U)
#define USBD_IN_EP1_BUF_SZ             (0U)
#endif
#define USBD_OUT_EP1_BUF_SZ_64         (ALIGN_64(USBD_OUT_EP1_BUF_SZ))
#define USBD_IN_EP1_BUF_SZ_64          (ALIGN_64(USBD_IN_EP1_BUF_SZ))

#if (USBD_MAX_ENDPOINT_NUM > 1)
#ifndef USBD_OUT_EP2_BUF_SZ
#define USBD_OUT_EP2_BUF_SZ            (64U)
#endif
#ifndef USBD_IN_EP2_BUF_SZ
#define USBD_IN_EP2_BUF_SZ             (64U)
#endif
#else
#define USBD_OUT_EP2_BUF_SZ            (0U)
#define USBD_IN_EP2_BUF_SZ             (0U)
#endif
#define USBD_OUT_EP2_BUF_SZ_64         (ALIGN_64(USBD_OUT_EP2_BUF_SZ))
#define USBD_IN_EP2_BUF_SZ_64          (ALIGN_64(USBD_IN_EP2_BUF_SZ))

#if (USBD_MAX_ENDPOINT_NUM > 2)
#ifndef USBD_OUT_EP3_BUF_SZ
#define USBD_OUT_EP3_BUF_SZ            (64U)
#endif
#ifndef USBD_IN_EP3_BUF_SZ
#define USBD_IN_EP3_BUF_SZ             (64U)
#endif
#else
#define USBD_OUT_EP3_BUF_SZ            (0U)
#define USBD_IN_EP3_BUF_SZ             (0U)
#endif
#define USBD_OUT_EP3_BUF_SZ_64         (ALIGN_64(USBD_OUT_EP3_BUF_SZ))
#define USBD_IN_EP3_BUF_SZ_64          (ALIGN_64(USBD_IN_EP3_BUF_SZ))

#if (USBD_MAX_ENDPOINT_NUM > 3)
#ifndef USBD_OUT_EP4_BUF_SZ
#define USBD_OUT_EP4_BUF_SZ            (64U)
#endif
#ifndef USBD_IN_EP4_BUF_SZ
#define USBD_IN_EP4_BUF_SZ             (64U)
#endif
#else
#define USBD_OUT_EP4_BUF_SZ            (0U)
#define USBD_IN_EP4_BUF_SZ             (0U)
#endif
#define USBD_OUT_EP4_BUF_SZ_64         (ALIGN_64(USBD_OUT_EP4_BUF_SZ))
#define USBD_IN_EP4_BUF_SZ_64          (ALIGN_64(USBD_IN_EP4_BUF_SZ))

#define USBD_OUT_EP0_BUF_OFFSET        (0U)
#define USBD_IN_EP0_BUF_OFFSET         (USBD_OUT_EP0_BUF_SZ_64)
#define USBD_OUT_EP1_BUF_OFFSET        (USBD_IN_EP0_BUF_OFFSET  + USBD_IN_EP0_BUF_SZ_64)
#define USBD_IN_EP1_BUF_OFFSET         (USBD_OUT_EP1_BUF_OFFSET + USBD_OUT_EP1_BUF_SZ_64)
#define USBD_OUT_EP2_BUF_OFFSET        (USBD_IN_EP1_BUF_OFFSET  + USBD_IN_EP1_BUF_SZ_64)
#define USBD_IN_EP2_BUF_OFFSET         (USBD_OUT_EP2_BUF_OFFSET + USBD_OUT_EP2_BUF_SZ_64)
#define USBD_OUT_EP3_BUF_OFFSET        (USBD_IN_EP2_BUF_OFFSET  + USBD_IN_EP2_BUF_SZ_64)
#define USBD_IN_EP3_BUF_OFFSET         (USBD_OUT_EP3_BUF_OFFSET + USBD_OUT_EP3_BUF_SZ_64)
#define USBD_OUT_EP4_BUF_OFFSET        (USBD_IN_EP3_BUF_OFFSET  + USBD_IN_EP3_BUF_SZ_64)
#define USBD_IN_EP4_BUF_OFFSET         (USBD_OUT_EP4_BUF_OFFSET + USBD_OUT_EP4_BUF_SZ_64)

#define USBD_EP_BUFFER_SZ              (USBD_OUT_EP0_BUF_SZ_64 + USBD_IN_EP0_BUF_SZ_64 + \
                                        USBD_OUT_EP1_BUF_SZ_64 + USBD_IN_EP1_BUF_SZ_64 + \
                                        USBD_OUT_EP2_BUF_SZ_64 + USBD_IN_EP2_BUF_SZ_64 + \
                                        USBD_OUT_EP3_BUF_SZ_64 + USBD_IN_EP3_BUF_SZ_64 + \
                                        USBD_OUT_EP4_BUF_SZ_64 + USBD_IN_EP4_BUF_SZ_64 )

#if (USBD_EP_BUFFER_SZ > 0x8000U)
  #error "Endpoint buffers do not fit into RAMx!"
#endif

#define EP_NUM(ep_addr)   (ep_addr & ARM_USB_ENDPOINT_NUMBER_MASK)
#define EP_IDX(ep_addr)  ((ep_addr & 0x80U) ? ((EP_NUM(ep_addr)) * 2U + 1U)  : (ep_addr * 2U))
#define CMD_IDX(ep_addr) ((ep_addr & 0x80U) ? ((EP_NUM(ep_addr)) * 4U + 2U)  : (ep_addr * 4U))

// Resource allocation
static uint8_t           ep_buf[USBD_EP_BUFFER_SZ]                __attribute__((at(0x04000000U)));
static EP_CMD            ep_cmd[(USBD_MAX_ENDPOINT_NUM + 1)* 4]   __attribute__((aligned(0x100U)));
static EP_TRANSFER       ep_transfer[(USBD_MAX_ENDPOINT_NUM + 1) * 2];

// Global variables
static ARM_USBD_STATE    usbd_state;
static uint8_t           usbd_flags;

static uint8_t           setup_packet[8];     // Setup packet data
static volatile uint8_t  setup_received;      // Setup packet received

static ARM_USBD_SignalDeviceEvent_t   SignalDeviceEvent;
static ARM_USBD_SignalEndpointEvent_t SignalEndpointEvent;

static const EP endpoint[] = {
  // Endpoint 0
  { &(ep_cmd[0]),  &(ep_buf[USBD_OUT_EP0_BUF_OFFSET]), &(ep_transfer[0]), USBD_OUT_EP0_BUF_OFFSET, },
  { &(ep_cmd[2]),  &(ep_buf[USBD_IN_EP0_BUF_OFFSET]),  &(ep_transfer[1]), USBD_IN_EP0_BUF_OFFSET,  },

#if (USBD_MAX_ENDPOINT_NUM > 0U)
  // Endpoint 1
  { &(ep_cmd[4]),  &(ep_buf[USBD_OUT_EP1_BUF_OFFSET]), &(ep_transfer[2]), USBD_OUT_EP1_BUF_OFFSET, },
  { &(ep_cmd[6]),  &(ep_buf[USBD_IN_EP1_BUF_OFFSET]),  &(ep_transfer[3]), USBD_IN_EP1_BUF_OFFSET,  },
#endif

#if (USBD_MAX_ENDPOINT_NUM > 1U)
  // Endpoint 2
  { &(ep_cmd[8]),  &(ep_buf[USBD_OUT_EP2_BUF_OFFSET]), &(ep_transfer[4]), USBD_OUT_EP2_BUF_OFFSET, },
  { &(ep_cmd[10]), &(ep_buf[USBD_IN_EP2_BUF_OFFSET]),  &(ep_transfer[5]), USBD_IN_EP2_BUF_OFFSET,  },
#endif

#if (USBD_MAX_ENDPOINT_NUM > 2U)
  // Endpoint 3
  { &(ep_cmd[12]), &(ep_buf[USBD_OUT_EP3_BUF_OFFSET]), &(ep_transfer[6]), USBD_OUT_EP3_BUF_OFFSET, },
  { &(ep_cmd[14]), &(ep_buf[USBD_IN_EP3_BUF_OFFSET]),  &(ep_transfer[7]), USBD_IN_EP3_BUF_OFFSET,  },
#endif

#if (USBD_MAX_ENDPOINT_NUM > 3U)
  // Endpoint 4
  { &(ep_cmd[16]), &(ep_buf[USBD_OUT_EP4_BUF_OFFSET]), &(ep_transfer[8]), USBD_OUT_EP4_BUF_OFFSET, },
  { &(ep_cmd[18]), &(ep_buf[USBD_IN_EP4_BUF_OFFSET]),  &(ep_transfer[9]), USBD_IN_EP4_BUF_OFFSET,  },
#endif
};


// USBD Driver *****************************************************************

#define ARM_USBD_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

// Driver Version
static const ARM_DRIVER_VERSION usbd_driver_version = { ARM_USBD_API_VERSION, ARM_USBD_DRV_VERSION };

// Driver Capabilities
static const ARM_USBD_CAPABILITIES usbd_driver_capabilities = {
#if (USBD_VBUS_DETECT == 1)
  1U,   // VBUS Detection
  1U,   // Event VBUS On
  1U,   // Event VBUS Off
#else
  0U,   // VBUS Detection
  0U,   // Event VBUS On
  0U    // Event VBUS Off
#endif
};

/**
  \fn          void USBD_Reset (void)
  \brief       Reset USB Endpoint settings and variables.
*/
static void USBD_Reset (void) {
  // Clear USB Endpoint command/status list
  memset((void *)ep_cmd, 0, sizeof(ep_cmd));

  memset((void *)&usbd_state, 0, sizeof(usbd_state));
}

// USBD Driver functions

/**
  \fn          ARM_DRIVER_VERSION USBD_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USBD_GetVersion (void) { return usbd_driver_version; }

/**
  \fn          ARM_USBD_CAPABILITIES USBD_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_USBD_CAPABILITIES
*/
static ARM_USBD_CAPABILITIES USBD_GetCapabilities (void) { return usbd_driver_capabilities; }

/**
  \fn          int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t   cb_device_event,
                                        ARM_USBD_SignalEndpointEvent_t cb_endpoint_event)
  \brief       Initialize USB Device Interface.
  \param[in]   cb_device_event    Pointer to \ref ARM_USBD_SignalDeviceEvent
  \param[in]   cb_endpoint_event  Pointer to \ref ARM_USBD_SignalEndpointEvent
  \return      \ref execution_status
*/
static int32_t USBD_Initialize (ARM_USBD_SignalDeviceEvent_t   cb_device_event,
                                ARM_USBD_SignalEndpointEvent_t cb_endpoint_event) {

  if ((usbd_flags & USBD_DRIVER_FLAG_INITIALIZED) != 0U) { return ARM_DRIVER_OK; }

  SignalDeviceEvent   = cb_device_event;
  SignalEndpointEvent = cb_endpoint_event;

  // Configure Pins
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);
  Chip_IOCON_PinMuxSet (LPC_IOCON, RTE_USB_VBUS_PORT, RTE_USB_VBUS_BIT, RTE_USB_VBUS_FUNC | IOCON_DIGITAL_EN | IOCON_MODE_INACT);

#if (RTE_USB_UP_LED_PIN_EN != 0U)
  Chip_IOCON_PinMuxSet (LPC_IOCON, RTE_USB_UP_LED_PORT, RTE_USB_UP_LED_BIT, RTE_USB_UP_LED_FUNC | IOCON_DIGITAL_EN);
#endif

  usbd_flags =  USBD_DRIVER_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_Uninitialize (void)
  \brief       De-initialize USB Device Interface.
  \return      \ref execution_status
*/
static int32_t USBD_Uninitialize (void) {

  // Unconfigure Pins
  Chip_IOCON_PinMuxSet (LPC_IOCON, RTE_USB_VBUS_PORT, RTE_USB_VBUS_BIT, 0U);
#if (RTE_USB_UP_LED_PIN_EN != 0U)
  Chip_IOCON_PinMuxSet (LPC_IOCON, RTE_USB_UP_LED_PORT, RTE_USB_UP_LED_BIT, 0U);
#endif

  usbd_flags &= ~USBD_DRIVER_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_PowerControl (ARM_POWER_STATE state)
  \brief       Control USB Device Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t USBD_PowerControl (ARM_POWER_STATE state) {

  switch (state) {
    case ARM_POWER_OFF:
      NVIC_DisableIRQ      (USB_IRQn);                  // Disable interrupt
      NVIC_ClearPendingIRQ (USB_IRQn);                  // Clear pending interrupt
      usbd_flags &= ~USBD_DRIVER_FLAG_POWERED;          // Clear powered flag
                                                        // Reset variables

      // Clear USB Endpoint command/status list
      memset((void *)ep_cmd, 0, sizeof(ep_cmd));

      // Clear Endpoint transfer structure
      memset((void *)ep_transfer, 0, sizeof(ep_transfer));

      // Reset the USB peripheral
      Chip_SYSCON_PeriphReset(RESET_USB);

      // Power down USB PHY
      Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_USB_PHY);

      // Disable waking-up of MCU from USB
      Chip_SYSCON_DisableWakeup(SYSCON_STARTER_USB);

      // Disable USB Peripheral clock
      Chip_Clock_DisablePeriphClock(SYSCON_CLOCK_USB);

      break;

    case ARM_POWER_FULL:
      if ((usbd_flags & USBD_DRIVER_FLAG_INITIALIZED) == 0U) { return ARM_DRIVER_ERROR; }
      if ((usbd_flags & USBD_DRIVER_FLAG_POWERED)     != 0U) { return ARM_DRIVER_OK; }

      // USB Device initialize
      Chip_USB_Init ();

      // Reset variables and endpoint settings
      USBD_Reset ();

      // Set Endpoint list start address
      LPC_USB->EPLISTSTART = (uint32_t)ep_cmd;

      // Set USB Data buffer start address
      LPC_USB->DATABUFSTART = (uint32_t)ep_buf;

      // Enable device status interrupt
      LPC_USB->INTEN = USB_INT_DEV_INT;

      usbd_flags |=  USBD_DRIVER_FLAG_POWERED;
      // Enable USB interrupt
      NVIC_EnableIRQ (USB_IRQn);
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceConnect (void)
  \brief       Connect USB Device.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceConnect (void) {

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  // Attach Device
  LPC_USB->DEVCMDSTAT |= USB_DEVCMDSTAT_DCON;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceDisconnect (void)
  \brief       Disconnect USB Device.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceDisconnect (void) {

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  // Detach Device
  LPC_USB->DEVCMDSTAT &= ~USB_DEVCMDSTAT_DCON;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USBD_STATE USBD_DeviceGetState (void)
  \brief       Get current USB Device State.
  \return      Device State \ref ARM_USBD_STATE
*/
static ARM_USBD_STATE USBD_DeviceGetState (void) {
  ARM_USBD_STATE dev_state = { 0U, 0U, 0U };

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return dev_state; }


  return usbd_state;
}

/**
  \fn          int32_t USBD_DeviceRemoteWakeup (void)
  \brief       Trigger USB Remote Wakeup.
  \return      \ref execution_status
*/
static int32_t USBD_DeviceRemoteWakeup (void) {

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  // Force remote wakeup
  LPC_USB->DEVCMDSTAT &= ~USB_DEVCMDSTAT_DSUS;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_DeviceSetAddress (uint8_t dev_addr)
  \brief       Set USB Device Address.
  \param[in]   dev_addr  Device Address
  \return      \ref execution_status
*/
static int32_t USBD_DeviceSetAddress (uint8_t dev_addr) {

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_ReadSetupPacket (uint8_t *setup)
  \brief       Read setup packet received over Control Endpoint.
  \param[out]  setup  Pointer to buffer for setup packet
  \return      \ref execution_status
*/
static int32_t USBD_ReadSetupPacket (uint8_t *setup) {
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }
  if (setup_received                          == 0U) { return ARM_DRIVER_ERROR; }

  setup_received = 0U;
  memcpy(setup, setup_packet, 8);

  if (setup_received != 0U) {           // If new setup packet was received while this was being read
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointConfigure (uint8_t  ep_addr,
                                               uint8_t  ep_type,
                                               uint16_t ep_max_packet_size)
  \brief       Configure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   ep_type  Endpoint Type (ARM_USB_ENDPOINT_xxx)
  \param[in]   ep_max_packet_size Endpoint Maximum Packet Size
  \return      \ref execution_status
*/
static int32_t USBD_EndpointConfigure (uint8_t  ep_addr,
                                       uint8_t  ep_type,
                                       uint16_t ep_max_packet_size) {
  uint8_t ep_num, ep_idx;
  EP const * ep;

  ep_num = EP_NUM(ep_addr);
  ep_idx = EP_IDX(ep_addr);
  ep     = &endpoint[ep_idx];

  if (ep_num > USBD_MAX_ENDPOINT_NUM)                { return ARM_DRIVER_ERROR; }
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  if (ep->cmd->active == 1U) {
    // Endpoint is "owned" by hardware
    return ARM_DRIVER_ERROR_BUSY;
  }

  if (ep_max_packet_size > ((ep+1)->buf_offset - ep->buf_offset)) {
    // Configured Endpoint buffer size is too small
    return ARM_DRIVER_ERROR;
  }

  // Clear Endpoint command/status
  memset((void *)ep->cmd, 0, sizeof(EP_CMD) * 2U);

  // Clear Endpoint transfer structure
  memset((void *)ep->transfer, 0, sizeof(EP_TRANSFER));

  ep_transfer[ep_idx].max_packet_sz = ep_max_packet_size & ARM_USB_ENDPOINT_MAX_PACKET_SIZE_MASK;

  if (ep_type == ARM_USB_ENDPOINT_ISOCHRONOUS) {
    ep->cmd->ep_type_isoch = 1;
  }

  ep->cmd->buff_addr_offset = ep->buf_offset >> 6;
  ep->cmd->ep_disabled      = 0U;
  ep->cmd->toggle_value     = 0U;

  if (ep_num != 0) {
    ep->cmd->toggle_reset   = 1U;
  }

  ep->cmd[1].buff_addr_offset = ep->buf_offset >> 6;
  ep->cmd[1].ep_disabled      = 0U;
  ep->cmd[1].toggle_value     = 0U;

  // Clear Endpoint Interrupt
  LPC_USB->INTSTAT = USB_INT_EP(ep_idx);

  // Enable endpoint interrupt
  LPC_USB->INTEN  |= USB_INT_EP(ep_idx);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointUnconfigure (uint8_t ep_addr)
  \brief       Unconfigure USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      \ref execution_status
*/
static int32_t USBD_EndpointUnconfigure (uint8_t ep_addr) {
  uint8_t ep_num, ep_idx;
  EP const * ep;

  ep_num = EP_NUM(ep_addr);
  ep_idx = EP_IDX(ep_addr);
  ep     = &endpoint[ep_idx];

  if (ep_num > USBD_MAX_ENDPOINT_NUM)                { return ARM_DRIVER_ERROR; }
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  if (ep->cmd->active == 1U) {
    // Endpoint is "owned" by hardware
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Disable endpoint interrupt
  LPC_USB->INTEN &= ~USB_INT_EP(ep_idx);

  if (ep->cmd->active) {
    LPC_USB->EPSKIP |= (1U << ep_idx);
    while (LPC_USB->EPSKIP & (1U << ep_idx));
  }

  // Clear Endpoint command/status
  memset((void *)ep->cmd, 0, sizeof(EP_CMD) * 2U);

  ep->cmd->ep_disabled = 1U;

  // Clear Endpoint Interrupt
  LPC_USB->INTSTAT = USB_INT_EP(ep_idx);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointStall (uint8_t ep_addr, bool stall)
  \brief       Set/Clear Stall for USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   stall  Operation
                - \b false Clear
                - \b true Set
  \return      \ref execution_status
*/
static int32_t USBD_EndpointStall (uint8_t ep_addr, bool stall) {
  uint8_t ep_num;
  EP const * ep;

  ep_num = EP_NUM(ep_addr);
  ep     = &endpoint[EP_IDX(ep_addr)];

  if (ep_num > USBD_MAX_ENDPOINT_NUM)                { return ARM_DRIVER_ERROR; }
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  if (ep->cmd->active == 1U) {
    // Endpoint is "owned" by hardware
    return ARM_DRIVER_ERROR_BUSY;
  }

  if (stall != 0U) {
    // Set Endpoint stall
    ep->cmd->stall = 1U;
  } else {
    ep->cmd->toggle_value     = 0U;
    ep->cmd->toggle_reset     = 1U;

    // Clear Stall
    ep->cmd->stall = 0U;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBD_EndpointTransfer (uint8_t ep_addr, uint8_t *data, uint32_t num)
  \brief       Read data from or Write data to USB Endpoint.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[out]  data Pointer to buffer for data to read or with data to write
  \param[in]   num  Number of data bytes to transfer
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransfer (uint8_t ep_addr, uint8_t *data, uint32_t num) {
  uint8_t      ep_num, ep_idx;
  EP const * ep;

  ep_num = EP_NUM(ep_addr);
  ep_idx = EP_IDX(ep_addr);
  ep     = &endpoint[ep_idx];

  if (ep_num > USBD_MAX_ENDPOINT_NUM)                { return ARM_DRIVER_ERROR; }
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U)  { return ARM_DRIVER_ERROR; }

  if (ep->cmd->active == 1U) {
    // Endpoint is "owned" by hardware
    return ARM_DRIVER_ERROR_BUSY;
  }

  ep->transfer->num = num;
  ep->transfer->buf = data;
  ep->transfer->num_transferred_total = 0U;
  if (num > ep->transfer->max_packet_sz) { num = ep->transfer->max_packet_sz; }

  if (ep_addr & ARM_USB_ENDPOINT_DIRECTION_MASK) {
    // Copy data into IN Endpoint buffer
    memcpy (ep->buf, ep->transfer->buf, num);
  }

  ep->cmd->buff_addr_offset = ep->buf_offset >> 6;

  ep->transfer->num_transferring = num;

  // Set number of bytes to send/receive
  ep->cmd->NBytes = num;

  // Activate endpoint
  ep->cmd->active |= 1U;

  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USBD_EndpointTransferGetResult (uint8_t ep_addr)
  \brief       Get result of USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      number of successfully transferred data bytes
*/
static uint32_t USBD_EndpointTransferGetResult (uint8_t ep_addr) {

  if (EP_NUM(ep_addr) > USBD_MAX_ENDPOINT_NUM) { return 0U; }

  return (ep_transfer[EP_IDX(ep_addr)].num_transferred_total);
}

/**
  \fn          int32_t USBD_EndpointTransferAbort (uint8_t ep_addr)
  \brief       Abort current USB Endpoint transfer.
  \param[in]   ep_addr  Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \return      \ref execution_status
*/
static int32_t USBD_EndpointTransferAbort (uint8_t ep_addr) {
  uint8_t      ep_num, ep_idx;
  EP const * ep;

  ep_num = EP_NUM(ep_addr);
  ep_idx = EP_IDX(ep_addr);
  ep     = &endpoint[ep_idx];

  if (ep_num > USBD_MAX_ENDPOINT_NUM)                { return ARM_DRIVER_ERROR; }
  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return ARM_DRIVER_ERROR; }

  // Disable endpoint interrupt
  LPC_USB->INTEN  &= ~USB_INT_EP(ep_idx);

  if (ep->cmd->active == 1U) {
    LPC_USB->EPSKIP |= (1U << EP_IDX(ep_addr));
    while (LPC_USB->EPSKIP & (1U << EP_IDX(ep_addr)));
    ep->cmd->active = 0U;
  }

  // Clear transfer info
  ep->transfer->num                   = 0U;
  ep->transfer->num_transferred_total = 0U;
  ep->transfer->num_transferring      = 0U;

  // Clear Endpoint Interrupt
  LPC_USB->INTSTAT = USB_INT_EP(ep_idx);

  // Enable endpoint interrupt
  LPC_USB->INTEN  |= USB_INT_EP(ep_idx);

  return ARM_DRIVER_OK;
}

/**
  \fn          uint16_t USBD_GetFrameNumber (void)
  \brief       Get current USB Frame Number.
  \return      Frame Number
*/
static uint16_t USBD_GetFrameNumber (void) {

  if ((usbd_flags & USBD_DRIVER_FLAG_POWERED) == 0U) { return 0; }

  return ((LPC_USB->INFO & USB_INFO_FRAME_NR_MSK) >> USB_INFO_FRAME_NR_POS);
}

/**
  \fn          void USB_IRQHandler (void)
  \brief       USB0 Device Interrupt Routine (IRQ).
*/
void USB_IRQHandler (void) {
  uint32_t num, ep_idx, intstat, cmdstat, dev_evt = 0U;
  uint16_t val;
  EP const * ep;

  intstat = LPC_USB->INTSTAT & LPC_USB->INTEN;
  cmdstat = LPC_USB->DEVCMDSTAT;

  // Clear interrupt flags
  LPC_USB->INTSTAT = intstat;

  // Device Status interrupt
  if (intstat & USB_INT_DEV_INT) {

    // Reset
    if (cmdstat & USB_DEVCMDSTAT_DRES_C) {
      USBD_Reset ();
      usbd_state.active = 1U;
      usbd_state.speed  = ARM_USB_SPEED_FULL;
      LPC_USB->DEVCMDSTAT |= USB_DEVCMDSTAT_DRES_C | USB_DEVCMDSTAT_DEV_EN;
      dev_evt |= ARM_USBD_EVENT_RESET;
    }

    // Suspend
    if (cmdstat & USB_DEVCMDSTAT_DSUS) {
       usbd_state.active = 0U;
       LPC_USB->DEVCMDSTAT |= USB_DEVCMDSTAT_DSUS;
       dev_evt |= ARM_USBD_EVENT_SUSPEND;
    }

#if (USBD_VBUS_DETECT == 1)
    // Disconnect
    if (cmdstat & USB_DEVCMDSTAT_DCON_C) {
      usbd_state.active = 0U;
      usbd_state.vbus   = 0U;
      LPC_USB->DEVCMDSTAT |= USB_DEVCMDSTAT_DCON_C;
      dev_evt |= ARM_USBD_EVENT_VBUS_OFF;
    }

    // VBUS De-bounced
    if (cmdstat & USB_DEVCMDSTAT_VBUS_DEBOUNCED) {
      usbd_state.vbus   = 1U;
      dev_evt |= ARM_USBD_EVENT_VBUS_ON;
    }
#endif

    if ((dev_evt != 0U) && (SignalDeviceEvent != NULL)) {
      SignalDeviceEvent (dev_evt);
    }
  }

  // Endpoint interrupt
  if (intstat & USB_INT_EP_MSK) {
    for (ep_idx = 0; ep_idx <= USBD_MAX_ENDPOINT_NUM * 2U; ep_idx += 2U) {

      if (intstat & (USB_INT_EP(ep_idx))) {

        // Clear Interrupt status
        LPC_USB->INTSTAT = (1 << ep_idx);
        // Setup Packet
        if ((ep_idx == 0U) && ((cmdstat & USB_DEVCMDSTAT_SETUP) != 0U)) {
          ep_cmd[0].stall = 0U;
          ep_cmd[1].stall = 0U;
          ep_cmd[2].stall = 0U;
          ep_cmd[3].stall = 0U;

          LPC_USB->DEVCMDSTAT |= USB_DEVCMDSTAT_SETUP;
          memcpy(setup_packet, ep_buf, 8);

          // Analyze Setup packet for SetAddress
          val = setup_packet[0] | (setup_packet[1] << 8);
          if (val == 0x0500U) {
            val = (setup_packet[2] | (setup_packet[3] << 8)) & USB_DEVCMDSTAT_DEV_ADDR_MSK;
            // Set device address
            LPC_USB->DEVCMDSTAT = (LPC_USB->DEVCMDSTAT & ~USB_DEVCMDSTAT_DEV_ADDR_MSK) |
                                   USB_DEVCMDSTAT_DEV_ADDR(val) | USB_DEVCMDSTAT_DEV_EN;
          }

          setup_received = 1U;
          if (SignalEndpointEvent != NULL) {
            SignalEndpointEvent(0U, ARM_USBD_EVENT_SETUP);
          }
        } else {
        // OUT Packet
          ep = &endpoint[ep_idx];

          num = ep->transfer->num_transferring - ep->cmd->NBytes;

          // Copy EP data
          memcpy (ep->transfer->buf, ep->buf, num);

          ep->transfer->buf                   += num;
          ep->transfer->num_transferred_total += num;

          // Check if all OUT data received:
          //  - data terminated with ZLP or short packet or
          //  - all required data received
          if ((ep->transfer->num_transferred_total == ep->transfer->num) || 
              (num == 0U) || (num != ep->transfer->max_packet_sz)) {

            if (SignalEndpointEvent != NULL) {
              SignalEndpointEvent(ep_idx / 2U, ARM_USBD_EVENT_OUT);
            }
          } else {
            // Remaining data to transfer
            num = ep->transfer->num - ep->transfer->num_transferred_total;
            if (num > ep->transfer->max_packet_sz) { num = ep->transfer->max_packet_sz; }

            ep->transfer->num_transferring = num;
            ep->cmd->NBytes                = num;
            ep->cmd->buff_addr_offset      = ep->buf_offset >> 6;

            // Activate EP to receive next packet
            ep->cmd->active = 1U;
          }
        }
      }
    }

    // IN Packet
    for (ep_idx = 1; ep_idx <= USBD_MAX_ENDPOINT_NUM * 2U; ep_idx += 2U) {

      if (intstat & (USB_INT_EP(ep_idx))) {
        // Clear Interrupt status
        LPC_USB->INTSTAT = (1 << ep_idx);

        ep = &endpoint[ep_idx];

        ep->transfer->buf                   += ep->transfer->num_transferring;
        ep->transfer->num_transferred_total += ep->transfer->num_transferring;

        if (ep->transfer->num_transferred_total == ep->transfer->num) {
          // All data has been transfered
          if (SignalEndpointEvent != NULL) {
            SignalEndpointEvent(0x80 | (ep_idx / 2), ARM_USBD_EVENT_IN);
          }
        } else {
          // Still data to transfer
          num = ep->transfer->num - ep->transfer->num_transferred_total;
          if (num > ep->transfer->max_packet_sz) {
            // Remaining data bigger than max packet
            num = ep->transfer->max_packet_sz;
          }

          ep->transfer->num_transferring = num;

          // Copy data into IN Endpoint buffer
          memcpy (ep->buf, ep->transfer->buf, num);

          ep->cmd->buff_addr_offset = ep->buf_offset >> 6;

          // Set number of bytes to send
          ep->cmd->NBytes = num;

          // Activate EP to send next packet
          ep->cmd->active = 1U;
        }
      }
    }
  }
}

ARM_DRIVER_USBD Driver_USBD0 = {
  USBD_GetVersion,
  USBD_GetCapabilities,
  USBD_Initialize,
  USBD_Uninitialize,
  USBD_PowerControl,
  USBD_DeviceConnect,
  USBD_DeviceDisconnect,
  USBD_DeviceGetState,
  USBD_DeviceRemoteWakeup,
  USBD_DeviceSetAddress,
  USBD_ReadSetupPacket,
  USBD_EndpointConfigure,
  USBD_EndpointUnconfigure,
  USBD_EndpointStall,
  USBD_EndpointTransfer,
  USBD_EndpointTransferGetResult,
  USBD_EndpointTransferAbort,
  USBD_GetFrameNumber
};
