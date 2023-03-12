/*-----------------------------------------------------------------------------
 * Name:    LED_LPCXpresso54608.c
 * Purpose: LED interface for LPCXpresso54114 evaluation board
 * Rev.:    1.0.1
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2013 - 2017 ARM LIMITED

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

#include "LPC54608.h"                   // Device header
#include "Board_LED.h"                  // ::Board Support:LED


#define IOCON_FUNC0             (0x0U)				/* Selects pin function 0 */
#define IOCON_DIGITAL_EN        (0x1U << 8)		/* Enables digital function by setting 1 to bit 7(default) */

typedef struct {
	uint32_t port     :  8;			/* Pin port */
	uint32_t pin      :  8;			/* Pin number */
	uint32_t modefunc : 16;		  /* Function and mode */
} PIN_T;

// LED Pins
static const PIN_T LED_pin[] = {
  { 3U, 14U, IOCON_FUNC0 | IOCON_DIGITAL_EN}, /* LED1 */
  { 3U,  3U, IOCON_FUNC0 | IOCON_DIGITAL_EN}, /* LED2 */
  { 2U,  2U, IOCON_FUNC0 | IOCON_DIGITAL_EN}  /* LED3 */
};

#define LED_COUNT (sizeof(LED_pin)/sizeof(PIN_T))

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
  uint32_t i;

  SYSCON->AHBCLKCTRLSET[0] =  (1UL << 13);     /* enable IOCON clock */

  // Enable GPIO2 and GPIO3 clock
  SYSCON->AHBCLKCTRLSET[0] = ((1UL << 16) |    /* enable GPIO2 clock */
                              (1UL << 17)  );  /* enable GPIO3 clock */

  for (i = 0U; i < LED_COUNT; i++) {
    // Configure Pin multiplexer
    IOCON->PIO[LED_pin[i].port][LED_pin[i].pin] = LED_pin[i].modefunc;

    // Set Outpout high - (LED off)
    GPIO->SET[LED_pin[i].port]  =  (1UL << LED_pin[i].pin);

    // Set pins as Outputs
    GPIO->DIR[LED_pin[i].port] |=  (1UL << LED_pin[i].pin);
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
    IOCON->PIO[LED_pin[i].port][LED_pin[i].pin] = 0U;

    // Set Outpout low
    GPIO->CLR[LED_pin[i].port]  =  (1UL << LED_pin[i].pin);

    // Set pins as Inputs
    GPIO->DIR[LED_pin[i].port] &= ~(1UL << LED_pin[i].pin);
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
    GPIO->CLR[LED_pin[num].port] = (1UL << LED_pin[num].pin);
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
    GPIO->SET[LED_pin[num].port] = (1UL << LED_pin[num].pin);
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

