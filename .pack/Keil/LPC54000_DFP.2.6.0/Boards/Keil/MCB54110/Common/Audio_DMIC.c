/*-----------------------------------------------------------------------------
 * Name:    Audio_DMIC.c
 * Purpose: Audio LPC5411x DMIC interface
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

/*
 * System PLL output is used as DMIC clock soure.
 * System PLL must be externaly configuted by the user!
 */

#include "Board_Audio.h"
#include "chip.h"

// User Definitions
// Can be user defined by C preprocessor

// Maximum tolerance of DMIC sample rate in %
#ifndef DMIC_SAMLE_RATE_MAX_TOLERANCE
#define DMIC_SAMLE_RATE_MAX_TOLERANCE   (1)
#endif

// Oversample rate
#ifndef DMIC_OSR
#define DMIC_OSR                        (16)
#endif

// DMIC FIFO trigger lelvel 0 .. 15
#ifndef DMIC_FIFO_TRIGLVL
#define DMIC_FIFO_TRIGLVL               (7)
#endif

static const PINMUX_GRP_T dmic_io[] = {
  {1, 15, (IOCON_FUNC1 | IOCON_MODE_INACT  | IOCON_DIGITAL_EN)}, /* PDM CLK */
  {1, 16, (IOCON_FUNC1 | IOCON_MODE_INACT  | IOCON_DIGITAL_EN)}, /* PDM DATA */
};

static struct  {
  uint16_t * buf;
  uint32_t   cnt;
  uint32_t   num;
} volatile DMIC_Data = {0};

static DMIC_CHANNEL_CONFIG_T dmic_channel_cfg;
static Audio_SignalEvent_t Callback_event;

static bool dmic_mute = 0;


/**
  \fn          void DMIC_IRQHandler (void)
  \brief       DMIC interrupt handler
*/
void DMIC_IRQHandler (void) {
  uint32_t event = 0;
  uint32_t status, num, i;

  status = Chip_DMIC_FifoGetStatus (LPC_DMIC, 0);
  Chip_DMIC_FifoClearStatus (LPC_DMIC, 0, status);

  if (status & (1 << DMIC_FIFO_INT_P)) {
    if (DMIC_Data.buf != NULL) {
      num = DMIC_Data.num - DMIC_Data.cnt;
      
      for (i = 0; (i <= DMIC_FIFO_TRIGLVL) && (num > 0); num--, i++) {
        if (dmic_mute == true) {
          *(DMIC_Data.buf++) = 0;
          // Dummy read
          LPC_DMIC->CHANNEL[0].FIFO_DATA;
        } else {
          *(DMIC_Data.buf++) = (uint16_t)LPC_DMIC->CHANNEL[0].FIFO_DATA;
        }
        DMIC_Data.cnt++;
      }

      if (DMIC_Data.cnt == DMIC_Data.num) {
        DMIC_Data.buf = NULL;
        event |= AUDIO_EVENT_RECEIVE_COMPLETE;
      }
    }
  }

  if (status & (1 << DMIC_FIFO_OVERRUN_P)) {
    event |= AUDIO_EVENT_RX_OVERFLOW;
    // Dummy read
    LPC_DMIC->CHANNEL[0].FIFO_DATA;
  }

  if ((Callback_event != NULL) && (event != 0U)) {
    Callback_event (event);
  }
}

/**
  \fn          int32_t DMIC_SetSampleRate (uint32_t fs)
  \brief       Sets DMIC Sample Rate
  \param[in]   fs  sample rate
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t DMIC_SetSampleRate (uint32_t fs) {
  uint32_t clk, div, fs_cal, fs_min, fs_max;

  clk = Chip_Clock_GetSystemPLLOutClockRate (true);

  // Calculate PDM clock divider
  div = (((((clk << 1) + 1)) / (DMIC_OSR)) / fs) >> 3;
  if (div > 128) {
    // Max PDM clock dvider is 128
    return -1;
  }

  // Calculate actual sampling rate
  fs_cal = ((clk / div) / (DMIC_OSR)) >> 2;

  // Check if actual sampling rate is in limits
  fs_min = fs - DMIC_SAMLE_RATE_MAX_TOLERANCE;
  fs_max = fs + DMIC_SAMLE_RATE_MAX_TOLERANCE;
  if ((fs_cal < fs_min) || (fs_cal > fs_max)) {
    return -1;
  }

  LPC_SYSCON->DMICCLKDIV = div - 1;

  return 0;
}


// Driver functions

/**
  \fn          int32_t Audio_DMIC_Initialize (Audio_SignalEvent_t cb_event)
  \brief       Initialize Audio Interface
  \param[in]   cb_event  pointer to event notification function
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Initialize (Audio_SignalEvent_t cb_event) {
  uint32_t clk, div;

  // Save Callback 
  Callback_event = cb_event;

  // Enable clocks
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_INPUTMUX);

  // Configure DMIC Pins
  Chip_IOCON_SetPinMuxing(LPC_IOCON, dmic_io, sizeof(dmic_io) / sizeof(PINMUX_GRP_T));

  // DMIC clock soure is System PLL
  LPC_SYSCON->DMICCLKSEL = 2;

  clk = Chip_Clock_GetSystemPLLOutClockRate (true);

  // Max input frequency is 24.576MHz
  div = clk / 24576000;
  if (div != 0) { div--;}
  LPC_SYSCON->DMICCLKDIV = div;

  dmic_channel_cfg.side       = DMIC_LEFT;
  dmic_channel_cfg.gainshft   = 0;

  // Default PCM sample rate = 16000
  dmic_channel_cfg.osr        = DMIC_OSR;
  dmic_channel_cfg.divhfclk   = DMIC_PDM_DIV1;
  dmic_channel_cfg.preac2coef = DMIC_COMP0_0;
  dmic_channel_cfg.preac4coef = DMIC_COMP0_0;

  Chip_DMIC_Init(SYSCON_CLOCK_DMIC, RESET_DMIC);
  Chip_DMIC_CfgIO(LPC_DMIC, pdm_bypass_clk1);
  Chip_DMIC_SetOpMode(LPC_DMIC,DMIC_OP_INTR);
  Chip_DMIC_Use2fs(LPC_DMIC,false);
  Chip_DMIC_CfgChannel(LPC_DMIC, 0, &dmic_channel_cfg);

  // Default sample rate is 16 kHz
  if (DMIC_SetSampleRate (16000) == -1) {
    return -1;
  }

  Chip_DMIC_CfgChannelDc(LPC_DMIC, 0, DMIC_DC_CUT155,2,true);
  Chip_DMIC_FifoChannel (LPC_DMIC, 0, DMIC_FIFO_TRIGLVL, true, true);

  NVIC_EnableIRQ(DMIC_IRQn);

  return 0;
}

/**
  \fn          int32_t Audio_DMIC_Uninitialize (void)
  \brief       De-initialize Audio Interface
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Uninitialize (void) {

  // Disable DMIC interrupr in NVIC
  NVIC_DisableIRQ(DMIC_IRQn);

  // Disable all channels
  Chip_DMIC_EnableChannnel (LPC_DMIC, 0);

	// Reset DMIC
	Chip_SYSCON_PeriphReset(RESET_DMIC);

  // Disable DMIC peripheral clock
  Chip_Clock_DisablePeriphClock (SYSCON_CLOCK_DMIC);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_ReceiveData (void *data, uint32_t num)
  \brief       Start receiving data from Audio input stream
  \param[out]  data  pointer to buffer for data to receive
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_ReceiveData (void *data, uint32_t num) {
  if (DMIC_Data.buf == NULL) {
    DMIC_Data.buf = data;
    DMIC_Data.num = num;
    DMIC_Data.cnt = 0;

    return 0;
  }
  return -1;
}

/**
  \fn          uint32_t Audio_DMIC_GetDataRxCount (void)
  \brief       Get received data count
  \returns     number of data items received
*/
uint32_t Audio_DMIC_GetDataRxCount (void) {
  return DMIC_Data.cnt;
}

/**
  \fn          int32_t Audio_DMIC_Start (void)
  \brief       Start Audio stream
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Start (void) {
  Chip_DMIC_EnableChannnel (LPC_DMIC, 1);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_Stop (void)
  \brief       Stop Audio stream
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Stop (void) {
  Chip_DMIC_EnableChannnel (LPC_DMIC, 0);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_Pause ()
  \brief       Pause Audio stream
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Pause (void) {
  Chip_DMIC_EnableChannnel (LPC_DMIC, 0);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_Resume ()
  \brief       Resume Audio stream
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_Resume (void) {
  Chip_DMIC_EnableChannnel (LPC_DMIC, 1);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_SetVolume (uint8_t channel, uint8_t volume)
  \brief       Set volume level for Audio stream
  \param[in]   channel  channel identifier
  \param[in]   volume   volume level (0..100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_SetVolume (uint8_t channel, uint8_t volume) {
  // Scale in set gain
  LPC_DMIC->CHANNEL[0].GAINSHFT = ((volume * 31) / 100);
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_SetMute (uint8_t channel, bool mute)
  \brief       Set mute state for Audio stream
  \param[in]   channel  channel identifier
  \param[in]   mute     mute state
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_SetMute (uint8_t channel, bool mute) {
  dmic_mute = mute;
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_SetDataFormat (uint8_t format)
  \brief       Set Audio data format
  \param[in]   format  data format
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_SetDataFormat ( uint8_t format) {
  // Only 16-bit mono mode is supported
  if (format != AUDIO_DATA_16_STEREO) {
    return -1;
  }
  return 0;
}

/**
  \fn          int32_t Audio_DMIC_SetFrequency (uint32_t frequency)
  \brief       Set Audio stream frequency
  \param[in]   frequency  Audio frequency in Hz
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_DMIC_SetFrequency (uint32_t frequency) {
  return (DMIC_SetSampleRate (frequency));
}
