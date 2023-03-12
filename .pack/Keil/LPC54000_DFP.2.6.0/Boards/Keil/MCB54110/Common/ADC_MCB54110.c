/*-----------------------------------------------------------------------------
 * Name:    ADC_MCB54110.c
 * Purpose: A/D Converter interface for MCB54110 evaluation board
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

#include "Board_ADC.h"
#include "chip.h"

#define ADC_RESOLUTION        12        /* Number of A/D converter bits       */


/* IOCON settings for GPIO pin */
#define PIN_IOCON_GPIO       (IOCON_FUNC0      | \
                              IOCON_ANALOG_EN  | \
                              IOCON_INPFILT_OFF)

#define PIN_IOCON_GPIO_RST   (IOCON_FUNC0       | \
                              IOCON_MODE_PULLUP | \
                              IOCON_DIGITAL_EN  | \
                              IOCON_INPFILT_OFF)

static volatile uint16_t AD_last;       /* Last converted value               */
static volatile uint8_t  AD_done;       /* AD conversion done flag            */


/**
  \fn          int32_t ADC_Initialize (void)
  \brief       Initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC_Initialize (void) {

  /* Configure PIO0_30/ADC0_1 pin for analog operation */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO0);

  Chip_IOCON_PinMuxSet (LPC_IOCON, 0, 30, PIN_IOCON_GPIO);

  /* Configure ADC for 12 bit conversion and operation at system clock */
  Chip_ADC_Init (LPC_ADC, ADC_CR_RESOL(3) | ADC_CR_CLKDIV(0));

  /* Enable ADC Channel 1 conversion in Sequence A */
  Chip_ADC_SetSequencerBits(LPC_ADC, ADC_SEQA_IDX, ADC_SEQ_CTRL_SEQ_ENA | ADC_SEQ_CTRL_CHANNEL_EN(1));

  /* Enable Sequence A interrupt */
  Chip_ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQA_ENABLE);

  /* Trigger calibration */
  Chip_ADC_Calibration(LPC_ADC);

  /* Enable ADC Sequencer A interrupt in NVIC */
  NVIC_EnableIRQ(ADC_SEQA_IRQn);

  return 0;
}

/**
  \fn          int32_t ADC_Uninitialize (void)
  \brief       De-initialize Analog-to-Digital Converter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC_Uninitialize (void) {

  /* Disable ADC Sequencer A interrupts in NVIC */
  NVIC_EnableIRQ (ADC_SEQA_IRQn);
  
  /* Uninit ADC */
  Chip_ADC_DeInit (LPC_ADC);

  /* Reset pin PIO0_30 to default state */
  Chip_IOCON_PinMuxSet (LPC_IOCON, 0, 30, PIN_IOCON_GPIO_RST);

  return 0;
}

/**
  \fn          int32_t ADC_StartConversion (void)
  \brief       Start conversion
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t ADC_StartConversion (void) {

  Chip_ADC_StartSequencer (LPC_ADC, ADC_SEQA_IDX);

  return 0;
}

/**
  \fn          int32_t ADC_ConversionDone (void)
  \brief       Check if conversion finished
  \returns
   - \b  0: conversion finished
   - \b -1: conversion in progress
*/
int32_t ADC_ConversionDone (void) {
  return (AD_done ? 0 : -1);
}

/**
  \fn          int32_t ADC_GetValue (void)
  \brief       Get converted value
  \returns
   - <b> >=0</b>: converted value
   - \b -1: conversion in progress or failed
*/
int32_t ADC_GetValue (void) {

  if (AD_done) {
    AD_done = 0;
    return AD_last;
  }
  return -1;
}

/**
  \fn          uint32_t ADC_GetResolution (void)
  \brief       Get resolution of Analog-to-Digital Converter
  \returns     Resolution (in bits)
*/
uint32_t ADC_GetResolution (void) {
  return ADC_RESOLUTION;
}

/**
  \fn          ADC_SEQA_IRQHandler (void)
  \brief       Analog-to-Digital Sequencer A Interrupt Handler
*/
void ADC_SEQA_IRQHandler(void) {

  /* Read global data register */
  AD_last = (Chip_ADC_GetGlobalDataReg(LPC_ADC, ADC_SEQA_IDX) & ADC_SEQ_GDAT_RESULT_MASK) >> ADC_SEQ_GDAT_RESULT_BITPOS;
  AD_done = 1;
}
