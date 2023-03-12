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
 * $Date:        2. February 2015
 * $Revision:    V1.0
 *
 * Project:      USB Driver Definitions for NXP LPC541x
 * -------------------------------------------------------------------------- */

#ifndef __USB_LPC541X_H
#define __USB_LPC541X_H

#include <stdint.h>

// USB Peripheral Register bit definitions

// USB Device Command/Status register
#define USB_DEVCMDSTAT_DEV_ADDR_POS       (0U      )
#define USB_DEVCMDSTAT_DEV_ADDR_MSK       (0x7FU   )
#define USB_DEVCMDSTAT_DEV_ADDR(addr)     (((addr) << USB_DEVCMDSTAT_DEV_ADDR_POS) & USB_DEVCMDSTAT_DEV_ADDR_MSK)
#define USB_DEVCMDSTAT_DEV_EN             (1U << 7 )
#define USB_DEVCMDSTAT_SETUP              (1U << 8 )
#define USB_DEVCMDSTAT_FORCE_NEEDCLK      (1U << 9 )
#define USB_DEVCMDSTAT_LPM_SUP            (1U << 11)
#define USB_DEVCMDSTAT_INTONNAK_AO        (1U << 12)
#define USB_DEVCMDSTAT_INTONNAK_AI        (1U << 13)
#define USB_DEVCMDSTAT_INTONNAK_CO        (1U << 14)
#define USB_DEVCMDSTAT_INTONNAK_CI        (1U << 15)
#define USB_DEVCMDSTAT_DCON               (1U << 16)
#define USB_DEVCMDSTAT_DSUS               (1U << 17)
#define USB_DEVCMDSTAT_LPM_SUS            (1U << 19)
#define USB_DEVCMDSTAT_LPM_REWP           (1U << 20)
#define USB_DEVCMDSTAT_DCON_C             (1U << 24)
#define USB_DEVCMDSTAT_DSUS_C             (1U << 25)
#define USB_DEVCMDSTAT_DRES_C             (1U << 26)
#define USB_DEVCMDSTAT_VBUS_DEBOUNCED     (1U << 28)

// USB Device Interrupt (status, enable, set) register
#define USB_INT_EP_MSK                    (0x03FFU)
#define USB_INT_EP(ep_idx)                ((1U << (ep_idx)) & USB_INT_EP_MSK)
#define USB_INT_FRAME_INT                 (1U << 30)
#define USB_INT_DEV_INT                   (1U << 31)

// USB Device Endpoint Skip register
#define USB_EPSKIP_SKIP(ep_idx)           (1U << (ep_idx))

//USB Device Info register
#define USB_INFO_FRAME_NR_POS             (0U)
#define USB_INFO_FRAME_NR_MSK             (0x07FF << USB_INFO_FRAME_NR_POS)
#define USB_INFO_ERR_CODE_POS             (11U)
#define USB_INFO_ERR_CODE_MSK             (0x0FU << USB_INFO_ERR_CODE_POS)

// USB Driver State Flags
// Device State Flags
#define USBD_DRIVER_FLAG_INITIALIZED      (1U      )
#define USBD_DRIVER_FLAG_POWERED          (1U << 1 )

// Transfer information structure
typedef struct {
  uint32_t max_packet_sz;
  uint32_t num;
  uint32_t num_transferred_total;
  uint32_t num_transferring;
  uint8_t *buf;
} EP_TRANSFER;

// Endpoint command/status
typedef struct {
  uint32_t buff_addr_offset     : 16;
  uint32_t NBytes               : 10;
  uint32_t ep_type_isoch        : 1;
  uint32_t toggle_value         : 1;
  uint32_t toggle_reset         : 1;
  uint32_t stall                : 1;
  uint32_t ep_disabled          : 1;
  uint32_t active               : 1;
} EP_CMD;

// Endpoint structure
typedef struct __EP {
  EP_CMD      * const cmd;
  uint8_t     * const buf;
  EP_TRANSFER * const transfer;
  uint16_t            buf_offset;
} EP;

#endif /* __USB_LPC541X_H */
