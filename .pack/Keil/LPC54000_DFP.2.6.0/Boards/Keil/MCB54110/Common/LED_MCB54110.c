/*-----------------------------------------------------------------------------
 * Name:    LED_MCB54110.c
 * Purpose: LED interface for MCB54110 evaluation board
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

#include "Board_LED.h"
#include "Driver_I2C.h"
#include "chip.h"

#define LED_COUNT                      (8)

/* I2C Interface: I2C4 (Flexcomm Interface 4)
  I2C pins:
   - SDA: P0_26 (FC4_CTS_SDA_SSEL0)
   - SCL: P0_25 (FC4_RTS_SCL_SSEL1)

  LED pins:
   - LED1: P1_7
   - LED2: P1_8
   - LED3: P1_9
   - LED4: P1_10

   - LED5: IOEXP P10
   - LED5: IOEXP P11
   - LED6: IOEXP P12
   - LED7: IOEXP P13 */

#define LED_COUNT_IO                   (4)
#define LED_COUNT_IOEXP                (4)

/* Input/output pin description */
typedef struct io_pin {
  uint8_t port;
  uint8_t pin;
} IO_PIN;

/* IOCON settings for GPIO pin */
#define PIN_IOCON_GPIO       (IOCON_FUNC0      | \
                              IOCON_DIGITAL_EN | \
                              IOCON_INPFILT_OFF)

#define PIN_IOCON_GPIO_RST   (IOCON_FUNC0       | \
                              IOCON_MODE_PULLUP | \
                              IOCON_DIGITAL_EN  | \
                              IOCON_INPFILT_OFF)

/* LED GPIO definitions */
const IO_PIN led[] = {
  { 1, 7  },
  { 1, 8  },
  { 1, 9  },
  { 1, 10 },
};

/* I2C Driver */
extern ARM_DRIVER_I2C         Driver_I2C4;
#define ptrI2C              (&Driver_I2C4)

#define IOEXPANDER_I2C_ADDR   0x74

/* LED related IO expander registers */
#define IOEXP_P1_INPUT        0x01
#define IOEXP_P1_OUTPUT       0x03
#define IOEXP_P1_POLINV       0x05
#define IOEXP_P1_CONFIG       0x07

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
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
  uint32_t n;
  uint8_t  val;

  /* Enable IO clocks */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO1);

  /* Configure LED pins as outputs set to high state */
  for (n = 0; n < LED_COUNT_IO; n++) {
    Chip_IOCON_PinMuxSet (LPC_IOCON, led[n].port, led[n].pin, PIN_IOCON_GPIO);
    Chip_GPIO_SetPinState (LPC_GPIO, led[n].port, led[n].pin, true);
    Chip_GPIO_SetPinDIROutput (LPC_GPIO, led[n].port, led[n].pin);
  }

  /* Configure IO expander pins as outputs set to high state */
  ptrI2C->Initialize  (NULL);
  ptrI2C->PowerControl(ARM_POWER_FULL);
  ptrI2C->Control     (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
  ptrI2C->Control     (ARM_I2C_BUS_CLEAR, 0);

  IOExpander_Read  (IOEXP_P1_CONFIG, &val);
  IOExpander_Write (IOEXP_P1_CONFIG, val & ~0x0F);

  IOExpander_Read  (IOEXP_P1_OUTPUT, &val);
  IOExpander_Write (IOEXP_P1_OUTPUT, val | 0x0F);

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
  uint32_t n;
  uint8_t  val;

  /* Reset LED pins to default state */
  for (n = 0; n < LED_COUNT_IO; n++) {
    Chip_GPIO_SetPinDIRInput (LPC_GPIO, led[n].port, led[n].pin);
    Chip_IOCON_PinMuxSet (LPC_IOCON, led[n].port, led[n].pin, PIN_IOCON_GPIO_RST);
  }

  /* Reset IO expander register values to default */
  IOExpander_Read  (IOEXP_P1_CONFIG, &val);
  IOExpander_Write (IOEXP_P1_CONFIG, val | 0x0F);

  IOExpander_Read  (IOEXP_P1_OUTPUT, &val);
  IOExpander_Write (IOEXP_P1_OUTPUT, val | 0x0F);

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
  uint8_t val;

  if (num < LED_COUNT_IO) {
    Chip_GPIO_SetPinState (LPC_GPIO, led[num].port, led[num].pin, false);
  }
  else if (num < LED_COUNT) {
    IOExpander_Read  (IOEXP_P1_OUTPUT, &val);
    IOExpander_Write (IOEXP_P1_OUTPUT, val & ~(1U << (7-num)));
  }
  else {
    return -1;
  }

  return 0;
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
  uint8_t val;

  if (num < LED_COUNT_IO) {
    Chip_GPIO_SetPinState (LPC_GPIO, led[num].port, led[num].pin, true);
  }
  else if (num < LED_COUNT) {
    IOExpander_Read  (IOEXP_P1_OUTPUT, &val);
    IOExpander_Write (IOEXP_P1_OUTPUT, val | (1U << (7-num)));
  }
  else {
    return -1;
  }

  return 0;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < LED_COUNT; n++) {
    if (val & (1 << n)) { LED_On (n); }
    else                { LED_Off(n); }
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
