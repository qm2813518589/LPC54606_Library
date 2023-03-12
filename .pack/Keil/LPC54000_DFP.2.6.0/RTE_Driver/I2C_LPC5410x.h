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
 * Project:      I2C Driver Definitions for NXP LPC5410x
 * -------------------------------------------------------------------------- */

#ifndef __I2C_LPC5410X_H
#define __I2C_LPC5410X_H

#include <string.h>

#include "chip.h"
#include "Driver_I2C.h"

#include "RTE_Components.h"
#include "RTE_Device.h"

#if ((defined(RTE_Drivers_I2C0) || \
      defined(RTE_Drivers_I2C1) || \
      defined(RTE_Drivers_I2C2))   \
     && (RTE_I2C0 == 0)            \
     && (RTE_I2C1 == 0)            \
     && (RTE_I2C2 == 0))

#error "I2C not configured in RTE_Device.h!"
#endif

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
  uint8_t               ctrl;               // Transfer control flags
} I2C_TRANSFER_INFO;

/* I2C Control Information */
typedef struct {
  ARM_I2C_SignalEvent_t cb_event;           // Event callback
  ARM_I2C_STATUS        status;             // Status flags
  I2C_TRANSFER_INFO     xfer;
  uint8_t               flags;              // Control and state flags
} I2C_INFO;

/* I2C Pin Description */
typedef struct i2c_pin {
  uint8_t port;                             // Port number
  uint8_t pin;                              // Pin number
  uint8_t func;                             // Pin function
} I2C_PIN;

/* I2C Resource Configuration */
typedef struct {
  LPC_I2C_T            *reg;                // I2C register interface
  IRQn_Type             irqn;               // I2C Event IRQ Number
  CHIP_SYSCON_CLOCK_T   clk_msk;            // APB clock control bit mask
  I2C_PIN               scl;                // I2C SCL pin
  I2C_PIN               sda;                // I2C SDA pin
  I2C_INFO             *info;               // Run-Time control information
} const I2C_RESOURCES;

#endif /* __I2C_LPC5410X_H */
