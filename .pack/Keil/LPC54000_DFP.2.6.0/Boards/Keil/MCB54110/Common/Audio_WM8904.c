/*-----------------------------------------------------------------------------
 * Name:    Audio_WM8904.c
 * Purpose: Audio interface for MCB54110 Board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2016 ARM LIMITED

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

#include "cmsis_os.h"
#include "chip.h"
#include "Driver_I2C.h"
#include "Driver_SAI.h"
#include "Board_Audio.h"
#include "RTE_Components.h"             // Component selection


#define WM8904_OUTPUT_HPOUT   (1)
#define WM8904_OUTPUT_SPEAKER (2)

#ifndef WM8904_OUTPUT
#define WM8904_OUTPUT (WM8904_OUTPUT_HPOUT | WM8904_OUTPUT_SPEAKER)
#endif

// Maximum tolerance of SYSCLK/fs ratio in ppm
#ifndef WM8904_CLK_SYS_RAT_MAX_TOLERANCE
#define WM8904_CLK_SYS_RAT_MAX_TOLERANCE   (1000)
#endif

#ifndef WM8904_I2C_PORT
#define WM8904_I2C_PORT          4         // I2C Port number
#endif

#define WM8904_I2C_ADDR          (0x1A >> 0)      // I2C address

// I2C Driver
#define _I2C_Driver_(n)  Driver_I2C##n
#define  I2C_Driver_(n) _I2C_Driver_(n)
extern ARM_DRIVER_I2C    I2C_Driver_(WM8904_I2C_PORT);
#define ptrI2C         (&I2C_Driver_(WM8904_I2C_PORT))
static uint8_t  DeviceAddr = WM8904_I2C_ADDR;

// SAI
#ifndef WM8904_SAI_PORT_OUT
#define WM8904_SAI_PORT_OUT    0         // SAI Port number
#endif

#ifndef WM8904_SAI_PORT_IN
#define WM8904_SAI_PORT_IN     1         // SAI Port number
#endif

// SAI Driver
#define _SAI_Driver_(n)  Driver_SAI##n
#define  SAI_Driver_(n) _SAI_Driver_(n)
extern ARM_DRIVER_SAI    SAI_Driver_(WM8904_SAI_PORT_OUT);
#define ptrSAI_OUT      (&SAI_Driver_(WM8904_SAI_PORT_OUT))

#ifdef RTE_BSP_AUDIO_CODEC_IN
extern ARM_DRIVER_SAI    SAI_Driver_(WM8904_SAI_PORT_IN);
#define ptrSAI_IN       (&SAI_Driver_(WM8904_SAI_PORT_IN))
#endif

// Pointer to Stream info
#ifdef RTE_BSP_AUDIO_CODEC_IN
#define STREAM_PTR(stream) ((stream & 0x80U) == 0U ? (&Audio.Out) : (&Audio.In))
#else
#define STREAM_PTR(stream) (&Audio.Out)
#endif

// Audio Stream run-time information
typedef struct _STREAM_INFO {
  void                *pData;
  uint32_t             DataSize;
  uint8_t              Volume_L;
  uint8_t              Volume_R;
  uint8_t              Running;
  uint8_t              Paused;
  uint8_t              UnderflowPending;
  uint8_t              SendCompletePending;
  uint32_t             SFreq;
} STREAM_INFO;

// Audio Stream
typedef struct _STREAM {
  ARM_DRIVER_SAI      *SAI;
  STREAM_INFO         *Info;
} STREAM;

// Audio run-time information
typedef const struct _AUDIO_RESOURCES {
  STREAM               Out;
#ifdef RTE_BSP_AUDIO_CODEC_IN
  STREAM               In;
#endif
} AUDIO_RESOURCES;

static uint8_t     DataBits      = 16U;
static uint32_t    SamplingFreq  = 16000U;
static STREAM_INFO StreamInfoOut = { 0U };

#ifdef RTE_BSP_AUDIO_CODEC_IN
static STREAM_INFO StreamInfoIn  = { 0U };
#endif

static const AUDIO_RESOURCES Audio = {
  ptrSAI_OUT,
  &StreamInfoOut,
#ifdef RTE_BSP_AUDIO_CODEC_IN
  ptrSAI_IN,
  &StreamInfoIn,
#endif
};

static const uint16_t SYSCLK_FS_ratio[] = {64, 128, 192, 256, 384, 512, 768, 1024, 1408, 1536};
static Audio_SignalEvent_t CB_Event;

// Callback
/**
  \fn          void Audio_callback (uint32_t event)
  \brief       SAI callback function
  \param[in]   event
*/
void Audio_callback (uint32_t event) {

  if (event & ARM_SAI_EVENT_FRAME_ERROR) {
    // Audio interface does not support FRAME ERRORS
    event &= ~ARM_SAI_EVENT_FRAME_ERROR;
  }

  if (event & ARM_SAI_EVENT_TX_UNDERFLOW) {
    if ((Audio.Out.Info->Running == 0U)  ||
        (Audio.Out.Info->Paused  == 1U)) {
       Audio.Out.Info->UnderflowPending = 1U;

       // Invalid underflow: Transmitter is running to generate clock for codec
       event &= ~ARM_SAI_EVENT_TX_UNDERFLOW;
     }
  }

  if (event & ARM_SAI_EVENT_SEND_COMPLETE) {
    if (Audio.Out.Info->Paused  == 1U) {
       Audio.Out.Info->SendCompletePending = 1U;

       // Invalid SendComplete: Transmitter is running to generate clock for codec
       event &= ~ARM_SAI_EVENT_SEND_COMPLETE;
     }
  }

  if ((CB_Event != NULL) && (event != 0U)) {
    CB_Event (event);
  }
}

/**
  \fn          int32_t WM8904_RegRead (uint8_t reg, uint16_t * reg_val)
  \brief       Read value from WM8904 register
  \param[in]   reg      WM8904 register address
  \param[Out]  reg_val  Pointer to value read from WM8904 register
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t WM8904_RegRead (uint8_t reg, uint16_t *reg_val) {
  uint8_t  val[2];

  if (ptrI2C->MasterTransmit (DeviceAddr, &reg, 1U, true) != ARM_DRIVER_OK) {
    return -1;
  }
  while (ptrI2C->GetStatus().busy) {
    osDelay(1);
  };

  if (ptrI2C->MasterReceive (DeviceAddr, val, 2U, false) != ARM_DRIVER_OK) {
    return -1;
  }
  while (ptrI2C->GetStatus().busy) {
    osDelay(1);
  };

  *reg_val  = val[0] << 8;
  *reg_val |= val[1];

  return 0;
}

/**
  \fn          int32_t WM8904_RegWrite (uint8_t reg, uint16_t reg_val)
  \brief       Write value to WM8904 register
  \param[in]   reg      WM8904 register address
  \param[in]   reg_val  Value to be written to WM8904 register
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t WM8904_RegWrite (uint8_t reg, uint16_t reg_val) {
  uint8_t val[3];

  val[0] =  reg;
  val[1] = (reg_val >> 8) & 0xFFU;
  val[2] =  reg_val & 0xFFU;

  if (ptrI2C->MasterTransmit (DeviceAddr, val, 3U, false) != ARM_DRIVER_OK) {
    return -1;
  }
  while (ptrI2C->GetStatus().busy) {
    osDelay(1);
  };

  return 0;
}

/**
  \fn          int32_t SAI_Configure (STREAM *ptrStream)
  \brief       Configure SAI transmitter
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t SAI_Configure (const STREAM  *ptrStream) {
  uint32_t cfg, arg1, arg2;

  if (ptrStream == &Audio.Out) {
    // Master transmitter, User protocol
    cfg =  ARM_SAI_CONFIGURE_TX                                   |
           ARM_SAI_MODE_MASTER                                    |
           ARM_SAI_PROTOCOL_USER                                  |
           ARM_SAI_CLOCK_POLARITY_0                               |
           ARM_SAI_MCLK_PIN_OUTPUT                                |
           ARM_SAI_DATA_SIZE(DataBits);
    arg2 = (SamplingFreq & ARM_SAI_AUDIO_FREQ_Msk);
  }

#ifdef RTE_BSP_AUDIO_CODEC_IN
  if (ptrStream == &Audio.In) {
    // Slave receiver, User protocol
    cfg =  ARM_SAI_CONFIGURE_RX                                   |
           ARM_SAI_MODE_SLAVE                                     |
           ARM_SAI_PROTOCOL_USER                                  |
           ARM_SAI_CLOCK_POLARITY_0                               |
           ARM_SAI_DATA_SIZE(DataBits);
  }
#endif

  arg1 = ARM_SAI_FRAME_LENGTH(DataBits * 2U)    |
         ARM_SAI_FRAME_SYNC_WIDTH(DataBits *1U) |
         ARM_SAI_FRAME_SYNC_EARLY               |
         ARM_SAI_FRAME_SYNC_POLARITY_LOW        |
         ARM_SAI_SLOT_COUNT(2);
  if (ptrStream->SAI->Control (cfg, arg1, arg2) != ARM_DRIVER_OK) {
    return -1;
  }

  return 0;
}

/**
  \fn          int32_t WM8904_ConfigureClockRates (uint32_t fs)
  \brief       Congfigure WM8904 sample rate and SYSCLK/fs ratio
  \param[in]   fs  sampling rate
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
static int32_t WM8904_ConfigureClockRates (uint32_t fs) {
  uint32_t mclk, ratio_min, ratio_max, ratio, i;
  uint64_t ratio_exact;
  uint16_t reg_val;

  switch (Chip_Clock_GetMCLKSource ()) {
    case SYSCON_MCLKSRC_FROHF:
      mclk = Chip_Clock_GetFROHFRate () / Chip_Clock_GetMCLKDiv ();
      break;
    case SYSCON_MCLKSRC_PLL:
      mclk = Chip_Clock_GetSystemPLLOutClockRate (true) / Chip_Clock_GetMCLKDiv ();
      break;
    case SYSCON_MCLKSRC_MCLKIN:
    case SYSCON_MCLKSRC_DISABLED:
    default : return -1;
  }

  // Calculate exact ratio multiplied by 1000000
  ratio_exact = ((uint64_t)mclk * 1000000) / fs;
  if (ratio_exact > (0xFFFFFFFF - WM8904_CLK_SYS_RAT_MAX_TOLERANCE)) {
    return -1;
  }

  //  Calculate ratio limit values
  ratio_min = (uint32_t)ratio_exact - WM8904_CLK_SYS_RAT_MAX_TOLERANCE;
  ratio_max = (uint32_t)ratio_exact + WM8904_CLK_SYS_RAT_MAX_TOLERANCE;

  for (i = 0; i < sizeof(SYSCLK_FS_ratio)/ sizeof(SYSCLK_FS_ratio[0]); i++) {
    ratio = SYSCLK_FS_ratio[i] * 1000000;
    if ((ratio > ratio_min) && (ratio < ratio_max)) {
      break;
    }
  }

  if (i == sizeof(SYSCLK_FS_ratio)/ sizeof(SYSCLK_FS_ratio[0])) {
    // No proper ratio available
    return -1;
  }

  switch (fs) {
    case 8000:  reg_val = 0; break;
    case 11025:
    case 12000: reg_val = 1; break;
    case 16000: reg_val = 2; break;
    case 22050:
    case 24000: reg_val = 3; break;
    case 32000: reg_val = 4; break;
    case 44100:
    case 48000: reg_val = 5; break;
    default: return -1;
  }

  WM8904_RegWrite (0x15, reg_val | (i << 10));

  return 0;
}

// Board Audio Interface
/**
  \fn          int32_t Audio_WM8904_Initialize (Audio_SignalEvent_t cb_event)
  \brief       Initialize Audio Interface
  \param[in]   cb_event  pointer to event notification function
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Initialize (Audio_SignalEvent_t cb_event) {
  uint16_t reg_val;

  CB_Event = cb_event;

  // Clear Stream info
  Audio.Out.Info->DataSize     = 0U;
  Audio.Out.Info->pData        = NULL;
  Audio.Out.Info->Running      = 0U;
  Audio.Out.Info->Paused       = 0U;
#ifdef RTE_BSP_AUDIO_CODEC_IN
  Audio.In.Info->DataSize      = 0U;
  Audio.In.Info->pData         = NULL;
  Audio.In.Info->Running       = 0U;
  Audio.In.Info->Paused        = 0U;
#endif

  // Set Default values
  DataBits     = 16U;
  SamplingFreq = 16000;

  // SAI Initialization
  Audio.Out.SAI->Initialize   (Audio_callback);
#ifdef RTE_BSP_AUDIO_CODEC_IN
  Audio.In.SAI->Initialize    (Audio_callback);
#endif
  Audio.Out.SAI->PowerControl (ARM_POWER_FULL);
#ifdef RTE_BSP_AUDIO_CODEC_IN
  Audio.In.SAI->PowerControl  (ARM_POWER_FULL);
#endif

  if (SAI_Configure (&Audio.Out) != 0U) {
    return -1;
  }

#ifdef RTE_BSP_AUDIO_CODEC_IN
  if (SAI_Configure (&Audio.In) != 0U) {
    return -1;
  }
#endif

  // Start transmitter to Generate BCLK and WSCLK
  Audio.Out.SAI->Control (ARM_SAI_CONTROL_TX, 1U, 0U);

  // I2C Initialization and configuration
  ptrI2C->Initialize   (NULL);
  ptrI2C->PowerControl (ARM_POWER_FULL);
  ptrI2C->Control      (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
  ptrI2C->Control      (ARM_I2C_BUS_CLEAR, 0);

  // Configure MCLK pin
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_INPUTMUX);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_GPIO1);  
  Chip_IOCON_PinMuxSet (LPC_IOCON, 1, 17, IOCON_FUNC4 | IOCON_DIGITAL_EN);
  Chip_Clock_SetMCLKDirOutput ();

  // WM 8904 Configuration
  // Reset
  WM8904_RegWrite (0x00, 0x0000);

  // SYSCLK source = MCLK;
  // Enable: DSP clock, System clock
  WM8904_RegWrite (0x16, 0x000F);

  // Disable and reset FLL
  WM8904_RegWrite (0x74, 0x0000);
  WM8904_RegWrite (0x75, 0x0000);

  // Check if Codec ID is valid
  WM8904_RegRead (0x00, &reg_val);
  if (reg_val != 0x8904) {
    return -1;
  }

  // Write sequencer enable
  WM8904_RegWrite (0x6C, 0x0100);
  WM8904_RegWrite (0x6F, 0x0100);
  osDelay (100);
  WM8904_RegWrite (0xFF, 500);

  WM8904_RegWrite ( 0x14, 0xA45F );

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // Enable LINEOUTL input stage
  WM8904_RegWrite ( 0x5E, 0x0010 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // Enable HP input stage
  WM8904_RegWrite ( 0x5A, 0x0011 );
#endif

  // Enable left and right PGA input
  WM8904_RegWrite ( 0x0C, 0x0003 );

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // Enable left and right Headphone output
  WM8904_RegWrite ( 0x0E, 0x0003 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // Enable left line output (Speaker)
  WM8904_RegWrite ( 0x0F, 0x0002 );
#endif

  // Enable DAC and ADC
  WM8904_RegWrite ( 0x12, 0x000F );

  // ADC oversampling = 128 x fs
  WM8904_RegWrite ( 0x0a, 0x0001 );
  osDelay(1);

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // Enable LINEOUTL intermediate stage
  WM8904_RegWrite ( 0x5E, 0x0030 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // Enable HP intermediate stage
  WM8904_RegWrite ( 0x5A, 0x0033 );
#endif

  // Set Sample rate to 48 kHz
  if (WM8904_ConfigureClockRates (48000) == -1) {
    return -1;
  } 

  // Right ADC -> output on right channel; Right DAC -> right data channel
  WM8904_RegWrite ( 0x18, 0x0050 );

  // ADC slot0 is used; BCLK is input; 16bit data; I2S protocol
  WM8904_RegWrite ( 0x19, 0x0002 );

  // BCLK divider = 16
  WM8904_RegWrite ( 0x1a, 0x000c );

  // DAC un-mute
  WM8904_RegWrite ( 0x21, 0x0040 );

  // Single-ended mode on IN1L and IN1R
  WM8904_RegWrite ( 0x2E, 0x0000 );
  WM8904_RegWrite ( 0x2F, 0x0000 );

  // Un-mute PGA; Default gain is 0dB
  WM8904_RegWrite ( 0x2C, 0x0005 );
  WM8904_RegWrite ( 0x2D, 0x0005 );

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // Un-mute Headphone; Gain = 0dB
  WM8904_RegWrite ( 0x39, 0x0039 );
  WM8904_RegWrite ( 0x3A, 0x0039 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // Un-mute Left line out; Gain = 0dB
  WM8904_RegWrite ( 0x3b, 0x0039 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // Enable LINEOUTL output stage
  WM8904_RegWrite ( 0x5E, 0x0070 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // Enable HP output stage
  WM8904_RegWrite ( 0x5A, 0x0077 );
#endif

  // DC servo enable (HP and LINEOUT)
  WM8904_RegWrite ( 0x43, 0x000F );

#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
  // LINEOUTL remove short
  WM8904_RegWrite ( 0x5E, 0x00F0 );
#endif

#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
  // HP remove short
  WM8904_RegWrite( 0x5A, 0x00FF );
#endif

  // Enable charge pump
  WM8904_RegWrite ( 0x68, 0x0001 );
  WM8904_RegWrite ( 0x62, 0x0001 );

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_Uninitialize (void)
  \brief       De-initialize Audio Interface
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Uninitialize (void) {

  // Abort SAI Send and Receive
#ifdef RTE_BSP_AUDIO_CODEC_IN
  Audio.In.SAI->Control  (ARM_SAI_ABORT_RECEIVE, 0U, 0U);
#endif
  Audio.Out.SAI->Control (ARM_SAI_ABORT_SEND,    0U, 0U);

  // WM8994 Reset
  WM8904_RegWrite(0x0000, 0x0000);

  // Disable SAI transmitter and receiver
#ifdef RTE_BSP_AUDIO_CODEC_IN
  Audio.In.SAI->Control  (ARM_SAI_CONTROL_RX, 0U, 0U);
#endif
  Audio.Out.SAI->Control (ARM_SAI_CONTROL_TX, 0U, 0U);

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_SendData (const void *data, uint32_t num)
  \brief       Start sending data to Audio output stream
  \param[in]   data  pointer to buffer with data to send
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_SendData (const void *data, uint32_t num) {

  if ((Audio.Out.Info->Running == 0U)  ||
      (Audio.Out.Info->Paused  == 1U)) {
    // Save data info
    Audio.Out.Info->pData    = (void *)data;
    Audio.Out.Info->DataSize = num;
  } else {
    Audio.Out.Info->pData    = (void *)data;
    Audio.Out.Info->DataSize = num;
    if (Audio.Out.SAI->Send (data, num) != ARM_DRIVER_OK) {
      return -1;
    }
  }

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_ReceiveData (void *data, uint32_t num)
  \brief       Start receiving data from Audio input stream
  \param[out]  data  pointer to buffer for data to receive
  \param[in]   num   number of data items to send
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_ReceiveData (void *data, uint32_t num) {  
#ifdef RTE_BSP_AUDIO_CODEC_IN
  if (Audio.In.SAI->Receive (data, num) != ARM_DRIVER_OK) {
    return -1;
  }
#endif

  return 0;
}

/**
  \fn          uint32_t Audio_WM8904_GetDataTxCount (void)
  \brief       Get transmitted data count
  \returns     number of data items transmitted
*/
uint32_t Audio_WM8904_GetDataTxCount (void) {
  return (Audio.Out.SAI->GetTxCount());
}

/**
  \fn          uint32_t Audio_GetDataRxCount (void)
  \brief       Get received data count
  \returns     number of data items received
*/
uint32_t Audio_WM8904_GetDataRxCount (void) {
#ifdef RTE_BSP_AUDIO_CODEC_IN
  return (Audio.In.SAI->GetRxCount());
#else
  return 0;
#endif

}

/**
  \fn          int32_t Audio_WM8904_Start (uint8_t stream)
  \brief       Start Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Start (uint8_t stream) {
  void * data;
  uint32_t num, evt = 0U;

  switch (stream) {
    case AUDIO_STREAM_OUT:
      // Set Running flag
      Audio.Out.Info->Running = 1U;
      if (Audio.Out.Info->Paused == 0U) {

        // Check for valid data to be send
        if (Audio.Out.Info->DataSize != 0U) {
          // Clear pending event flags
          Audio.Out.Info->UnderflowPending    = 0U;
          Audio.Out.Info->SendCompletePending = 0U;

          // Save data information, before clearing it
          num  = Audio.Out.Info->DataSize;
          data = Audio.Out.Info->pData;
          Audio.Out.Info->DataSize = 0U;
          Audio.Out.Info->pData    = NULL;

          // Send data
          if (Audio.Out.SAI->Send (data, num) != ARM_DRIVER_OK) {
            return -1;
          }
        } else {
          // Check for pending events
          if (Audio.Out.Info->UnderflowPending    == 1U) { evt |= AUDIO_EVENT_TX_UNDERFLOW;  }
          if (Audio.Out.Info->SendCompletePending == 1U) { evt |= AUDIO_EVENT_SEND_COMPLETE; }

          // Clear pending event flags
          Audio.Out.Info->UnderflowPending    = 0U;
          Audio.Out.Info->SendCompletePending = 0U;

          if ((CB_Event != NULL) && (evt != 0U)) {
            CB_Event (evt);
          }
        }
      }
      break;
    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      Audio.In.SAI->Control (ARM_SAI_CONTROL_RX, 1U, 0U);
#endif
      break;
    default: return -1;
  }

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_Stop (uint8_t stream)
  \brief       Stop Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Stop (uint8_t stream) {

  switch (stream) {
    case AUDIO_STREAM_OUT:
      // Clear running flag
      Audio.Out.Info->Running = 0U;
      // Abort transfer
      Audio.Out.SAI->Control (ARM_SAI_ABORT_SEND, 1U, 0U);
      break;
    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      // Clear running flag
      Audio.In.Info->Running = 0U;
      Audio.In.SAI->Control (ARM_SAI_CONTROL_RX, 0U, 0U);
      // Abort transfer
      Audio.In.SAI->Control (ARM_SAI_ABORT_RECEIVE, 1U, 0U);
#endif
      break;
    default: return (-1);
  }

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_Pause (uint8_t stream)
  \brief       Pause Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Pause (uint8_t stream) {

  switch (stream) {
    case AUDIO_STREAM_OUT:
      if (Audio.Out.Info->Running == 0U) { return 0U; }

      // Clear pending event flags
      Audio.Out.Info->UnderflowPending    = 0U;
      Audio.Out.Info->SendCompletePending = 0U;

      // Set paused flag
      Audio.Out.Info->Paused = 1U;
      break;
    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      if (Audio.In.Info->Running == 0U) { return 0U; }

      // Disable SAI receiver
      Audio.In.SAI->Control (ARM_SAI_CONTROL_RX, 0U, 0U);
#endif
      break;
    default: return (-1);
  }
  return 0;
}

/**
  \fn          int32_t Audio_WM8904_Resume (uint8_t stream)
  \brief       Resume Audio stream
  \param[in]   stream  stream identifier
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_Resume (uint8_t stream) {
  void * data;
  uint32_t num, evt = 0U;

  switch (stream) {
    case AUDIO_STREAM_OUT:
      if ((Audio.Out.Info->Paused  == 1U) &&
          (Audio.Out.Info->Running == 1U)) {

        // Clear Paused flag
        Audio.Out.Info->Paused = 0U;

        // Check for valid data to be send
        if (Audio.Out.Info->DataSize != 0U) {
          // Clear pending event flags
          Audio.Out.Info->UnderflowPending    = 0U;
          Audio.Out.Info->SendCompletePending = 0U;

          // Save data information, before clearing it
          num  = Audio.Out.Info->DataSize;
          data = Audio.Out.Info->pData;
          Audio.Out.Info->DataSize = 0U;
          Audio.Out.Info->pData    = NULL;

          // Send data
          if (Audio.Out.SAI->Send (data, num) != ARM_DRIVER_OK) {
            return -1;
          }
        } else {
          // Check for pending events
          if (Audio.Out.Info->UnderflowPending    == 1U) { evt |= AUDIO_EVENT_TX_UNDERFLOW;  }
          if (Audio.Out.Info->SendCompletePending == 1U) { evt |= AUDIO_EVENT_SEND_COMPLETE; }

          // Clear pending event flags
          Audio.Out.Info->UnderflowPending    = 0U;
          Audio.Out.Info->SendCompletePending = 0U;

          if ((CB_Event != NULL) && (evt != 0U)) {
            CB_Event (evt);
          }
        }
      }
      break;
    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      if (Audio.In.Info->Paused == 1U) {
        // Clear paused flag
        Audio.In.Info->Paused = 0U;

        // Enable Receiver
        Audio.In.SAI->Control (ARM_SAI_CONTROL_RX, 1U, 0U);
      }
#endif
      break;
    default: return -1;
  }

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume)
  \brief       Set volume level for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   volume   volume level (0..100)
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_SetVolume (uint8_t stream, uint8_t channel, uint8_t volume) {

  if (volume > 100) { return -1; }

  switch (stream) {
    case AUDIO_STREAM_OUT:
      // Scale volume
      volume  = (volume * 63) / 100;
      volume |= 0xC0;
      switch (channel) {
        case AUDIO_CHANNEL_MASTER:
          Audio.Out.Info->Volume_L = volume;
          Audio.Out.Info->Volume_R = volume;
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          // Update Left and Right channel volume
          WM8904_RegWrite(0x39, volume);
          WM8904_RegWrite(0x3A, volume);
#endif
#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
          // Update volume
          WM8904_RegWrite(0x3B, volume);
#endif
          break;

        case AUDIO_CHANNEL_LEFT:
          Audio.Out.Info->Volume_L = volume;
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          // Update Left channel volume
          WM8904_RegWrite(0x39, volume);
#endif
#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
          // Update volume
          WM8904_RegWrite(0x3B, volume);
#endif
          break;
        case AUDIO_CHANNEL_RIGHT:
          Audio.Out.Info->Volume_R = volume;
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          // Update Right channel volume
          WM8904_RegWrite(0x3A, volume);
#endif
          break;
      }
    break;
    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      // Scale Volume
      volume = (volume * 31) / 100;
      switch (channel) {
        case AUDIO_CHANNEL_MASTER:
          // Save Audio IN volume settings
          Audio.In.Info->Volume_L = volume;
          Audio.In.Info->Volume_R = volume;

          // Update Left and Right channel volume
          WM8904_RegWrite(0x2C, volume);
          WM8904_RegWrite(0x2D, volume);
          break;
        case AUDIO_CHANNEL_LEFT:
          // Save Audio IN volume settings
          Audio.In.Info->Volume_L = volume;

          // Update Left channel volume
          WM8904_RegWrite(0x2C, volume);
          break;
        case AUDIO_CHANNEL_RIGHT:
          // Save Audio IN volume settings
          Audio.In.Info->Volume_R = volume;

          // Update Right channel volume
          WM8904_RegWrite(0x2D, volume);
          break;
      }
#endif

    break;

    default: return -1;
  }
  return 0;
}

/**
  \fn          int32_t Audio_WM8904_SetMute (uint8_t stream, uint8_t channel, bool mute)
  \brief       Set mute state for Audio stream
  \param[in]   stream   stream identifier
  \param[in]   channel  channel identifier
  \param[in]   mute     mute state
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_SetMute (uint8_t stream, uint8_t channel, bool mute) {
  uint16_t val;

  switch (stream) {
    case AUDIO_STREAM_OUT:
      if (mute == true) { val = 0x0100; }
      else              { val = 0x00C0; }

      switch (channel) {
        case AUDIO_CHANNEL_MASTER:
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          WM8904_RegWrite(0x39, Audio.Out.Info->Volume_L | val);
          WM8904_RegWrite(0x3A, Audio.Out.Info->Volume_R | val);
#endif
#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
          WM8904_RegWrite(0x3B, Audio.Out.Info->Volume_L | val);
#endif
          break;

        case AUDIO_CHANNEL_LEFT:
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          WM8904_RegWrite(0x39, Audio.Out.Info->Volume_L | val);
#endif
#if (WM8904_OUTPUT & WM8904_OUTPUT_SPEAKER)
          WM8904_RegWrite(0x3B, Audio.Out.Info->Volume_L | val);
#endif
          break;
        case AUDIO_CHANNEL_RIGHT:
#if (WM8904_OUTPUT & WM8904_OUTPUT_HPOUT)
          WM8904_RegWrite(0x3A, Audio.Out.Info->Volume_R | val);
#endif
          break;
      }
    break;

    case AUDIO_STREAM_IN:
#ifdef RTE_BSP_AUDIO_CODEC_IN
      if (mute == true) { val = 0x0080; }
      else              { val = 0x0000; }

      switch (channel) {
        case AUDIO_CHANNEL_MASTER:
          WM8904_RegWrite(0x2C, Audio.In.Info->Volume_L | val);
          WM8904_RegWrite(0x2D, Audio.In.Info->Volume_R | val);
          break;

        case AUDIO_CHANNEL_LEFT:
          WM8904_RegWrite(0x2C, Audio.In.Info->Volume_L | val);
          break;
        case AUDIO_CHANNEL_RIGHT:
          WM8904_RegWrite(0x2D, Audio.In.Info->Volume_R | val);
          break;
      }
#endif
    break;

    default: return -1;
  }
  return 0;
}

/**
  \fn          int32_t Audio_WM8904_SetDataFormat (uint8_t stream, uint8_t format)
  \brief       Set Audio data format
  \param[in]   stream  stream identifier
  \param[in]   format  data format
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_SetDataFormat (uint8_t stream, uint8_t format) {
  const STREAM *ptrStream;

  ptrStream = STREAM_PTR(stream);

  switch (format) {

    case AUDIO_DATA_16_STEREO:
      DataBits = 16U;
      // Set codec word legth
      WM8904_RegWrite ( 0x19, 0x0002 );
      break;
      case AUDIO_DATA_32_STEREO:
      DataBits = 32U;
      // Set codec word legth
      WM8904_RegWrite ( 0x19, 0x000E );
      break;
    default: return -1;
  }

  // Update SAI Data format configuration
  if (SAI_Configure (ptrStream) == -1) { return -1; }

  return 0;
}

/**
  \fn          int32_t Audio_WM8904_SetFrequency (uint8_t stream, uint32_t frequency)
  \brief       Set Audio stream frequency
  \param[in]   stream     stream identifier
  \param[in]   frequency  Audio frequency in Hz
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Audio_WM8904_SetFrequency (uint8_t stream, uint32_t frequency) {

  // Save Audio frequency value
  SamplingFreq = frequency;

  // Update WM8904 clock rates
  if (WM8904_ConfigureClockRates (frequency) == -1) {
    return -1;
  }

  // Update SAI Audio frequency configuration
  if (SAI_Configure (&Audio.Out) == -1) {
    return -1;
  }

  return 0;
}

