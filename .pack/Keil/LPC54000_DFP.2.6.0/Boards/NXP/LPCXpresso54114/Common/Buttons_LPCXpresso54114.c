/*-----------------------------------------------------------------------------
 * Name:    Buttons_LPCXpresso54114.c
 * Purpose: Button interface for LPCXpresso54114 evaluation board
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

#include "chip.h"                       // Device header
#include "Board_Buttons.h"

// Button Pins
static const PINMUX_GRP_T Button_pin[] = {
  { 0U, 24U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}, // WAKE SW1
  { 0U, 31U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}, // ISP0 SW2
  { 0U, 04U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}  // ISP1 SW3
};

#define BUTTON_NUM (sizeof(Button_pin)/sizeof(PINMUX_GRP_T))


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
  uint32_t i;

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

  // Enable GPIO0 clock
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO0);

  // Configure Pin multiplexer
  Chip_IOCON_SetPinMuxing (LPC_IOCON, Button_pin, BUTTON_NUM);

  for (i = 0U; i < BUTTON_NUM; i++) {
    // Set pins as Inputs
    Chip_GPIO_SetPinDIRInput (LPC_GPIO, Button_pin[i].port, Button_pin[i].pin);
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
  uint32_t i;

  // Unconfigure Pins
  for (i = 0U; i < BUTTON_NUM; i++) {
    Chip_IOCON_PinMuxSet (LPC_IOCON, Button_pin[i].port, Button_pin[i].pin, 0);
  }

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {
  uint32_t i, val;

  val = 0U;
  for (i = 0U; i < BUTTON_NUM; i++) {
    if (Chip_GPIO_GetPinState (LPC_GPIO, Button_pin[i].port, Button_pin[i].pin) == 0) {
      val |= (1U << i);
    }
  }
  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return BUTTON_NUM;
}
