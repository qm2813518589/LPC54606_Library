/*-----------------------------------------------------------------------------
 * Name:    LED_LPCXpresso54102.c
 * Purpose: LED interface for LPCXpresso54102 evaluation board
 * Rev.:    1.0.1
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
#include "Board_LED.h"                  // ::Board Support:LED

// LED Pins
static const PINMUX_GRP_T LED_pin[] = {
  { 0U, 29U, IOCON_FUNC0 | IOCON_DIGITAL_EN}, // red
  { 0U, 30U, IOCON_FUNC0 | IOCON_DIGITAL_EN}, // green
  { 0U, 31U, IOCON_FUNC0 | IOCON_DIGITAL_EN}  // blue
};

#define LED_COUNT (sizeof(LED_pin)/sizeof(PINMUX_GRP_T))

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
  uint32_t i;

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

  // Enable GPIO0 and GPIO1 clock
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO0);

  // Configure Pin multiplexer
  Chip_IOCON_SetPinMuxing (LPC_IOCON, LED_pin, LED_COUNT);

  for (i = 0U; i < LED_COUNT; i++) {
    // Set Outpout high - (LED off)
    Chip_GPIO_SetPinOutHigh (LPC_GPIO, LED_pin[i].port, LED_pin[i].pin);

    // Set pins as Outputs
    Chip_GPIO_SetPinDIROutput (LPC_GPIO, LED_pin[i].port, LED_pin[i].pin);
  }

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize (void) {
  uint32_t i;

  for (i = 0U; i < LED_COUNT; i++) {
    // Unconfigure pins
    Chip_IOCON_PinMuxSet (LPC_IOCON, LED_pin[i].port, LED_pin[i].pin, 0);

    // Set Outpout low
    Chip_GPIO_SetPinOutLow (LPC_GPIO, LED_pin[i].port, LED_pin[i].pin);

    // Set pins as Inputs
    Chip_GPIO_SetPinDIRInput (LPC_GPIO, LED_pin[i].port, LED_pin[i].pin);
  }

  return 0;
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    Chip_GPIO_SetPinOutLow (LPC_GPIO, LED_pin[num].port, LED_pin[num].pin);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    Chip_GPIO_SetPinOutHigh (LPC_GPIO, LED_pin[num].port, LED_pin[num].pin);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut(uint32_t value) {
  uint32_t i;

  for (i = 0U; i < LED_COUNT; i++) {
    if (value & (1U << i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {

  return LED_COUNT;
}

