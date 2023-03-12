/*-----------------------------------------------------------------------------
 * Name:    Buttons_LPCXpresso54608.c
 * Purpose: Button interface for LPCXpresso54608 evaluation board
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
#include "Board_Buttons.h"


#define IOCON_FUNC0             (0x0U)				/* Select pin function 0 */
#define IOCON_MODE_PULLUP       (0x2U << 4)		/* Select pull-up function */
#define IOCON_GPIO_MODE         (0x1U << 6)		/* GPIO Mode */
#define IOCON_DIGITAL_EN        (0x1U << 8)		/* Enable digital function by setting 1 to bit 7(default) */

typedef struct {
	uint32_t port     :  8;			/* Pin port */
	uint32_t pin      :  8;			/* Pin number */
	uint32_t modefunc : 16;		  /* Function and mode */
} PIN_T;

// Button Pins
static const PIN_T Button_pin[] = {
  { 1U,  1U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}, // USER SW5
  { 0U,  4U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}, // ISP0 SW4
  { 0U,  5U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}, // ISP1 SW3
  { 0U,  6U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_GPIO_MODE | IOCON_DIGITAL_EN}  // ISP2 SW2
};

#define BUTTON_NUM (sizeof(Button_pin)/sizeof(PIN_T))


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
  uint32_t i;

  SYSCON->AHBCLKCTRLSET[0] =  (1UL << 13);     /* enable IOCON clock */

  // Enable GPIO0 and GPIO1 clock
  SYSCON->AHBCLKCTRLSET[0] = ((1UL << 14) |    /* enable GPIO0 clock */
                              (1UL << 15)  );  /* enable GPIO1 clock */

  for (i = 0U; i < BUTTON_NUM; i++) {
    // Configure Pin multiplexer
    IOCON->PIO[Button_pin[i].port][Button_pin[i].pin] = Button_pin[i].modefunc;

    // Set pins as Inputs
    GPIO->DIR[Button_pin[i].port] &= ~(1UL << Button_pin[i].pin);
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
    IOCON->PIO[Button_pin[i].port][Button_pin[i].pin] = 0U;
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
    if (GPIO->B[ Button_pin[i].port][Button_pin[i].pin] == 0) {
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
