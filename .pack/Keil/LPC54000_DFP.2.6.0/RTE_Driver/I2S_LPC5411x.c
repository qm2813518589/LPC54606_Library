/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2016 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        11. August 2016
 * $Revision:    V1.1
 *
 * Driver:       Driver_SAI, Driver_SAI1
 *
 * Configured:   via RTE_Device.h configuration file
 * Project:      SAI Driver for NXP LPC5411x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   SAI Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_SAI# = 0       use SAI0 (I2S6)
 *   Connect to hardware via Driver_SAI# = 1       use SAI1 (I2S7)
 *
 * -------------------------------------------------------------------------- */

/* Note:
    When using DMA data transfer mode, make sure that application calls function
    Chip_DMASERVICE_Isr (part of LPCOpen firmware package, see dma_service_5411x.h
    for details) to handle DMA IRQ requests. Two possible ways are as follows:

    a) Replace all occurences of DMA_IRQHandler in device startup code
       with Chip_DMASERVICE_Isr
    b) Implement DMA_IRQHandler function which calls Chip_DMASERVICE_Isr:
       void DMA_IRQHandler (void) {
         Chip_DMASERVICE_Isr();
       }
 */

/* History:
 *  Version 1.1
 *    Merged TX and RX statuses
 *    I2S Flexcom clcok source can be defined by the user
 *  Version 1.0
 *    Initial release
 */

#include "I2S_LPC5411x.h"

#define ARM_SAI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,1)   // driver version
// Driver Version
static const ARM_DRIVER_VERSION DriverVersion = { ARM_SAI_API_VERSION, ARM_SAI_DRV_VERSION };

// User Definitions
// Can be user defined by C preprocessor

// SAI0 STREAM DIRECTION: SAI_STREAM_IN or SAI_STREAM_OUT
#ifndef SAI0_STREAM
#define SAI0_STREAM  SAI_STREAM_OUT
#endif

// SAI1 STREAM: SAI_STREAM_IN or SAI_STREAM_OUT
#ifndef SAI1_STREAM
#define SAI1_STREAM  SAI_STREAM_OUT
#endif

// Trigger level definitions (trig_level: 0 .. 7)
#ifndef SAI0_TRIG_LEVEL
#define SAI0_TRIG_LEVEL      4U
#endif
#ifndef SAI1_TRIG_LEVEL
#define SAI1_TRIG_LEVEL      4U
#endif

// Clock Source used for I2S FLEXCOMM (CHIP_SYSCON_FLEXCOMMCLKSELSRC_T)
// SAI0 -> I2S FLEXCOMM6
#ifndef SAI0_CLK_SRC
#define SAI0_CLK_SRC         (SYSCON_FLEXCOMMCLKSELSRC_MCLK)
#endif
// SAI1 -> I2S FLEXCOMM1
#ifndef SAI1_CLK_SRC
#define SAI1_CLK_SRC         (SYSCON_FLEXCOMMCLKSELSRC_MCLK)
#endif


// DMA
#define I2S_TX_DMA_USED         (((RTE_SAI0 != 0) && (I2S6_TX_DMA_EN != 0) && (SAI0_STREAM == SAI_STREAM_OUT)) || \
                                 ((RTE_SAI1 != 0) && (I2S7_TX_DMA_EN != 0) && (SAI1_STREAM == SAI_STREAM_OUT)))

// DMA
#define I2S_RX_DMA_USED         (((RTE_SAI0 != 0) && (I2S6_RX_DMA_EN != 0) && (SAI0_STREAM == SAI_STREAM_IN)) || \
                                 ((RTE_SAI1 != 0) && (I2S7_RX_DMA_EN != 0) && (SAI1_STREAM == SAI_STREAM_IN)))

#define I2S_DMA_USED            (I2S_TX_DMA_USED | I2S_RX_DMA_USED)

// End of User Definitions

// SAI0 (I2S6)
#if (RTE_SAI0)
static SAI_INFO SAI0_Info  = { 0 };
#if (RTE_I2S6_WS_PIN_EN == 1)
  static PINMUX_GRP_T SAI0_pin_ws   = { RTE_I2S6_WS_PORT,  RTE_I2S6_WS_BIT,  RTE_I2S6_WS_FUNC   | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif
#if (RTE_I2S6_SDA_PIN_EN == 1)
  static PINMUX_GRP_T SAI0_pin_data = { RTE_I2S6_SDA_PORT, RTE_I2S6_SDA_BIT, RTE_I2S6_SDA_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif
#if (RTE_I2S6_SCK_PIN_EN == 1)
  static PINMUX_GRP_T SAI0_pin_sck  = { RTE_I2S6_SCK_PORT, RTE_I2S6_SCK_BIT,  RTE_I2S6_SCK_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif

// DMA
#if ((SAI0_STREAM == SAI_STREAM_OUT) && (I2S6_TX_DMA_EN != 0))
static       void    SAI0_TX_DMA_Callback (int32_t event);
static const I2S_DMA SAI0_TX_DMA = { I2S6_TX_DMAREQ, &SAI0_TX_DMA_Callback, };
#endif
#if ((SAI0_STREAM == SAI_STREAM_IN) && (I2S6_RX_DMA_EN != 0))
static       void    SAI0_RX_DMA_Callback (int32_t event);
static const I2S_DMA SAI0_RX_DMA = { I2S6_RX_DMAREQ, &SAI0_RX_DMA_Callback, };
#endif

static const SAI_RESOURCES SAI0_Resources = {
  {  // Capabilities
    1,   ///< supports asynchronous Transmit/Receive
    0,   ///< supports synchronous Transmit/Receive
    1,   ///< supports user defined Protocol
    1,   ///< supports I2S Protocol
    1,   ///< supports MSB/LSB justified Protocol
    1,   ///< supports PCM short/long frame Protocol
    0,   ///< supports AC'97 Protocol
    0,   ///< supports Mono mode
    0,   ///< supports Companding
    0,   ///< supports MCLK (Master Clock) pin
    0,   ///< supports Frame error event: \ref ARM_SAI_EVENT_FRAME_ERROR
  },
  LPC_I2S6,

  {  // PINS
#if (RTE_I2S6_WS_PIN_EN == 1)
    &SAI0_pin_ws,
#else
    NULL,
#endif
#if (RTE_I2S6_SDA_PIN_EN == 1)
    &SAI0_pin_data,
#else
    NULL,
#endif
#if (RTE_I2S6_SCK_PIN_EN == 1)
    &SAI0_pin_sck,
#else
    NULL,
#endif
  },

  // DMA
#if   ((SAI0_STREAM == SAI_STREAM_IN)  && (I2S6_RX_DMA_EN != 0))
  &SAI0_RX_DMA,
#elif ((SAI0_STREAM == SAI_STREAM_OUT) && (I2S6_TX_DMA_EN != 0))
  &SAI0_TX_DMA,
#else
  NULL,
#endif
  I2S6_IRQn,
  SAI0_TRIG_LEVEL,
  (I2S_DIR_T)SAI0_STREAM,
  &SAI0_Info,
};
#endif

// SAI1 (I2S7)
#if (RTE_SAI1)
static SAI_INFO SAI1_Info  = { 0 };
#if (RTE_I2S7_WS_PIN_EN == 1)
  static PINMUX_GRP_T SAI1_pin_ws   = { RTE_I2S7_WS_PORT,  RTE_I2S7_WS_BIT,  RTE_I2S7_WS_FUNC   | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif
#if (RTE_I2S7_SDA_PIN_EN == 1)
  static PINMUX_GRP_T SAI1_pin_data = { RTE_I2S7_SDA_PORT, RTE_I2S7_SDA_BIT, RTE_I2S7_SDA_FUNC | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif
#if (RTE_I2S7_SCK_PIN_EN == 1)
  static PINMUX_GRP_T SAI1_pin_sck  = { RTE_I2S7_SCK_PORT, RTE_I2S7_SCK_BIT,  RTE_I2S7_SCK_FUNC  | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF | IOCON_MODE_PULLDOWN};
#endif

// DMA
#if ((SAI1_STREAM == SAI_STREAM_OUT) && (I2S7_TX_DMA_EN != 0))
static       void    SAI1_TX_DMA_Callback (int32_t event);
static const I2S_DMA SAI1_TX_DMA = { I2S7_TX_DMAREQ, &SAI1_TX_DMA_Callback, };
#endif
#if ((SAI1_STREAM == SAI_STREAM_IN) && (I2S7_RX_DMA_EN != 0))
static       void    SAI1_RX_DMA_Callback (int32_t event);
static const I2S_DMA SAI1_RX_DMA = { I2S7_RX_DMAREQ, &SAI1_RX_DMA_Callback, };
#endif

static const SAI_RESOURCES SAI1_Resources = {
  {  // Capabilities
    1,   ///< supports asynchronous Transmit/Receive
    0,   ///< supports synchronous Transmit/Receive
    1,   ///< supports user defined Protocol
    1,   ///< supports I2S Protocol
    1,   ///< supports MSB/LSB justified Protocol
    1,   ///< supports PCM short/long frame Protocol
    0,   ///< supports AC'97 Protocol
    0,   ///< supports Mono mode
    0,   ///< supports Companding
    0,   ///< supports MCLK (Master Clock) pin
    0,   ///< supports Frame error event: \ref ARM_SAI_EVENT_FRAME_ERROR
  },
  LPC_I2S7,

  {  // PINS
#if (RTE_I2S7_WS_PIN_EN == 1)
    &SAI1_pin_ws,
#else
    NULL,
#endif
#if (RTE_I2S7_SDA_PIN_EN == 1)
    &SAI1_pin_data,
#else
    NULL,
#endif
#if (RTE_I2S7_SCK_PIN_EN == 1)
    &SAI1_pin_sck,
#else
    NULL,
#endif
  },

  // DMA
#if   ((SAI1_STREAM == SAI_STREAM_IN)  && (I2S7_RX_DMA_EN != 0))
  &SAI1_RX_DMA,
#elif ((SAI1_STREAM == SAI_STREAM_OUT) && (I2S7_TX_DMA_EN != 0))
  &SAI1_TX_DMA,
#else
  NULL,
#endif
  I2S7_IRQn,
  SAI1_TRIG_LEVEL,
  (I2S_DIR_T)SAI1_STREAM,
  &SAI1_Info,
};
#endif

// Function prototypes
#if I2S_TX_DMA_USED
static void SAI_TX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai);
#endif
#if I2S_RX_DMA_USED
static void SAI_RX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai);
#endif

/**
  \fn          ARM_DRIVER_VERSION SAI_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION SAI_GetVersion (void) {
  return (DriverVersion);
}

/**
  \fn          ARM_SAI_CAPABILITIES SAI_GetCapabilities (SAI_RESOURCES *sai)
  \brief       Get driver capabilities.
  \param[in]   sai  Pointer to sai resources
  \return      \ref ARM_SAI_CAPABILITIES
*/
static ARM_SAI_CAPABILITIES SAI_GetCapabilities (SAI_RESOURCES *sai) {
  return (sai->capabilities);
}

/**
  \fn          int32_t SAI_Initialize (ARM_SAI_SignalEvent_t cb_event, SAI_RESOURCES *sai)
  \brief       Initialize SAI Interface.
  \param[in]   cb_event  Pointer to \ref ARM_SAI_SignalEvent
  \param[in]   sai       Pointer to sai resources
  \return      \ref execution_status
*/
static int32_t SAI_Initialize (ARM_SAI_SignalEvent_t cb_event, SAI_RESOURCES *sai) {

  if (sai->info->flags & SAI_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // Initialize I2S Run-Time resources
  sai->info->cb_event               = cb_event;
  sai->info->status.frame_error     = 0U;
  sai->info->status.busy            = 0U;
  sai->info->status.under_over_flow = 0U;

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

  // Configure WS pin
  if (sai->pins.ws != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.ws->port, sai->pins.ws->pin, sai->pins.ws->modefunc);
  }

  // Configure SDA pin
  if (sai->pins.sda != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.sda->port, sai->pins.sda->pin, sai->pins.sda->modefunc);
  }

  // Configure SCK pin
  if (sai->pins.sck != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.sck->port, sai->pins.sck->pin, sai->pins.sck->modefunc);
  }

  sai->info->flags = SAI_FLAG_INITIALIZED;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SAI_Uninitialize (SAI_RESOURCES *sai)
  \brief       De-initialize SAI Interface.
  \param[in]   sai  Pointer to sai resources
  \return      \ref execution_status
*/
static int32_t SAI_Uninitialize (SAI_RESOURCES *sai) {

  Chip_Clock_EnablePeriphClock(SYSCON_CLOCK_IOCON);

  // Reset WS pin Configuration
  if (sai->pins.ws != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.ws->port, sai->pins.ws->pin, 0U);
  }

  // Reset SDA pin Configuration
  if (sai->pins.sda != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.sda->port, sai->pins.sda->pin, 0U);
  }

  // Reset SCK pin Configuration
  if (sai->pins.sck != NULL) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, sai->pins.sck->port, sai->pins.sck->pin, 0U);
  }

  // Reset SAI status flags
  sai->info->flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SAI_PowerControl (ARM_POWER_STATE state, SAI_RESOURCES *sai)
  \brief       Control SAI Interface Power.
  \param[in]   state  Power state
  \param[in]   sai    Pointer to sai resources
  \return      \ref execution_status
*/
static int32_t SAI_PowerControl (ARM_POWER_STATE state, SAI_RESOURCES *sai) {

  I2S_AUDIO_FORMAT_T       audio;
#if I2S_DMA_USED
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  switch (state) {
    case ARM_POWER_OFF:

      // Disable I2S IRQ
      NVIC_DisableIRQ(sai->irq_num);

      // Deinitialize I2S
      Chip_I2S_DeInit (sai->reg);

      // Clear pending I2S interrupts in NVIC
      NVIC_ClearPendingIRQ(sai->irq_num);
      
      sai->info->status.frame_error     = 0U;
      sai->info->status.busy            = 0U;
      sai->info->status.under_over_flow = 0U;

      sai->info->flags = SAI_FLAG_INITIALIZED;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((sai->info->flags & SAI_FLAG_INITIALIZED) == 0U) { return ARM_DRIVER_ERROR; }
      if ((sai->info->flags & SAI_FLAG_POWERED)     != 0U) { return ARM_DRIVER_OK; }

      // I2S peripheral initialize
      audio.Direction = sai->stream_dir;
      Chip_I2S_Init (sai->reg, &audio);

      // Set I2S FLEXCOMM clock source
      if (sai->reg == LPC_I2S6) {
        Chip_Clock_SetFLEXCOMMClockSource(6, SAI0_CLK_SRC);
      } else if (sai->reg == LPC_I2S7) {
        Chip_Clock_SetFLEXCOMMClockSource(7, SAI1_CLK_SRC);
      }
      

      // Disable FIFO
      audio.Direction = I2S_TX;
      Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_DISABLE);
      audio.Direction = I2S_RX;
      Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_DISABLE);

      audio.Direction = sai->stream_dir;

      // Disable I2S FIFO Interupts
      Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_BITMASK);

      // Empty the FIFO
      Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_CLEAR);

      // Set USART FIFO trigger level
      Chip_I2S_SetFIFOTrigLevel (sai->reg, sai->fifo_lvl, sai->fifo_lvl);

      // Clear driver variables
      sai->info->status.frame_error     = 0U;
      sai->info->status.busy            = 0U;
      sai->info->status.under_over_flow = 0U;

#if I2S_DMA_USED
      if (sai->dma != NULL) {
        // DMA Initialization
        Chip_DMA_Init(LPC_DMA);
        Chip_DMA_Enable(LPC_DMA);
        Chip_DMA_SetSRAMBase(LPC_DMA, DMA_ADDR(Chip_DMA_Table));

        // Register SAI DMA Callback
        dma_context.channel = sai->dma->chid;
        Chip_DMASERVICE_RegisterCb (&dma_context, sai->dma->callback);

        // Enable DMA IRQ
        NVIC_EnableIRQ(DMA_IRQn);

        // Enable DMA requests
        Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_DMA_ENABLE);
      }
#endif

      sai->info->flags = SAI_FLAG_POWERED | SAI_FLAG_INITIALIZED;

      // Clear and Enable SAO IRQ
      NVIC_ClearPendingIRQ(sai->irq_num);
      NVIC_EnableIRQ(sai->irq_num);

      break;

    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SAI_Send (const void *data, uint32_t num, SAI_RESOURCES *sai)
  \brief       Start sending data to SAI transmitter.
  \param[in]   data  Pointer to buffer with data to send to SAI transmitter
  \param[in]   num   Number of data items to send
  \param[in]   sai   Pointer to sai resources
  \return      \ref execution_status
*/
static int32_t SAI_Send (const void *data, uint32_t num, SAI_RESOURCES *sai) {
#if I2S_TX_DMA_USED
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((sai->info->flags & SAI_FLAG_CONFIGURED) == 0U) {
    // SAI is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (sai->stream_dir != SAI_STREAM_OUT) {
    // Stream is not defined as OUT stream
    return ARM_DRIVER_ERROR;
  }

  if (sai->info->status.busy) {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Send active flag
  sai->info->status.busy = 1U;

  // Clear TX underflow flag
  sai->info->status.under_over_flow = 0U;

  // Save transmit buffer info
  sai->info->xfer.buf = (uint8_t *)data;
  sai->info->xfer.num = num;
  sai->info->xfer.cnt = 0U;

#if I2S_TX_DMA_USED
  // DMA mode
  if ((sai->info->xfer.data_bits > 8U) && (sai->dma != NULL)) {
    if (num > 1024U) { num = 1024U; }

    sai->info->xfer.num_pending = num;

    dma_context.width = DMA_XFERCFG_WIDTH_32;
    if (sai->info->xfer.data_bits <= 16) {
      num *= 2U;
    } else {
      num *= 4U;
    }

    sai->info->xfer.buf += num;

    dma_context.channel           = sai->dma->chid;
    dma_context.register_location = &(sai->reg->FIFOWR);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_1;
    dma_context.write             = 1U;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)data, num);
  } else
#endif
  {
    // Enable I2S FIFO level Interupts
    Chip_I2S_FIFO_SetInterrupt (sai->reg, I2S_FIFO_INT_TXLVL);
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SAI_Receive (void *data, uint32_t num, SAI_RESOURCES *sai)
  \brief       Start receiving data from SAI receiver.
  \param[out]  data  Pointer to buffer for data to receive from SAI receiver
  \param[in]   num   Number of data items to receive
  \param[in]   sai   Pointer to sai resources
  \return      \ref execution_status
*/
static int32_t SAI_Receive (void *data, uint32_t num, SAI_RESOURCES *sai) {
#if I2S_RX_DMA_USED
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((sai->info->flags & SAI_FLAG_CONFIGURED) == 0U) {
    // SAI is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if (sai->stream_dir != SAI_STREAM_IN) {
    // Stream is not defined as IN stream
    return ARM_DRIVER_ERROR;
  }

  if (sai->info->status.busy) {
    // Receive is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set Receive active flag
  sai->info->status.busy = 1U;

  // Clear RX overflow flag
  sai->info->status.under_over_flow = 0U;

  // Save receive buffer info
  sai->info->xfer.buf = (uint8_t *)data;
  sai->info->xfer.num = num;
  sai->info->xfer.cnt = 0U;

#if I2S_RX_DMA_USED
  // DMA mode
  if ((sai->info->xfer.data_bits > 8U) && (sai->dma != NULL)) {
    if (num > 1024U) { num = 1024U; }

    sai->info->xfer.num_pending = num;

    dma_context.width = DMA_XFERCFG_WIDTH_32;
    if (sai->info->xfer.data_bits <= 16U) {
      num *= 2U;
    } else {
      num *= 4U;
    }

    sai->info->xfer.buf += num;

    dma_context.channel           = sai->dma->chid;
    dma_context.register_location = (volatile uint32_t *)(&sai->reg->FIFORD);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_1;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = 0U;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)data, num);
  } else
#endif
  {
    // Enable I2S FIFO level Interrupts
    Chip_I2S_FIFO_SetInterrupt (sai->reg, I2S_FIFO_INT_RXLVL);
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t SAI_GetTxCount (SAI_RESOURCES *sai)
  \brief       Get transmitted data count.
  \param[in]   sai  Pointer to sai resources
  \return      number of data items transmitted
*/
static uint32_t SAI_GetTxCount (SAI_RESOURCES *sai) {
  volatile uint32_t cnt;

  if ((sai->info->flags & SAI_FLAG_CONFIGURED) == 0U) {
    // SAI is not configured (mode not selected)
    return 0U;
  }

  if (sai->stream_dir != SAI_STREAM_OUT) {
    // Stream is not defined as OUT stream
    return 0U;
  }

#if I2S_TX_DMA_USED
  if (sai->dma != NULL) {
    do {
      // Clear DMA interrupt flag
      sai->info->xfer.dma_int_flag = 0U;

      cnt  = sai->info->xfer.cnt + sai->info->xfer.num_pending;
      cnt -= ((LPC_DMA->DMACH[sai->dma->chid].XFERCFG >> 16U) & 0x3FF) + 1U;
    } while (sai->info->xfer.dma_int_flag != 0U);
  } else
#endif
  {
    cnt = sai->info->xfer.cnt;
  }

  return (sai->info->xfer.cnt);
}

/**
  \fn          uint32_t SAI_GetRxCount (SAI_RESOURCES *sai)
  \brief       Get received data count.
  \param[in]   sai       Pointer to sai resources
  \return      number of data items received
*/
static uint32_t SAI_GetRxCount (SAI_RESOURCES *sai) {
  volatile uint32_t cnt;

  if ((sai->info->flags & SAI_FLAG_CONFIGURED) == 0U) {
    // SAI is not configured (mode not selected)
    return 0U;
  }

  if (sai->stream_dir != SAI_STREAM_OUT) {
    // Stream is not defined as OUT stream
    return 0U;
  }

#if I2S_RX_DMA_USED
  if (sai->dma != NULL) {
    do {
      // Clear DMA interrupt flag
      sai->info->xfer.dma_int_flag = 0U;

      cnt  = sai->info->xfer.cnt + sai->info->xfer.num_pending;
      cnt -= ((LPC_DMA->DMACH[sai->dma->chid].XFERCFG >> 16U) & 0x3FF) + 1U;
    } while (sai->info->xfer.dma_int_flag != 0U);
  } else
#endif
  {
    cnt = sai->info->xfer.cnt;
  }

  return cnt;
}

/**
  \fn          int32_t SAI_Control (uint32_t control, uint32_t arg1, uint32_t arg2, SAI_RESOURCES *sai)
  \brief       Control SAI Interface.
  \param[in]   control  Operation
  \param[in]   arg1     Argument 1 of operation (optional)
  \param[in]   arg2     Argument 2 of operation (optional)
  \param[in]   sai      Pointer to sai resources
  \return      common \ref execution_status and driver specific \ref sai_execution_status
*/
static int32_t SAI_Control (uint32_t control, uint32_t arg1, uint32_t arg2, SAI_RESOURCES *sai) {
  uint32_t div, clk, en;
  uint32_t frame_sync_width, slot_size, slot_count;
  I2S_AUDIO_FORMAT_T audio;

  if ((sai->info->flags & SAI_FLAG_POWERED) == 0U) {
    // SAI not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_SAI_CONTROL_Msk) {
    case ARM_SAI_CONFIGURE_TX:
      if (sai->stream_dir != SAI_STREAM_OUT) {
        // Stream is not defined as OUT stream
        return ARM_DRIVER_ERROR;
      }
      audio.Direction = I2S_TX;
      break;

    case ARM_SAI_CONFIGURE_RX:
      if (sai->stream_dir != SAI_STREAM_IN) {
        // Stream is not defined as IN stream
        return ARM_DRIVER_ERROR;
      }
      audio.Direction = I2S_RX;
      break;

    case ARM_SAI_CONTROL_TX:
      if (sai->stream_dir != SAI_STREAM_OUT) {
        // Stream is not defined as OUT stream
        return ARM_DRIVER_ERROR;
      }

      audio.Direction = I2S_TX;
      if ((arg1 & 1U) == 0U) {

        // Disable TX FIFO level and transmit error Interrupt
        Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_TXERR | I2S_FIFO_INT_TXLVL);

        // Disable DMA Channel
#if I2S_TX_DMA_USED
        if (sai->dma != NULL) {
          Chip_DMA_DisableIntChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_DisableChannel(LPC_DMA, sai->dma->chid);
        }
#endif

        // Stop I2S
        Chip_I2S_Stop (sai->reg);

        // Disable Transmit FIFO
        Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_DISABLE);

      } else {

#if I2S_TX_DMA_USED
        // Enable DMA Channel
        if (sai->dma != NULL) {
          Chip_DMA_EnableChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_EnableIntChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_SetupChannelConfig(LPC_DMA, sai->dma->chid,(DMA_CFG_PERIPHREQEN | DMA_CFG_TRIGTYPE_EDGE | DMA_CFG_TRIGPOL_HIGH | DMA_CFG_BURSTPOWER_8 | DMA_CFG_CHPRIORITY(0)));
        }
#endif

        // Enable transmit error Interrupt
        Chip_I2S_FIFO_SetInterrupt (sai->reg, I2S_FIFO_INT_TXERR);

        // Start I2S
        Chip_I2S_Start (sai->reg);

        // Enable Transmit FIFO
        Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_ENABLE);
      }

      // Mute
      sai->info->xfer.mute = (((arg1 & 2U) != 0U) ? 1U : 0U);
      return ARM_DRIVER_OK;

    case ARM_SAI_CONTROL_RX:
      if (sai->stream_dir != SAI_STREAM_IN) {
        // Stream is not defined as IN stream
        return ARM_DRIVER_ERROR;
      }

      audio.Direction = I2S_RX;
      if ((arg1 & 1U) == 0U) {

        // Disable RX FIFO level and receive error Interrupt
        Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_RXERR | I2S_FIFO_INT_RXLVL);

        // Disable DMA Channel
#if I2S_RX_DMA_USED
        if (sai->dma != NULL) {
          Chip_DMA_DisableIntChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_DisableChannel(LPC_DMA, sai->dma->chid);
        }
#endif

        // Stop I2S
        Chip_I2S_Stop (sai->reg);

        // Disable Receive FIFO
        Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_DISABLE);

      } else {

        // Enable DMA Channel
#if I2S_RX_DMA_USED
        if (sai->dma != NULL) {
          Chip_DMA_EnableChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_EnableIntChannel(LPC_DMA, sai->dma->chid);
          Chip_DMA_SetupChannelConfig(LPC_DMA, sai->dma->chid,(DMA_CFG_PERIPHREQEN | DMA_CFG_TRIGTYPE_EDGE | DMA_CFG_TRIGPOL_HIGH | DMA_CFG_BURSTPOWER_8 | DMA_CFG_CHPRIORITY(0)));
        }
#endif

        // Enable receive error Interrupt
        Chip_I2S_FIFO_SetInterrupt (sai->reg, I2S_FIFO_INT_RXERR);

         // Start I2S
        Chip_I2S_Start (sai->reg);

        // Enable Receive FIFO
        Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_ENABLE);
      }

      return ARM_DRIVER_OK;

    case ARM_SAI_MASK_SLOTS_TX:
      return ARM_DRIVER_OK;

    case ARM_SAI_MASK_SLOTS_RX:
      return ARM_DRIVER_OK;

    case ARM_SAI_ABORT_SEND:
      if (sai->stream_dir != SAI_STREAM_OUT) {
        // Stream is not defined as OUT stream
        return ARM_DRIVER_ERROR;
      }

#if I2S_TX_DMA_USED
      if (sai->dma != NULL) {
        // Abort DMA Channel
        Chip_DMA_AbortChannel (LPC_DMA, sai->dma->chid);
      }
#endif

      // Disable TX FIFO Level Interrupt
      Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_TXLVL);

      // Empty transmit FIFO
      audio.Direction = I2S_TX;
      Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_CLEAR);

      sai->info->xfer.num = 0U;

      // Clear busy flag
      sai->info->status.busy = 0U;

      return ARM_DRIVER_OK;

    case ARM_SAI_ABORT_RECEIVE:
      if (sai->stream_dir != SAI_STREAM_IN) {
        // Stream is not defined as IN stream
        return ARM_DRIVER_ERROR;
      }

#if I2S_RX_DMA_USED
      if (sai->dma != NULL) {
        // Abort DMA Channel
        Chip_DMA_AbortChannel (LPC_DMA, sai->dma->chid);
      }
#endif

      // Disable RX FIFO Level Interrupt
      Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_RXLVL);

      // Empty receive FIFO
      audio.Direction = I2S_RX;
      Chip_I2S_FIFO_Control (sai->reg, &audio, I2S_FIFO_CLEAR);

      sai->info->xfer.num = 0U;

      // Clear busy flag
      sai->info->status.busy = 0U;
      return ARM_DRIVER_OK;

    default: return ARM_DRIVER_ERROR;
  }

  // Mode
  switch (control & ARM_SAI_MODE_Msk) {
    case ARM_SAI_MODE_MASTER:
      audio.MSCfg = NORMAL_MASTER;
      break;
    case ARM_SAI_MODE_SLAVE:
      audio.MSCfg = NORMAL_SLAVE;
      break;
    default: return ARM_DRIVER_ERROR;
  }

  // Synchronization - only asynchronous mode supported
  if ((control & ARM_SAI_SYNCHRONIZATION_Msk) != ARM_SAI_ASYNCHRONOUS) {
    return ARM_SAI_ERROR_SYNCHRONIZATION;
  }

  // Data size
  sai->info->xfer.data_bits = ((control & ARM_SAI_DATA_SIZE_Msk) >> ARM_SAI_DATA_SIZE_Pos) + 1;
  audio.WordWidth = sai->info->xfer.data_bits;

  if (audio.WordWidth < 8U) {
    return ARM_SAI_ERROR_DATA_SIZE;
  }
  if (sai->stream_dir == SAI_STREAM_IN) {
    // Errata limitation
    if (audio.WordWidth > 24U) {
      return ARM_SAI_ERROR_DATA_SIZE;
    }
  } else {
    if (audio.WordWidth > 32U) {
      return ARM_SAI_ERROR_DATA_SIZE;
    }
  }

  // Bit Order
  if ((control & ARM_SAI_BIT_ORDER_Msk) != ARM_SAI_MSB_FIRST) {
    return ARM_SAI_ERROR_BIT_ORDER;
  }

  // Clock polarity
  switch (control & ARM_SAI_CLOCK_POLARITY_Msk) {
    case ARM_SAI_CLOCK_POLARITY_0:
      audio.SCKPol = false;
      break;
    case ARM_SAI_CLOCK_POLARITY_1 :
      audio.SCKPol = true;
      break;
    default: return ARM_SAI_ERROR_CLOCK_POLARITY;
  }

  // Get Frame length
  audio.FrameWidth = ((arg1 & ARM_SAI_FRAME_LENGTH_Msk) >> ARM_SAI_FRAME_LENGTH_Pos) + 1U;
  // Check if default value
  if (audio.FrameWidth == 1U) { audio.FrameWidth = 0U; }


  // Slot offset
  audio.DataPos = (arg1 & ARM_SAI_SLOT_OFFSET_Msk) >> ARM_SAI_SLOT_OFFSET_Pos;
  if (audio.DataPos != 0U ) {
    return ARM_SAI_ERROR_SLOT_OFFESET;
  }


  if ((arg1 & ARM_SAI_FRAME_SYNC_POLARITY_Msk) == ARM_SAI_FRAME_SYNC_POLARITY_HIGH) {
    audio.WSPol = true;
  } else {
    audio.WSPol = false;
  }

  frame_sync_width   = (arg1 & ARM_SAI_FRAME_SYNC_WIDTH_Msk) >> ARM_SAI_FRAME_SYNC_WIDTH_Pos;
  if (frame_sync_width > 255U) {
    return ARM_SAI_ERROR_FRAME_SYNC_WIDTH;
  } else {
    frame_sync_width++;
  }

  switch ((arg1 & ARM_SAI_SLOT_SIZE_Msk) >> ARM_SAI_SLOT_SIZE_Pos) {
    case ARM_SAI_SLOT_SIZE_DEFAULT:
      slot_size = audio.WordWidth;
      break;
    case ARM_SAI_SLOT_SIZE_16:
      slot_size = 16U;
      break;
    case ARM_SAI_SLOT_SIZE_32:
      slot_size = 32U;
      break;
    default: return ARM_SAI_ERROR_SLOT_SIZE;
  }

  slot_count = ((arg1 & ARM_SAI_SLOT_COUNT_Msk) >> ARM_SAI_SLOT_COUNT_Pos) + 1U;

  switch (control & ARM_SAI_PROTOCOL_Msk) {
    // User Protocol
    case ARM_SAI_PROTOCOL_USER:
      if (((arg1 & ARM_SAI_SLOT_OFFSET_Msk) >> ARM_SAI_SLOT_OFFSET_Pos) != 0U) {
        // Slot offset is not supported
        return ARM_SAI_ERROR_SLOT_OFFESET;
      }

        audio.RightLow  = false;
        audio.LeftJust  = false;
        audio.PDMData   = false;
        audio.DataPos   = 0U;

      if ((arg1 & ARM_SAI_FRAME_SYNC_EARLY) != 0U) {
        // Frame sync early only in mode0 (classic I2S mode)

        if ((frame_sync_width != audio.WordWidth)        ||
            (slot_size        != audio.WordWidth)        ||
            (audio.FrameWidth != (audio.WordWidth * 2U)) ||
            (slot_count       != 2U)) {
          return ARM_SAI_ERROR_FRAME_SYNC_EARLY;
        }

        audio.Mode = I2S_CLASSIC;

      } else if (frame_sync_width == 1U) {
        // Frame sync width is 1 - > mode2

        if (slot_size != audio.WordWidth) {
          return ARM_SAI_ERROR_SLOT_SIZE;
        }

        if (audio.FrameWidth != (slot_size * slot_count)) {
          return ARM_SAI_ERROR_FRAME_LENGHT;
        }

        audio.Mode = DSP_WS_SHORT;

      } else if (frame_sync_width * 2U == audio.FrameWidth) {
        // Frame sync width is half of frame long -> mode1

        if (slot_size != audio.WordWidth) {
          return ARM_SAI_ERROR_SLOT_SIZE;
        }

        audio.Mode = DSP_WS_50;

      } else if (frame_sync_width == slot_size) {
        // Frame size is slot size long - > mode3

        if (audio.WordWidth * 2U < audio.FrameWidth) {
          return ARM_SAI_ERROR_FRAME_LENGHT;
        }

        if (audio.WordWidth != slot_size) {
          return ARM_SAI_ERROR_SLOT_SIZE;
        }

        audio.Mode = DSP_WS_LONG;
      } else {
        return ARM_SAI_ERROR_PROTOCOL;
      }
      break;

    // I2S
    case ARM_SAI_PROTOCOL_I2S:
      if (audio.FrameWidth == 0U) {
        audio.FrameWidth = audio.WordWidth * 2U;
      } else {
        if (audio.FrameWidth != (audio.WordWidth * 2U)) { return ARM_SAI_ERROR_FRAME_LENGHT; }
      }
      audio.Mode      = I2S_CLASSIC;
      audio.RightLow  = false;
      audio.LeftJust  = false;
      audio.PDMData   = false;
      audio.WSPol     = false;
      audio.DataPos   = 0U;

      break;

    // MSB Justified
    case ARM_SAI_PROTOCOL_MSB_JUSTIFIED:
      if (audio.FrameWidth == 0U) {
        audio.FrameWidth = audio.WordWidth * 2U;
      } else {
        return ARM_SAI_ERROR_FRAME_LENGHT;
      }
      audio.Mode      = DSP_WS_50;
      audio.RightLow  = false;
      audio.LeftJust  = false;
      audio.PDMData   = false;
      audio.WSPol     = true;
      audio.DataPos   = 0U;

      break;

    // LSB Justified
    case ARM_SAI_PROTOCOL_LSB_JUSTIFIED:
      if (audio.FrameWidth == 0U) {
        audio.FrameWidth = audio.WordWidth * 2U;
      } else {
        return ARM_SAI_ERROR_FRAME_LENGHT;
      }

      // Offset: Slot size - Data size
      audio.DataPos   = (audio.FrameWidth / 2U) - audio.WordWidth;
      
      audio.Mode      = DSP_WS_50;
      audio.RightLow  = false;
      audio.LeftJust  = false;
      audio.PDMData   = false;
      audio.WSPol     = true;

      break;

    // PCM Long
    case ARM_SAI_PROTOCOL_PCM_LONG:
      if (audio.WordWidth != 13U) {
        return ARM_SAI_ERROR_DATA_SIZE;
      }
      if (audio.FrameWidth == 0U) {
        audio.FrameWidth = audio.WordWidth * 2U;
      } else {
        if (audio.FrameWidth != (audio.WordWidth * 2U)) {
          return ARM_SAI_ERROR_FRAME_LENGHT;
        }
      }

      audio.Mode      = DSP_WS_50;
      audio.RightLow  = false;
      audio.LeftJust  = false;
      audio.PDMData   = false;
      audio.WSPol     = true;
      audio.DataPos   = 0U;

    case ARM_SAI_PROTOCOL_PCM_SHORT:
    case ARM_SAI_PROTOCOL_AC97:
    default: return ARM_SAI_ERROR_PROTOCOL;
  }

  // Mono mode - not supported
  if (control & ARM_SAI_MONO_MODE) {
    return ARM_SAI_ERROR_MONO_MODE;
  }

  // Companding
  if ((control & ARM_SAI_COMPANDING_Msk) != ARM_SAI_COMPANDING_NONE)  { return ARM_SAI_ERROR_COMPANDING; }

  // Audio Frequency
  // I2S Clock must be properly configured externally
  if (audio.MSCfg == NORMAL_MASTER) {
    // WS and SCK are generated only by master
    if (sai->reg == LPC_I2S6) {
      clk = Chip_Clock_GetFLEXCOMMClockRate (6U);
    } else if (sai->reg == LPC_I2S7) {
      clk = Chip_Clock_GetFLEXCOMMClockRate (7U);
    } else {
      return ARM_DRIVER_ERROR;
    }

    audio.Divider = clk / ((arg2 & ARM_SAI_AUDIO_FREQ_Msk) * audio.FrameWidth);

    // Round Up
    div = (clk << 4) / ((arg2 & ARM_SAI_AUDIO_FREQ_Msk) * audio.FrameWidth);
    if ((div & 0x0FU) > 7U) {
      audio.Divider++;
    }
  } else {
    audio.Divider = 1U;
  }

  en = sai->reg->CFG1 & 1;
  Chip_I2S_Config (sai->reg, &audio);
  sai->reg->CFG1 |= en;
  

  sai->info->flags |= SAI_FLAG_CONFIGURED;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_SAI_STATUS SAI_GetStatus (SAI_RESOURCES *sai)
  \brief       Get SAI status.
  \param[in]   sai  Pointer to sai resources
  \return      SAI status \ref ARM_SAI_STATUS
*/
static ARM_SAI_STATUS SAI_GetStatus (SAI_RESOURCES *sai) {
  ARM_SAI_STATUS status;

  status.frame_error  = sai->info->status.frame_error;
  status.rx_busy      = sai->info->status.busy;
  status.rx_overflow  = sai->info->status.under_over_flow;
  status.tx_busy      = sai->info->status.busy;
  status.tx_underflow = sai->info->status.under_over_flow;

  return status;
}

/**
  \fn          void SAI_IRQHandler (SAI_RESOURCES *sai)
  \param[in]   sai  Pointer to sai resources
  \brief       SAI Interrupt handler.
*/
void SAI_IRQHandler (SAI_RESOURCES *sai) {
  uint32_t val, cnt, stat, fifo_stat_int, event;
  uint32_t data;
  uint16_t *ptr_data_16;
  uint32_t *ptr_data_32;

  // Get FIFO pending interrupts
  fifo_stat_int = Chip_I2S_FIFO_GetPendingInts (sai->reg);

  // Get I2S status
  stat = Chip_I2S_GetStatus (sai->reg);

  // Clear FIFO Status
  Chip_I2S_ClrFIFOStatus (sai->reg, I2S_FIFO_STAT_TXERR | I2S_FIFO_STAT_RXERR);

  event = 0U;

  // Slave Frame Error
  if (stat & I2S_STAT_SLVFRMERR) {
    // Update status
    sai->info->status.frame_error = 1U;

    // Clear flag
    Chip_I2S_ClearStatus (sai->reg, I2S_STAT_SLVFRMERR);
    event |= ARM_SAI_EVENT_TX_UNDERFLOW;
  }

  // Stream OUT - Transmit
  if (sai->stream_dir == SAI_STREAM_OUT) {

    // Transmit FIFO error interrupt
    if (fifo_stat_int & I2S_FIFO_INT_TXERR) {
      // Update status
      sai->info->status.under_over_flow = 1U;

      // Set Transmit underflow interrupt
      event |= ARM_SAI_EVENT_TX_UNDERFLOW;
    }

    // Transmit FIFO Level interrupt
    if (fifo_stat_int & I2S_FIFO_INT_TXLVL) {

      // Available space in FIFO
      cnt = 8U - Chip_I2S_GetFIFOTxLevel (sai->reg);

      if (sai->info->xfer.data_bits <= 16U) {
        // 32-bit FIFO entry -> Left and Right data can fit in one entry
        cnt *= 2U;
      }

      val = sai->info->xfer.num - sai->info->xfer.cnt;
      if (cnt > val) { cnt = val; }

      // 17 .. 32 Data Bits
      if (sai->info->xfer.data_bits > 16U) {
        ptr_data_32 = (uint32_t *)sai->info->xfer.buf;
        sai->info->xfer.buf += 4U * cnt;
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          Chip_I2S_Send (sai->reg, *(ptr_data_32++));
          sai->info->xfer.cnt++;
        }

      // 9 .. 16 Data Bits
      } else if (sai->info->xfer.data_bits > 8U) {
        ptr_data_16 = (uint16_t *)sai->info->xfer.buf;
        sai->info->xfer.buf += 2U * cnt;
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          data = *ptr_data_16++;
          sai->info->xfer.cnt++;

          if (sai->info->xfer.cnt < cnt) {
            data |= (*ptr_data_16++) << 16;
            sai->info->xfer.cnt++;
          }
          Chip_I2S_Send (sai->reg, data);
        }

      // 4 .. 8 Data Bits
      } else {
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          data = *sai->info->xfer.buf++;
          sai->info->xfer.cnt++;
          if (sai->info->xfer.cnt < cnt) {
            data |= (*sai->info->xfer.buf++) << 16;
            sai->info->xfer.cnt++;
          }
          Chip_I2S_Send (sai->reg, data);
        }
      }

      // Check if all data is transmitted
      if (sai->info->xfer.cnt == sai->info->xfer.num) {
        // Disable I2S FIFO level Interrupts
        Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_TXLVL);

        // Clear TX busy flag
        sai->info->status.busy = 0U;

        // Set Send complete event
        event |= ARM_SAI_EVENT_SEND_COMPLETE;
      }
    }
  }

  // Stream IN - Receive
  if (sai->stream_dir == SAI_STREAM_IN) {

    // Receive FIFO error interrupt
    if (fifo_stat_int & I2S_FIFO_INT_RXERR) {
      // Update status
      sai->info->status.under_over_flow = 1U;

      // Set Transmit underflow interrupt
      event |= ARM_SAI_EVENT_RX_OVERFLOW;
    }

    // Receive FIFO Level interrupt
    if (fifo_stat_int & I2S_FIFO_INT_RXLVL) {
      // Available space in FIFO
      cnt = Chip_I2S_GetFIFORxLevel (sai->reg);
      
      if (sai->info->xfer.data_bits <= 16U) {
         // 32-bit FIFO entry -> Left and Right data can fit in one entry
         cnt *= 2U;
      }

      val = sai->info->xfer.num - sai->info->xfer.cnt;
      if (cnt > val) { cnt = val; }

      // 17 .. 32 Data Bits
      if (sai->info->xfer.data_bits > 16U) {
        ptr_data_32 = (uint32_t *)sai->info->xfer.buf;
        sai->info->xfer.buf += 4U * cnt;
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          *ptr_data_32++ = Chip_I2S_Receive (sai->reg);
          sai->info->xfer.cnt++;
        }

      // 9 .. 16 Data Bits
      } else if (sai->info->xfer.data_bits > 8U) {
        ptr_data_16 = (uint16_t *)sai->info->xfer.buf;
        sai->info->xfer.buf += 2U * cnt;
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          data = Chip_I2S_Receive (sai->reg);

          *ptr_data_16++ = data & 0xFFFFU;
          sai->info->xfer.cnt++;

          if (sai->info->xfer.cnt < cnt) {
            *ptr_data_16++ = (data >> 16) & 0xFFFFU;
            sai->info->xfer.cnt++;
          }
        }

      // 4 .. 8 Data Bits
      } else {
        cnt += sai->info->xfer.cnt;
        while (sai->info->xfer.cnt < cnt) {
          data = Chip_I2S_Receive (sai->reg);

          *sai->info->xfer.buf++ = data & 0xFFU;
          sai->info->xfer.cnt++;

          if (sai->info->xfer.cnt < cnt) {
            *sai->info->xfer.buf++ = (data >> 16) & 0xFFU;
            sai->info->xfer.cnt++;
          }
        }
      }

      // Check if all data is received
      if (sai->info->xfer.cnt == sai->info->xfer.num) {
        // Disable I2S FIFO level Interrupts
        Chip_I2S_FIFO_ClrInterrupt (sai->reg, I2S_FIFO_INT_RXLVL);

        // Clear RX busy flag
        sai->info->status.busy = 0U;

        // Set Send complete event
        event |= ARM_SAI_EVENT_RECEIVE_COMPLETE;
      }
    }
  }

  // Signal Events
  if ((event != 0U) && (sai->info->cb_event != NULL)) {
    sai->info->cb_event (event);
  }
}

#if I2S_TX_DMA_USED
/**
  \fn          void SAI_TX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai)
  \brief       SAI Transmit DMA Interrupt handler.
*/
static void SAI_TX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai) {
  uint32_t                  num;
  uint32_t                  data_addr;
  DMA_PERIPHERAL_CONTEXT_T  dma_context;

  // Set DMA interrupt flag
  sai->info->xfer.dma_int_flag = 1U;

  sai->info->xfer.cnt += sai->info->xfer.num_pending;

  if (sai->info->xfer.cnt < sai->info->xfer.num) {
    // Still data to transmit
    data_addr = (uint32_t)sai->info->xfer.buf;

    num = sai->info->xfer.num - sai->info->xfer.cnt;
    if (num > 1024U) { num = 1024U; }

    sai->info->xfer.num_pending = num;

    dma_context.width = DMA_XFERCFG_WIDTH_32;
    if (sai->info->xfer.data_bits <= 16) {
      num *= 2U;
    } else {
      num *= 4U;
    }

    sai->info->xfer.buf += num;

    dma_context.channel           = sai->dma->chid;
    dma_context.register_location = &(sai->reg->FIFOWR);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_1;
    dma_context.write             = 1U;

    Chip_DMASERVICE_SingleBuffer (&dma_context, data_addr, num);
  } else {
    // All requested data has been transmitted

    // Clear TX busy flag
    sai->info->status.busy = 0U;

    if (sai->info->cb_event) {
      sai->info->cb_event (ARM_SAI_EVENT_SEND_COMPLETE);
    }
  }
}
#endif

#if I2S_RX_DMA_USED
/**
  \fn          void SAI_RX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai)
  \brief       SAI Receive DMA Interrupt handler.
*/
static void SAI_RX_DMA_IRQHandler (int32_t val, SAI_RESOURCES *sai) {
  uint32_t                  num;
  uint32_t                  data_addr;
  DMA_PERIPHERAL_CONTEXT_T  dma_context;

  // Set DMA interrupt flag
  sai->info->xfer.dma_int_flag = 1U;

  sai->info->xfer.cnt += sai->info->xfer.num_pending;

  if (sai->info->xfer.cnt < sai->info->xfer.num) {
    // Still data to receive
    data_addr = (uint32_t)sai->info->xfer.buf;

    num = sai->info->xfer.num - sai->info->xfer.cnt;
    if (num > 1024U) { num = 1024U; }

    sai->info->xfer.num_pending = num;

    dma_context.width = DMA_XFERCFG_WIDTH_32;
    if (sai->info->xfer.data_bits <= 16) {
      num *= 2U;
    } else {
      num *= 4U;
    }

    sai->info->xfer.buf += num;

    dma_context.channel           = sai->dma->chid;
    dma_context.register_location = (volatile uint32_t *)&(sai->reg->FIFORD);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_1;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = 0U;

    Chip_DMASERVICE_SingleBuffer (&dma_context, data_addr, num);
  } else {
    // All requested data has been received

    // Clear RX busy flag
    sai->info->status.busy = 0U;

    if (sai->info->cb_event) {
      sai->info->cb_event (ARM_SAI_EVENT_RECEIVE_COMPLETE);
    }
  }
}
#endif

#if (RTE_SAI0)
// SAI0 Driver Wrapper functions
static ARM_SAI_CAPABILITIES SAI0_GetCapabilities (void) {
  return SAI_GetCapabilities (&SAI0_Resources);
}

static int32_t SAI0_Initialize (ARM_SAI_SignalEvent_t cb_event) {
  return SAI_Initialize (cb_event, &SAI0_Resources);
}

static int32_t SAI0_Uninitialize (void) {
  return SAI_Uninitialize (&SAI0_Resources);
}

static int32_t SAI0_PowerControl (ARM_POWER_STATE state) {
  return SAI_PowerControl (state, &SAI0_Resources);
}

static int32_t SAI0_Send (const void *data, uint32_t num) {
  return SAI_Send (data, num, &SAI0_Resources);
}

static int32_t SAI0_Receive (void *data, uint32_t num) {
  return SAI_Receive (data, num, &SAI0_Resources);
}

static uint32_t SAI0_GetTxCount (void) {
  return SAI_GetTxCount (&SAI0_Resources);
}

static uint32_t SAI0_GetRxCount (void) {
  return SAI_GetRxCount (&SAI0_Resources);
}

static int32_t SAI0_Control (uint32_t control, uint32_t arg1, uint32_t arg2) {
  return SAI_Control (control, arg1, arg2, &SAI0_Resources);
}

static ARM_SAI_STATUS SAI0_GetStatus (void) {
  return SAI_GetStatus (&SAI0_Resources);
}

void I2S6_IRQHandler (void) {
  SAI_IRQHandler (&SAI0_Resources);
}

#if ((SAI0_STREAM == SAI_STREAM_OUT) && (I2S6_TX_DMA_EN != 0))
void SAI0_TX_DMA_Callback (int32_t event) {
  SAI_TX_DMA_IRQHandler (event, &SAI0_Resources);
}
#endif

#if ((SAI0_STREAM == SAI_STREAM_IN) && (I2S6_RX_DMA_EN != 0))
void SAI0_RX_DMA_Callback (int32_t event) {
  SAI_RX_DMA_IRQHandler (event, &SAI0_Resources);
}
#endif

// SAI Driver Control Block
ARM_DRIVER_SAI Driver_SAI0 = {
    SAI_GetVersion,
    SAI0_GetCapabilities,
    SAI0_Initialize,
    SAI0_Uninitialize,
    SAI0_PowerControl,
    SAI0_Send,
    SAI0_Receive,
    SAI0_GetTxCount,
    SAI0_GetRxCount,
    SAI0_Control,
    SAI0_GetStatus
};
#endif

#if (RTE_SAI1)
// SAI1 Driver Wrapper functions
static ARM_SAI_CAPABILITIES SAI1_GetCapabilities (void) {
  return SAI_GetCapabilities (&SAI1_Resources);
}

static int32_t SAI1_Initialize (ARM_SAI_SignalEvent_t cb_event) {
  return SAI_Initialize (cb_event, &SAI1_Resources);
}

static int32_t SAI1_Uninitialize (void) {
  return SAI_Uninitialize (&SAI1_Resources);
}

static int32_t SAI1_PowerControl (ARM_POWER_STATE state) {
  return SAI_PowerControl (state, &SAI1_Resources);
}

static int32_t SAI1_Send (const void *data, uint32_t num) {
  return SAI_Send (data, num, &SAI1_Resources);
}

static int32_t SAI1_Receive (void *data, uint32_t num) {
  return SAI_Receive (data, num, &SAI1_Resources);
}

static uint32_t SAI1_GetTxCount (void) {
  return SAI_GetTxCount (&SAI1_Resources);
}

static uint32_t SAI1_GetRxCount (void) {
  return SAI_GetRxCount (&SAI1_Resources);
}

static int32_t SAI1_Control (uint32_t control, uint32_t arg1, uint32_t arg2) {
  return SAI_Control (control, arg1, arg2, &SAI1_Resources);
}

static ARM_SAI_STATUS SAI1_GetStatus (void) {
  return SAI_GetStatus (&SAI1_Resources);
}

void I2S7_IRQHandler (void) {
  SAI_IRQHandler (&SAI1_Resources);
}

#if ((SAI1_STREAM == SAI_STREAM_OUT) && (I2S7_TX_DMA_EN != 0))
void SAI1_TX_DMA_Callback (int32_t event) {
  SAI_TX_DMA_IRQHandler (event, &SAI1_Resources);
}
#endif

#if ((SAI1_STREAM == SAI_STREAM_IN) && (I2S7_RX_DMA_EN != 0))
void SAI1_RX_DMA_Callback (int32_t event) {
  SAI_RX_DMA_IRQHandler (event, &SAI1_Resources);
}
#endif

// SAI Driver Control Block
ARM_DRIVER_SAI Driver_SAI1 = {
    SAI_GetVersion,
    SAI1_GetCapabilities,
    SAI1_Initialize,
    SAI1_Uninitialize,
    SAI1_PowerControl,
    SAI1_Send,
    SAI1_Receive,
    SAI1_GetTxCount,
    SAI1_GetRxCount,
    SAI1_Control,
    SAI1_GetStatus
};
#endif
