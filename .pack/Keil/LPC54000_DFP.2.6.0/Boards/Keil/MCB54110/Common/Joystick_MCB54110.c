/*-----------------------------------------------------------------------------
 * Name:    Joystick_MCB54110.c
 * Purpose: Joystick interface for MCB54110 evaluation board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2013 - 2016 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "Board_Joystick.h"
#include "Driver_I2C.h"
#include "chip.h"

#define JOYSTICK_COUNT                 (5)

/* I2C Interface: I2C4 (Flexcomm Interface 4)
  I2C pins:
   - SDA: P0_26 (FC4_CTS_SDA_SSEL0)
   - SCL: P0_25 (FC4_RTS_SCL_SSEL1)

  Joystick pins:
   - up:     IOEXP = P00
   - left:   IOEXP = P01
   - down:   IOEXP = P02
   - right:  IOEXP = P03
   - center: IOEXP = P04   */


/* I2C Driver */
extern ARM_DRIVER_I2C         Driver_I2C4;
#define ptrI2C              (&Driver_I2C4)

#define IOEXPANDER_I2C_ADDR   0x74

/* LED related IO expander registers */
#define IOEXP_P0_INPUT        0x00
#define IOEXP_P0_OUTPUT       0x02
#define IOEXP_P0_POLINV       0x04
#define IOEXP_P0_CONFIG       0x06

/**
  \fn          int32_t IOExpander_Read (uint8_t reg, uint8_t *val)
  \brief       Read value from IO expander register
  \param[in]   reg    Register address
  \param[out]  val    Pointer where data will be read from register
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t IOExpander_Read (uint8_t reg, uint8_t *val) {
  uint8_t data[1];

  data[0] = reg;
  ptrI2C->MasterTransmit(IOEXPANDER_I2C_ADDR, data, 1, true);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 1) { return -1; }
  ptrI2C->MasterReceive (IOEXPANDER_I2C_ADDR, val, 1, false);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 1) { return -1; }

  return 0;
}

/**
  \fn          int32_t IOExpander_Write (uint8_t reg, const uint8_t *val)
  \brief       Write value to IO expander register
  \param[in]   reg    Register address
  \param[in]   val    Pointer with data to write to register
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t IOExpander_Write (uint8_t reg, uint8_t val) {
  uint8_t data[2];

  data[0] = reg;
  data[1] = val;
  ptrI2C->MasterTransmit(IOEXPANDER_I2C_ADDR, data, 2, false);
  while (ptrI2C->GetStatus().busy);
  if (ptrI2C->GetDataCount() != 2) { return -1; }

  return 0;
}


/**
  \fn          int32_t Joystick_Initialize (void)
  \brief       Initialize joystick
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Joystick_Initialize (void) {
  uint8_t val;

  /* Configure IO expander pins as outputs set to high state */
  ptrI2C->Initialize  (NULL);
  ptrI2C->PowerControl(ARM_POWER_FULL);
  ptrI2C->Control     (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  ptrI2C->Control     (ARM_I2C_BUS_CLEAR, 0);

  IOExpander_Read  (IOEXP_P0_CONFIG, &val);
  IOExpander_Write (IOEXP_P0_CONFIG, val | 0x0F);

  return 0;
}

/**
  \fn          int32_t Joystick_Uninitialize (void)
  \brief       De-initialize joystick
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Joystick_Uninitialize (void) {
  return 0;
}

/**
  \fn          uint32_t Joystick_GetState (void)
  \brief       Get joystick state
  \returns     Joystick state
*/
uint32_t Joystick_GetState (void) {
  uint32_t state;
  uint8_t  val;
  state  = 0;
  
  if (IOExpander_Read (IOEXP_P0_INPUT, &val) == 0) {
    if ((val & 0x01) == 0) { state |= JOYSTICK_UP;     }
    if ((val & 0x02) == 0) { state |= JOYSTICK_LEFT;   }
    if ((val & 0x04) == 0) { state |= JOYSTICK_DOWN;   }
    if ((val & 0x08) == 0) { state |= JOYSTICK_RIGHT;  }
    if ((val & 0x10) == 0) { state |= JOYSTICK_CENTER; }
  }

  return state;
}
