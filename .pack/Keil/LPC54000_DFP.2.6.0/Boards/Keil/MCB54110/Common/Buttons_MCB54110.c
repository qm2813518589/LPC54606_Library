/*-----------------------------------------------------------------------------
 * Name:    Buttons_MCB54110.c
 * Purpose: Buttons interface for MCB54110 evaluation board
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

#include "Board_Buttons.h"
#include "Driver_I2C.h"
#include "chip.h"

#define BUTTONS_COUNT                  (3)
#define BUTTON_ISP0                    (1 << 0)
#define BUTTON_ISP1                    (1 << 1)
#define BUTTON_WAKEUP                  (1 << 2)

/* Button pins:
   - BUTTON0: ISP0   = P0.31
   - BUTTON1: ISP1   = P0.4
   - BUTTON2: WAKEUP = P0.24 */

/* Input/output pin description */
typedef struct io_pin {
  uint8_t port;
  uint8_t pin;
} IO_PIN;

/* IOCON setting for GPIO pin */
#define PIN_IOCON_GPIO       (IOCON_FUNC0       | \
                              IOCON_MODE_PULLUP | \
                              IOCON_DIGITAL_EN  | \
                              IOCON_INPFILT_OFF)

/* Button GPIO definitions */
const IO_PIN button[] = {
  { 0, 31 },
  { 0, 4  },
  { 0, 24 }
};


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
  uint32_t n;

  /* Enable IO clocks */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO0);

  /* Configure button pins as inputs with pull up enabled */
  for (n = 0; n < BUTTONS_COUNT; n++) {
    Chip_IOCON_PinMuxSet (LPC_IOCON, button[n].port, button[n].pin, PIN_IOCON_GPIO);
    Chip_GPIO_SetPinDIRInput (LPC_GPIO, button[n].port, button[n].pin);
  }

  return 0;
}

/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) {
  uint32_t n;

  /* Reset button pins to default state */
  for (n = 0; n < BUTTONS_COUNT; n++) {
    Chip_IOCON_PinMuxSet (LPC_IOCON, button[n].port, button[n].pin, PIN_IOCON_GPIO);
    Chip_GPIO_SetPinDIRInput (LPC_GPIO, button[n].port, button[n].pin);
  }

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {
  uint32_t val;

  val = 0;
  if (!(Chip_GPIO_GetPinState (LPC_GPIO, button[0].port, button[0].pin))) { val |= BUTTON_ISP0; }
  if (!(Chip_GPIO_GetPinState (LPC_GPIO, button[1].port, button[1].pin))) { val |= BUTTON_ISP1; }
  if (!(Chip_GPIO_GetPinState (LPC_GPIO, button[2].port, button[2].pin))) { val |= BUTTON_WAKEUP; }

  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return BUTTONS_COUNT;
}
