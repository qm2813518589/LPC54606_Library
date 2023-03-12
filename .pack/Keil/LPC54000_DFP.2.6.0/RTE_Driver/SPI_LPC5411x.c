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
 * $Date:        22. August 2016
 * $Revision:    V1.1
 *
 * Driver:       Driver_SPI0, Driver_SPI1, Driver_SPI2, Driver_SPI3,
 *               Driver_SPI4, Driver_SPI5, Driver_SPI6, Driver_SPI7
 * Configured:   via RTE_Device.h configuration file
 * Project:      SPI Driver for NXP LPC5411x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   SPI Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_SPI# = 0       use SPI0
 *   Connect to hardware via Driver_SPI# = 1       use SPI1
 *   Connect to hardware via Driver_SPI# = 2       use SPI2
 *   Connect to hardware via Driver_SPI# = 3       use SPI3
 *   Connect to hardware via Driver_SPI# = 4       use SPI4
 *   Connect to hardware via Driver_SPI# = 5       use SPI5
 *   Connect to hardware via Driver_SPI# = 6       use SPI6
 *   Connect to hardware via Driver_SPI# = 7       use SPI7
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
 *    - Code which has been bypassing a bug in Chip_DMASERVICE_SingleBuffer() is updated, due to 
 *      correction of the Chip_DMASERVICE_SingleBuffer() function.
 *  Version 1.0
 *    - Initial release
 */

#include "SPI_LPC5411x.h"

#define ARM_SPI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,1) /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_SPI_API_VERSION,
  ARM_SPI_DRV_VERSION
};

// Driver Capabilities
static const ARM_SPI_CAPABILITIES DriverCapabilities = {
  0,  // Simplex Mode (Master and Slave)
  0,  // TI Synchronous Serial Interface
  0,  // Microwire Interface
  0   // Signal Mode Fault event: \ref ARM_SPI_EVENT_MODE_FAULT
};


#if (RTE_SPI0)
/* SPI0 Control Information */
static SPI_INFO SPI0_Ctrl = { 0 };

#if defined(SPI0_DMA_EN)
static       void    SPI0_TX_DMA_Callback (int32_t);
static       void    SPI0_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI0_Tx_DMA = { DMAREQ_SPI0_TX, &SPI0_TX_DMA_Callback };
static const SPI_DMA SPI0_Rx_DMA = { DMAREQ_SPI0_RX, &SPI0_RX_DMA_Callback };
#endif

#if defined(SPI0_SSEL_PORT)
static SPI_SSEL spi0_ssel = { SPI0_SSEL_PORT,
                              SPI0_SSEL_PIN,
                              SPI0_SSEL_FUNC,
                              SPI0_SSEL_NUM };
#endif

/* SPI0 Resources */
static SPI_RESOURCES SPI0_Resources = {
  LPC_SPI0,
  SPI0_IRQn,
  SPI0_SYSCON_CLOCK,
  { SPI0_MOSI_PORT, SPI0_MOSI_PIN, SPI0_MOSI_FUNC },
  { SPI0_MISO_PORT, SPI0_MISO_PIN, SPI0_MISO_FUNC },
  { SPI0_SCK_PORT,  SPI0_SCK_PIN,  SPI0_SCK_FUNC  },
  #if defined(SPI0_SSEL_PORT)
  &spi0_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI0_DMA_EN)
  &SPI0_Tx_DMA,
  &SPI0_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI0_Ctrl
};
#endif /* RTE_SPI0 */


#if (RTE_SPI1)
/* SPI1 Control Information */
static SPI_INFO SPI1_Ctrl = { 0 };

#if defined(SPI1_DMA_EN)
static       void    SPI1_TX_DMA_Callback (int32_t);
static       void    SPI1_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI1_Tx_DMA = { DMAREQ_SPI1_TX, &SPI1_TX_DMA_Callback };
static const SPI_DMA SPI1_Rx_DMA = { DMAREQ_SPI1_RX, &SPI1_RX_DMA_Callback };
#endif

#if defined(SPI1_SSEL_PORT)
static SPI_SSEL spi1_ssel = { SPI1_SSEL_PORT,
                              SPI1_SSEL_PIN,
                              SPI1_SSEL_FUNC,
                              SPI1_SSEL_NUM };
#endif

/* SPI1 Resources */
static SPI_RESOURCES SPI1_Resources = {
  LPC_SPI1,
  SPI1_IRQn,
  SPI1_SYSCON_CLOCK,
  { SPI1_MOSI_PORT, SPI1_MOSI_PIN, SPI1_MOSI_FUNC },
  { SPI1_MISO_PORT, SPI1_MISO_PIN, SPI1_MISO_FUNC },
  { SPI1_SCK_PORT,  SPI1_SCK_PIN,  SPI1_SCK_FUNC  },
  #if defined(SPI1_SSEL_PORT)
  &spi1_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI1_DMA_EN)
  &SPI1_Tx_DMA,
  &SPI1_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI1_Ctrl
};
#endif /* RTE_SPI1 */


#if (RTE_SPI2)
/* SPI2 Control Information */
static SPI_INFO SPI2_Ctrl = { 0 };

#if defined(SPI2_DMA_EN)
static       void    SPI2_TX_DMA_Callback (int32_t);
static       void    SPI2_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI2_Tx_DMA = { DMAREQ_SPI2_TX, &SPI2_TX_DMA_Callback };
static const SPI_DMA SPI2_Rx_DMA = { DMAREQ_SPI2_RX, &SPI2_RX_DMA_Callback };
#endif

#if defined(SPI2_SSEL_PORT)
static SPI_SSEL spi2_ssel = { SPI2_SSEL_PORT,
                              SPI2_SSEL_PIN,
                              SPI2_SSEL_FUNC,
                              SPI2_SSEL_NUM };
#endif

/* SPI2 Resources */
static SPI_RESOURCES SPI2_Resources = {
  LPC_SPI2,
  SPI2_IRQn,
  SPI2_SYSCON_CLOCK,
  { SPI2_MOSI_PORT, SPI2_MOSI_PIN, SPI2_MOSI_FUNC },
  { SPI2_MISO_PORT, SPI2_MISO_PIN, SPI2_MISO_FUNC },
  { SPI2_SCK_PORT,  SPI2_SCK_PIN,  SPI2_SCK_FUNC  },
  #if defined(SPI2_SSEL_PORT)
  &spi2_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI2_DMA_EN)
  &SPI2_Tx_DMA,
  &SPI2_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI2_Ctrl
};
#endif /* RTE_SPI2 */


#if (RTE_SPI3)
/* SPI3 Control Information */
static SPI_INFO SPI3_Ctrl = { 0 };

#if defined(SPI3_DMA_EN)
static       void    SPI3_TX_DMA_Callback (int32_t);
static       void    SPI3_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI3_Tx_DMA = { DMAREQ_SPI3_TX, &SPI3_TX_DMA_Callback };
static const SPI_DMA SPI3_Rx_DMA = { DMAREQ_SPI3_RX, &SPI3_RX_DMA_Callback };
#endif

#if defined(SPI3_SSEL_PORT)
static SPI_SSEL spi3_ssel = { SPI3_SSEL_PORT,
                              SPI3_SSEL_PIN,
                              SPI3_SSEL_FUNC,
                              SPI3_SSEL_NUM };
#endif

/* SPI3 Resources */
static SPI_RESOURCES SPI3_Resources = {
  LPC_SPI3,
  SPI3_IRQn,
  SPI3_SYSCON_CLOCK,
  { SPI3_MOSI_PORT, SPI3_MOSI_PIN, SPI3_MOSI_FUNC },
  { SPI3_MISO_PORT, SPI3_MISO_PIN, SPI3_MISO_FUNC },
  { SPI3_SCK_PORT,  SPI3_SCK_PIN,  SPI3_SCK_FUNC  },
  #if defined(SPI3_SSEL_PORT)
  &spi3_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI3_DMA_EN)
  &SPI3_Tx_DMA,
  &SPI3_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI3_Ctrl
};
#endif /* RTE_SPI3 */


#if (RTE_SPI4)
/* SPI4 Control Information */
static SPI_INFO SPI4_Ctrl = { 0 };

#if defined(SPI4_DMA_EN)
static       void    SPI4_TX_DMA_Callback (int32_t);
static       void    SPI4_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI4_Tx_DMA = { DMAREQ_SPI4_TX, &SPI4_TX_DMA_Callback };
static const SPI_DMA SPI4_Rx_DMA = { DMAREQ_SPI4_RX, &SPI4_RX_DMA_Callback };
#endif

#if defined(SPI4_SSEL_PORT)
static SPI_SSEL spi4_ssel = { SPI4_SSEL_PORT,
                              SPI4_SSEL_PIN,
                              SPI4_SSEL_FUNC,
                              SPI4_SSEL_NUM };
#endif

/* SPI4 Resources */
static SPI_RESOURCES SPI4_Resources = {
  LPC_SPI4,
  SPI4_IRQn,
  SPI4_SYSCON_CLOCK,
  { SPI4_MOSI_PORT, SPI4_MOSI_PIN, SPI4_MOSI_FUNC },
  { SPI4_MISO_PORT, SPI4_MISO_PIN, SPI4_MISO_FUNC },
  { SPI4_SCK_PORT,  SPI4_SCK_PIN,  SPI4_SCK_FUNC  },
  #if defined(SPI4_SSEL_PORT)
  &spi4_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI4_DMA_EN)
  &SPI4_Tx_DMA,
  &SPI4_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI4_Ctrl
};
#endif /* RTE_SPI4 */


#if (RTE_SPI5)
/* SPI5 Control Information */
static SPI_INFO SPI5_Ctrl = { 0 };

#if defined(SPI5_DMA_EN)
static       void    SPI5_TX_DMA_Callback (int32_t);
static       void    SPI5_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI5_Tx_DMA = { DMAREQ_SPI5_TX, &SPI5_TX_DMA_Callback };
static const SPI_DMA SPI5_Rx_DMA = { DMAREQ_SPI5_RX, &SPI5_RX_DMA_Callback };
#endif

#if defined(SPI5_SSEL_PORT)
static SPI_SSEL spi5_ssel = { SPI5_SSEL_PORT,
                              SPI5_SSEL_PIN,
                              SPI5_SSEL_FUNC,
                              SPI5_SSEL_NUM };
#endif

/* SPI5 Resources */
static SPI_RESOURCES SPI5_Resources = {
  LPC_SPI5,
  SPI5_IRQn,
  SPI5_SYSCON_CLOCK,
  { SPI5_MOSI_PORT, SPI5_MOSI_PIN, SPI5_MOSI_FUNC },
  { SPI5_MISO_PORT, SPI5_MISO_PIN, SPI5_MISO_FUNC },
  { SPI5_SCK_PORT,  SPI5_SCK_PIN,  SPI5_SCK_FUNC  },
  #if defined(SPI5_SSEL_PORT)
  &spi5_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI5_DMA_EN)
  &SPI5_Tx_DMA,
  &SPI5_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI5_Ctrl
};
#endif /* RTE_SPI5 */


#if (RTE_SPI6)
/* SPI6 Control Information */
static SPI_INFO SPI6_Ctrl = { 0 };

#if defined(SPI6_DMA_EN)
static       void    SPI6_TX_DMA_Callback (int32_t);
static       void    SPI6_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI6_Tx_DMA = { DMAREQ_SPI6_TX, &SPI6_TX_DMA_Callback };
static const SPI_DMA SPI6_Rx_DMA = { DMAREQ_SPI6_RX, &SPI6_RX_DMA_Callback };
#endif

#if defined(SPI6_SSEL_PORT)
static SPI_SSEL spi6_ssel = { SPI6_SSEL_PORT,
                              SPI6_SSEL_PIN,
                              SPI6_SSEL_FUNC,
                              SPI6_SSEL_NUM };
#endif

/* SPI6 Resources */
static SPI_RESOURCES SPI6_Resources = {
  LPC_SPI6,
  SPI6_IRQn,
  SPI6_SYSCON_CLOCK,
  { SPI6_MOSI_PORT, SPI6_MOSI_PIN, SPI6_MOSI_FUNC },
  { SPI6_MISO_PORT, SPI6_MISO_PIN, SPI6_MISO_FUNC },
  { SPI6_SCK_PORT,  SPI6_SCK_PIN,  SPI6_SCK_FUNC  },
  #if defined(SPI6_SSEL_PORT)
  &spi6_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI6_DMA_EN)
  &SPI6_Tx_DMA,
  &SPI6_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI6_Ctrl
};
#endif /* RTE_SPI6 */


#if (RTE_SPI7)
/* SPI7 Control Information */
static SPI_INFO SPI7_Ctrl = { 0 };

#if defined(SPI7_DMA_EN)
static       void    SPI7_TX_DMA_Callback (int32_t);
static       void    SPI7_RX_DMA_Callback (int32_t);
static const SPI_DMA SPI7_Tx_DMA = { DMAREQ_SPI7_TX, &SPI7_TX_DMA_Callback };
static const SPI_DMA SPI7_Rx_DMA = { DMAREQ_SPI7_RX, &SPI7_RX_DMA_Callback };
#endif

#if defined(SPI7_SSEL_PORT)
static SPI_SSEL spi7_ssel = { SPI7_SSEL_PORT,
                              SPI7_SSEL_PIN,
                              SPI7_SSEL_FUNC,
                              SPI7_SSEL_NUM };
#endif

/* SPI7 Resources */
static SPI_RESOURCES SPI7_Resources = {
  LPC_SPI7,
  SPI7_IRQn,
  SPI7_SYSCON_CLOCK,
  { SPI7_MOSI_PORT, SPI7_MOSI_PIN, SPI7_MOSI_FUNC },
  { SPI7_MISO_PORT, SPI7_MISO_PIN, SPI7_MISO_FUNC },
  { SPI7_SCK_PORT,  SPI7_SCK_PIN,  SPI7_SCK_FUNC  },
  #if defined(SPI7_SSEL_PORT)
  &spi7_ssel,
  #else
  NULL,
  #endif
  #if defined (SPI7_DMA_EN)
  &SPI7_Tx_DMA,
  &SPI7_Rx_DMA,
  #else
  NULL,
  NULL,
  #endif
  &SPI7_Ctrl
};
#endif /* RTE_SPI7 */


/**
  \fn          ARM_DRIVER_VERSION SPI_GetVersion (void)
  \brief       Get SPI driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRIVER_VERSION SPI_GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_SPI_CAPABILITIES SPI_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_SPI_CAPABILITIES
*/
static ARM_SPI_CAPABILITIES SPI_GetCapabilities (void) {
  return DriverCapabilities;
}


/**
  \fn          int32_t SPI_Initialize (ARM_SPI_SignalEvent_t cb_event, const SPI_RESOURCES *spi)
  \brief       Initialize SPI Interface.
  \param[in]   cb_event  Pointer to \ref ARM_SPI_SignalEvent
  \param[in]   spi       Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Initialize (ARM_SPI_SignalEvent_t cb_event, const SPI_RESOURCES *spi) {
#if defined(SPI_DMA_ENABLE)
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if (spi->info->flags & SPI_INIT)  { return ARM_DRIVER_OK; }

  /* Enable power */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO0);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO1);

  /* Configure SPI pins */
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->mosi.port, spi->mosi.pin, spi->mosi.func | SPI_PIN_IOCON);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->miso.port, spi->miso.pin, spi->miso.func | SPI_PIN_IOCON);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->sck.port,  spi->sck.pin,  spi->sck.func  | SPI_PIN_IOCON);

#if defined(SPI_DMA_ENABLE)
  if ((spi->dma_rx != NULL) && (spi->dma_tx != NULL)) {
    if ((LPC_SYSCON->AHBCLKCTRL[0] & SYSCON_CLOCK_DMA) == 0) {
      /* DMA clock is disabled, init DMA service */
      Chip_DMASERVICE_Init (Chip_DMA_Table);
    }

    /* Enable DMA IRQ in NVIC */
    NVIC_EnableIRQ(DMA_IRQn);

    /* Configure transmit channel */
    Chip_DMA_EnableChannel     (LPC_DMA, spi->dma_tx->ch);
    Chip_DMA_EnableIntChannel  (LPC_DMA, spi->dma_tx->ch);
    Chip_DMA_SetupChannelConfig(LPC_DMA, spi->dma_tx->ch, DMA_CFG_PERIPHREQEN);

    dma_context.channel = spi->dma_tx->ch;
    Chip_DMASERVICE_RegisterCb (&dma_context, spi->dma_tx->callback);

    /* Configure receive channel */
    Chip_DMA_EnableChannel     (LPC_DMA, spi->dma_rx->ch);
    Chip_DMA_EnableIntChannel  (LPC_DMA, spi->dma_rx->ch);
    Chip_DMA_SetupChannelConfig(LPC_DMA, spi->dma_rx->ch, DMA_CFG_PERIPHREQEN);

    dma_context.channel = spi->dma_rx->ch;
    Chip_DMASERVICE_RegisterCb (&dma_context, spi->dma_rx->callback);
  }
#endif

  /* Reset Run-Time information structure */
  memset (spi->info, 0x00, sizeof (SPI_INFO));

  spi->info->cb_event = cb_event;
  spi->info->flags    = SPI_INIT;

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t SPI_Uninitialize (const SPI_RESOURCES *spi)
  \brief       De-initialize SPI Interface.
  \param[in]   spi  Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Uninitialize (const SPI_RESOURCES *spi) {

  spi->info->flags = 0U;
  
  /* Reset SPI pins */
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->mosi.port, spi->mosi.pin, SPI_PIN_IOCON_RESET);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->miso.port, spi->miso.pin, SPI_PIN_IOCON_RESET);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->sck.port,  spi->sck.pin,  SPI_PIN_IOCON_RESET);

  if (spi->ssel) {
    Chip_IOCON_PinMuxSet (LPC_IOCON, spi->ssel->port, spi->ssel->pin, SPI_PIN_IOCON_RESET);
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t SPI_PowerControl (ARM_POWER_STATE state,
                                         const SPI_RESOURCES *spi)
  \brief       Control SPI Interface Power.
  \param[in]   state  Power state
  \param[in]   spi    Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_PowerControl (ARM_POWER_STATE state, const SPI_RESOURCES *spi) {

  switch (state) {
    case ARM_POWER_OFF:
      /* Power off SPI peripheral */
      NVIC_DisableIRQ (spi->irqn);

      /* Init peripheral (enable clock + reset) */
      Chip_SPI_Init (spi->reg);

      /* Deinit peripheral */
      Chip_SPI_DeInit (spi->reg);

      /* Reset status */
      spi->info->status.busy       = 0U;
      spi->info->status.data_lost  = 0U;

      spi->info->flags  &= ~SPI_POWER;
      break;

    case ARM_POWER_FULL:
      if ((spi->info->flags & SPI_INIT)  == 0U) { return ARM_DRIVER_ERROR; }
      if ((spi->info->flags & SPI_POWER) != 0U) { return ARM_DRIVER_OK; }

      /* Init peripheral (enable clock + reset) */
      Chip_SPI_Init (spi->reg);

      /* Set PLL as SPI clock source */
      Chip_Clock_SetFLEXCOMMClockSource(Chip_FLEXCOMM_GetIndex (spi->reg), SYSCON_FLEXCOMMCLKSELSRC_PLL);

      /* Reset status */
      spi->info->status.busy       = 0U;
      spi->info->status.data_lost  = 0U;

      if ((spi->dma_rx != NULL) && (spi->dma_tx != NULL)) {
        spi->info->flags |= SPI_XFER_DMA;
      }

      /* Enable FIFO Tx and Rx level trigger */
      spi->reg->FIFOTRIG |= SPI_FIFOTRIG_TXLVLENA | SPI_FIFOTRIG_RXLVLENA;

      /* Enable SPI interrupts */
      Chip_SPI_EnableFIFOInts (spi->reg, SPI_FIFOINT_TXERR | SPI_FIFOINT_RXERR);

      if (spi->info->flags & SPI_XFER_DMA) {
        /* Enable DMA for transmit function */
        Chip_SPI_SetFIFOCfg(spi->reg, SPI_FIFOCFG_DMATX | SPI_FIFOCFG_DMARX);
      } else {
        /* Interrupt data transfer */
        Chip_SPI_EnableFIFOInts (spi->reg, SPI_FIFOINT_RXLVL);
      }

      /* Enable peripheral interrupts in NVIC */
      NVIC_ClearPendingIRQ (spi->irqn);
      NVIC_EnableIRQ       (spi->irqn);

      spi->info->flags |= SPI_POWER;
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SPI_Send (const void *data, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start sending data to SPI transmitter.
  \param[in]   data  Pointer to buffer with data to send to SPI transmitter
  \param[in]   num   Number of data items to send
  \param[in]   spi   Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Send (const void *data, uint32_t num, const SPI_RESOURCES *spi) {
#if defined(SPI_DMA_ENABLE)
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if ((data == NULL) || (num == 0U))        { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((spi->info->flags & SPI_SETUP) == 0U) { return ARM_DRIVER_ERROR; }
  if ( spi->info->status.busy)              { return ARM_DRIVER_ERROR_BUSY; }

  spi->info->status.busy      = 1U;
  spi->info->status.data_lost = 0U;

  spi->info->xfer.rx_buf = NULL;
  spi->info->xfer.tx_buf = (uint8_t *)data;
  spi->info->xfer.num    = num;
  spi->info->xfer.rx_cnt = 0U;
  spi->info->xfer.tx_cnt = 0U;

#if defined(SPI_DMA_ENABLE)
  if (spi->info->flags & SPI_XFER_DMA) {
    if (num > 1024U) { num = 1024U; }

    spi->info->xfer.rx_pend = num;
    spi->info->xfer.tx_pend = num;
    spi->info->xfer.ctrl    = (DMA_XFERCFG_DSTINC_0 | DMA_XFERCFG_SRCINC_0) << 16 |
                              (DMA_XFERCFG_DSTINC_0 | DMA_XFERCFG_SRCINC_1);

    spi->info->xfer.rx_buf  = (uint8_t *)&spi->info->xfer.dummy;

    if (spi->info->flags & SPI_16B_FRAME) {
      dma_context.width     = DMA_XFERCFG_WIDTH_16;
      num *= 2;
    } else {
      dma_context.width     = DMA_XFERCFG_WIDTH_8;
    }

    /* Configure receive channel */
    dma_context.channel           = spi->dma_rx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFORD);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = false;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.rx_buf, num);

    /* Configure transmit channel */
    dma_context.channel           = spi->dma_tx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFOWR);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_1;
    dma_context.write             = true;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.tx_buf, num);
  } else
#endif
  {
    /* Enable TX FIFO level interrupt */
    Chip_SPI_EnableFIFOInts (spi->reg, SPI_FIFOINT_TXLVL);
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t SPI_Receive (void *data, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start receiving data from SPI receiver.
  \param[out]  data  Pointer to buffer for data to receive from SPI receiver
  \param[in]   num   Number of data items to receive
  \param[in]   spi   Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Receive (void *data, uint32_t num, const SPI_RESOURCES *spi) {
#if defined(SPI_DMA_ENABLE)
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if ((data == NULL) || (num == 0U))        { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((spi->info->flags & SPI_SETUP) == 0U) { return ARM_DRIVER_ERROR; }
  if ( spi->info->status.busy)              { return ARM_DRIVER_ERROR_BUSY; }

  spi->info->status.busy       = 1U;
  spi->info->status.data_lost  = 0U;

  spi->info->xfer.rx_buf = (uint8_t *)data;
  spi->info->xfer.tx_buf = NULL;
  spi->info->xfer.num    = num;
  spi->info->xfer.rx_cnt = 0U;
  spi->info->xfer.tx_cnt = 0U;

#if defined(SPI_DMA_ENABLE)
  if (spi->info->flags & SPI_XFER_DMA) {
    if (num > 1024U) { num = 1024U; }

    spi->info->xfer.rx_pend = num;
    spi->info->xfer.tx_pend = num;
    spi->info->xfer.ctrl    = (DMA_XFERCFG_DSTINC_1 | DMA_XFERCFG_SRCINC_0) << 16 |
                              (DMA_XFERCFG_DSTINC_0 | DMA_XFERCFG_SRCINC_0);

    spi->info->xfer.tx_buf  = (uint8_t *)&spi->info->xfer.def_val;

    if (spi->info->flags & SPI_16B_FRAME) {
      dma_context.width     = DMA_XFERCFG_WIDTH_16;
      num *= 2;
    } else {
      dma_context.width     = DMA_XFERCFG_WIDTH_8;
    }

    /* Configure receive channel */
    dma_context.channel           = spi->dma_rx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFORD);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_1;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = false;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.rx_buf, num);

    /* Configure transmit channel */
    dma_context.channel           = spi->dma_tx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFOWR);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = true;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.tx_buf, num);
  } else
#endif
  {
    /* Enable TX FIFO level interrupt */
    Chip_SPI_EnableFIFOInts (spi->reg, SPI_FIFOINT_TXLVL);
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          int32_t SPI_Transfer (const void *data_out, void *data_in, uint32_t num, const SPI_RESOURCES *spi)
  \brief       Start sending/receiving data to/from SPI transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to SPI transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from SPI receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   spi       Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Transfer (const void *data_out, void *data_in, uint32_t num, const SPI_RESOURCES *spi) {
#if defined(SPI_DMA_ENABLE)
  DMA_PERIPHERAL_CONTEXT_T dma_context;
#endif

  if ((data_out == NULL) || (data_in == NULL) || (num == 0U)) { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((spi->info->flags & SPI_SETUP) == 0U)                   { return ARM_DRIVER_ERROR; }
  if ( spi->info->status.busy)                                { return ARM_DRIVER_ERROR_BUSY; }

  spi->info->status.busy       = 1U;
  spi->info->status.data_lost  = 0U;

  spi->info->xfer.rx_buf = (uint8_t *)data_in;
  spi->info->xfer.tx_buf = (uint8_t *)data_out;

  spi->info->xfer.num    = num;
  spi->info->xfer.rx_cnt = 0U;
  spi->info->xfer.tx_cnt = 0U;

#if defined(SPI_DMA_ENABLE)
  if (spi->info->flags & SPI_XFER_DMA) {
    if (num > 1024U) { num = 1024U; }

    spi->info->xfer.rx_pend = num;
    spi->info->xfer.tx_pend = num;
    spi->info->xfer.ctrl    = (DMA_XFERCFG_DSTINC_1 | DMA_XFERCFG_SRCINC_0) << 16 |
                              (DMA_XFERCFG_DSTINC_0 | DMA_XFERCFG_SRCINC_1);

    if (spi->info->flags & SPI_16B_FRAME) {
      dma_context.width     = DMA_XFERCFG_WIDTH_16;
      num *= 2;
    } else {
      dma_context.width     = DMA_XFERCFG_WIDTH_8;
    }

    /* Configure receive channel */
    dma_context.channel           = spi->dma_rx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFORD);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_1;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_0;
    dma_context.write             = false;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.rx_buf, num);

    /* Configure transmit channel */
    dma_context.channel           = spi->dma_tx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFOWR);
    dma_context.dst_increment     = DMA_XFERCFG_DSTINC_0;
    dma_context.src_increment     = DMA_XFERCFG_SRCINC_1;
    dma_context.write             = true;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)spi->info->xfer.tx_buf, num);
  } else
#endif
  {
    /* Enable TX FIFO level interrupt */
    Chip_SPI_EnableFIFOInts (spi->reg, SPI_FIFOINT_TXLVL);
  }

  return ARM_DRIVER_OK;
}


/**
  \fn          uint32_t SPI_GetDataCount (const SPI_RESOURCES *spi)
  \brief       Get transferred data count.
  \param[in]   spi  Pointer to SPI resources
  \return      number of data items transferred
*/
static uint32_t SPI_GetDataCount (const SPI_RESOURCES *spi) {
  return (spi->info->xfer.rx_cnt);
}


/**
  \fn          int32_t SPI_Control (uint32_t control, uint32_t arg, const SPI_RESOURCES *spi)
  \brief       Control SPI Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   spi      Pointer to SPI resources
  \return      \ref execution_status
*/
static int32_t SPI_Control (uint32_t control, uint32_t arg, const SPI_RESOURCES *spi) {
  uint32_t bits;

  if ((spi->info->flags & SPI_POWER) == 0U) { return ARM_DRIVER_ERROR; }

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_ABORT_TRANSFER) {
    /* Abort data transfer */
    Chip_SPI_ClearCFGRegBits (spi->reg, SPI_CFG_SPI_EN);
    Chip_SPI_SetCFGRegBits   (spi->reg, SPI_CFG_SPI_EN);

    spi->info->xfer.num    = 0U;
    spi->info->xfer.rx_cnt = 0U;
    spi->info->xfer.tx_cnt = 0U;
    spi->info->status.busy = 0U;

    return ARM_DRIVER_OK;
  }

  if (spi->info->status.busy) { return ARM_DRIVER_ERROR_BUSY; }

  switch (control & ARM_SPI_CONTROL_Msk) {
    case ARM_SPI_MODE_INACTIVE:             // SPI Inactive
      Chip_SPI_Disable (spi->reg);
      /* Clear configured flag */
      spi->info->flags &= ~SPI_SETUP;
      return ARM_DRIVER_OK;

    case ARM_SPI_MODE_MASTER:               // SPI Master (Output on MOSI, Input on MISO); arg = Bus Speed in bps
    case ARM_SPI_MODE_SLAVE:                // SPI Slave  (Output on MISO, Input on MOSI)
      break;

    case ARM_SPI_MODE_MASTER_SIMPLEX:       // SPI Master (Output/Input on MOSI); arg = Bus Speed in bps
    case ARM_SPI_MODE_SLAVE_SIMPLEX:        // SPI Slave  (Output/Input on MISO)
      return ARM_SPI_ERROR_MODE;

    case ARM_SPI_SET_BUS_SPEED:             // Set Bus Speed in bps; arg = value
      Chip_SPIM_SetClockRate (spi->reg, arg);
      return ARM_DRIVER_OK;

    case ARM_SPI_GET_BUS_SPEED:             // Get Bus Speed in bps
      return (Chip_SPIM_GetClockRate(spi->reg));

    case ARM_SPI_SET_DEFAULT_TX_VALUE:      // Set default Transmit value; arg = value
      spi->info->xfer.def_val = arg;
      return ARM_DRIVER_OK;

    case ARM_SPI_CONTROL_SS:                // Control Slave Select; arg = 0:inactive, 1:active
      if (spi->info->flags & SPI_SS_SWCTRL) {
        if (arg == ARM_SPI_SS_INACTIVE) {
          Chip_GPIO_SetPinState (LPC_GPIO, spi->ssel->port, spi->ssel->pin, true);
        } else {
          Chip_GPIO_SetPinState (LPC_GPIO, spi->ssel->port, spi->ssel->pin, false);
        }
        return ARM_DRIVER_OK;
      }
      return ARM_DRIVER_ERROR;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  /* Configure mode */
  Chip_SPI_Disable (spi->reg);

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_MASTER) {
    /* Enable master mode */
    Chip_SPI_SetCFGRegBits (spi->reg, SPI_CFG_MASTER_EN);

    /* Set SCK frequency */
    Chip_SPIM_SetClockRate (spi->reg, arg);

    if (spi->info->flags & SPI_XFER_DMA) {
      SPI_FIFOCTRL(spi->reg) |= 0x0F;
      SPI_FIFOCTRL(spi->reg) &= ~(1 << spi->ssel->num);
    } else {
      spi->info->xfer.ctrl = (0x000F & ~(1 << spi->ssel->num)) << 16;
    }
  } else {
    /* Enable slave mode */
    Chip_SPI_ClearCFGRegBits(spi->reg, SPI_CFG_MASTER_EN);
  }

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_MASTER) {
    switch (control & ARM_SPI_SS_MASTER_MODE_Msk) {
      case ARM_SPI_SS_MASTER_UNUSED:        // SPI Slave Select when Master: Not used (default)
        if (spi->ssel != NULL) {
          Chip_IOCON_PinMuxSet (LPC_IOCON, spi->ssel->port, spi->ssel->pin, SPI_PIN_IOCON_RESET);
        }
        spi->info->flags &= ~SPI_SS_SWCTRL;
        break;

      case ARM_SPI_SS_MASTER_HW_INPUT:      // SPI Slave Select when Master: Hardware monitored Input
        return ARM_SPI_ERROR_SS_MODE;

      case ARM_SPI_SS_MASTER_SW:            // SPI Slave Select when Master: Software controlled
        if (spi->ssel == NULL) {
          return ARM_SPI_ERROR_SS_MODE;
        }
        spi->info->flags |= SPI_SS_SWCTRL;

        Chip_IOCON_PinMuxSet (LPC_IOCON, spi->ssel->port, spi->ssel->pin, SPI_PIN_IOCON);
        /* Configure SSEL pin as high output */
        Chip_GPIO_SetPinState    (LPC_GPIO, spi->ssel->port, spi->ssel->pin, true);
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, spi->ssel->port, spi->ssel->pin);
        break;

      case ARM_SPI_SS_MASTER_HW_OUTPUT:     // SPI Slave Select when Master: Hardware controlled Output
        if (spi->ssel == NULL) {
          return ARM_SPI_ERROR_SS_MODE;
        }
        spi->info->flags &= ~SPI_SS_SWCTRL;

        Chip_IOCON_PinMuxSet (LPC_IOCON, spi->ssel->port, spi->ssel->pin, spi->ssel->func | SPI_PIN_IOCON);
        break;

      default:
        return ARM_SPI_ERROR_SS_MODE;
    }
  }

  if ((control & ARM_SPI_CONTROL_Msk) == ARM_SPI_MODE_SLAVE) {
    switch (control & ARM_SPI_SS_SLAVE_MODE_Msk) {
      case ARM_SPI_SS_SLAVE_HW:             // SPI Slave Select when Slave: Hardware monitored (default)
        if (spi->ssel == NULL) {
          return ARM_SPI_ERROR_SS_MODE;
        }
        Chip_IOCON_PinMuxSet (LPC_IOCON, spi->ssel->port, spi->ssel->pin, spi->ssel->func | SPI_PIN_IOCON);
        break;

      case ARM_SPI_SS_SLAVE_SW:             // SPI Slave Select when Slave: Software controlled
      default:
        return ARM_SPI_ERROR_SS_MODE;
    }
  }

  /* Configure Frame Format */
  switch (control & ARM_SPI_FRAME_FORMAT_Msk) {
    case ARM_SPI_CPOL0_CPHA0:
      Chip_SPI_SetSPIMode (spi->reg, SPI_CLOCK_CPHA0_CPOL0);
      break;

    case ARM_SPI_CPOL0_CPHA1:
      Chip_SPI_SetSPIMode (spi->reg, SPI_CLOCK_CPHA1_CPOL0);
      break;

    case ARM_SPI_CPOL1_CPHA0:
      Chip_SPI_SetSPIMode (spi->reg, SPI_CLOCK_CPHA0_CPOL1);
      break;

    case ARM_SPI_CPOL1_CPHA1:
      Chip_SPI_SetSPIMode (spi->reg, SPI_CLOCK_CPHA1_CPOL1);
      break;

    case ARM_SPI_TI_SSI:
    case ARM_SPI_MICROWIRE:
    default:
      return ARM_SPI_ERROR_FRAME_FORMAT;
  }

  /* Configure Number of Data Bits */
  bits = ((control & ARM_SPI_DATA_BITS_Msk) >> ARM_SPI_DATA_BITS_Pos);
  
  if (bits > 16U) {
    return ARM_SPI_ERROR_DATA_BITS;
  }
  else {
    if (bits > 8U) {
      spi->info->flags |= SPI_16B_FRAME;
    } else {
      spi->info->flags &= ~SPI_16B_FRAME;
    }
  }

  if (spi->info->flags & SPI_XFER_DMA) {
    SPI_FIFOCTRL(spi->reg) = (SPI_FIFOCTRL(spi->reg) & ~0x0F00UL) | ((bits - 1) << 8);
  } else {
    spi->info->xfer.ctrl &= ~(0x0FUL << 24);
    spi->info->xfer.ctrl |= ((bits - 1) << 24);
  }

  /* Configure Bit Order */
  if ((control & ARM_SPI_BIT_ORDER_Msk) == ARM_SPI_LSB_MSB) {
    Chip_SPI_EnableLSBFirst (spi->reg);
  } else {
    Chip_SPI_EnableMSBFirst (spi->reg);
  }

  Chip_SPI_Enable (spi->reg);

  spi->info->flags |= SPI_SETUP;

  return ARM_DRIVER_OK;

}


/**
  \fn          ARM_SPI_STATUS SPI_GetStatus (const SPI_RESOURCES *spi)
  \brief       Get SPI status.
  \param[in]   spi  Pointer to SPI resources
  \return      SPI status \ref ARM_SPI_STATUS
*/
static ARM_SPI_STATUS SPI_GetStatus (const SPI_RESOURCES *spi) {
  ARM_SPI_STATUS status;

  status.busy       = spi->info->status.busy;
  status.data_lost  = spi->info->status.data_lost;
  status.mode_fault = 0U;

  return (status);
}


/**
  \fn          void SPI_IRQHandler (SPI_RESOURCES *spi)
  \brief       SPI Interrupt handler.
  \param[in]   spi  Pointer to SPI resources
*/
void SPI_IRQHandler (const SPI_RESOURCES *spi) {
  SPI_TRANSFER_INFO *tr = &spi->info->xfer;
  uint32_t event;
  uint32_t mask;
  uint32_t stat;
  uint32_t data;

  event = 0U;
  mask  = 0U;
  stat  = Chip_SPI_GetFIFOStatus (spi->reg);

  if (stat & (SPI_FIFOSTAT_RXERR | SPI_FIFOSTAT_TXERR)) {
    if (stat & SPI_FIFOSTAT_RXERR) {
      /* Receiver overrun */
      Chip_SPI_ClearFIFOStatus (spi->reg, SPI_FIFOSTAT_RXERR);
    } else {
      /* Transmitter underfun */
      Chip_SPI_ClearFIFOStatus (spi->reg, SPI_FIFOSTAT_TXERR);
    }

    spi->info->status.data_lost = 1U;
    event |= ARM_SPI_EVENT_DATA_LOST;
  }

  if (tr->num > 0U) {
    data = Chip_SPI_GetFIFOEnabledInts(spi->reg);

    if (data & SPI_FIFOINT_TXLVL) {
      /* Tx interrupts are enabled */
      mask |= SPI_FIFOSTAT_TXNOTFULL;
    }
    if (data & SPI_FIFOINT_RXLVL) {
      /* Rx interrupts are enabled */
      mask |= SPI_FIFOSTAT_RXNOTEMPTY;
    }
  }

  if (stat & mask) {
    /* Transfer data is set */
    do {
      if (mask & SPI_FIFOSTAT_RXNOTEMPTY) {
        /* Rx interrupts are enabled */
        if (stat & SPI_FIFOSTAT_RXNOTEMPTY) {
          data = Chip_SPI_ReadRawRXFifo (spi->reg);

          if (tr->rx_cnt < tr->num) {
            if (tr->rx_buf) {
              *(tr->rx_buf++) = (uint8_t)data;

              if (spi->info->flags & SPI_16B_FRAME) {
                *(tr->rx_buf++) = (uint8_t)(data >> 8);
              }
            }
            tr->rx_cnt++;
          }
          else {
            /* All data received */
            mask &= ~SPI_FIFOSTAT_RXNOTEMPTY;
          }
        }
      }
      if (mask & SPI_FIFOSTAT_TXNOTFULL) {
        /* Tx interrupts are enabled */
        if (stat & SPI_FIFOSTAT_TXNOTFULL) {
          if (tr->tx_cnt < tr->num) {
            if (tr->tx_buf) {
              data = *tr->tx_buf++;

              if (spi->info->flags & SPI_16B_FRAME) {
                data |= *tr->tx_buf++ << 8;
              }
            }
            else {
              data = tr->def_val;
            }
            Chip_SPI_WriteFIFO (spi->reg, data | tr->ctrl);
            tr->tx_cnt++;
          }
          else {
            /* All data sent, disable TXRDY interrupt */
            Chip_SPI_DisableFIFOInts (spi->reg, SPI_FIFOINT_TXLVL);

            mask &= ~SPI_FIFOSTAT_TXNOTFULL;
          }
        }
      }

      stat = Chip_SPI_GetFIFOStatus(spi->reg);
    }
    while (stat & mask);

    if (tr->rx_cnt == tr->num) {
      /* Transfer completed */
      if (spi->reg->CFG & SPI_CFG_MASTER_EN) {
        spi->reg->STAT = SPI_STAT_EOT;
      }

      event |= ARM_SPI_EVENT_TRANSFER_COMPLETE;

      tr->num = 0U;
      spi->info->status.busy = 0U;
    }
  }

  /* Send event */
  if ((event != 0U) && (spi->info->cb_event != NULL)) {
    spi->info->cb_event (event);
  }
}


#if defined(SPI_DMA_ENABLE)
/**
  \fn          void SPI_TX_DMA_IRQHandler (const SPI_RESOURCES *spi)
  \brief       SPI TX DMA interrupt handler
  \param[in]   spi  Pointer to SPI resources
*/
void SPI_TX_DMA_IRQHandler (const SPI_RESOURCES *spi) {
  SPI_TRANSFER_INFO *tr = &spi->info->xfer;
  DMA_PERIPHERAL_CONTEXT_T  dma_context;
  uint32_t num;

  tr->tx_cnt += tr->tx_pend;

  if (tr->tx_cnt < tr->num) {
    /* Determine number of data to transmit */
    num = tr->num - tr->tx_cnt;

    if (num > 1024U) {
      num = 1024U;
    }

    tr->tx_pend = num;

    /* Increment buffer pointer and set transfer width */
    if (spi->info->flags & SPI_16B_FRAME) {
      tr->tx_buf += tr->tx_pend * 2;
      dma_context.width = DMA_XFERCFG_WIDTH_16;
      num *= 2;
    } else {
      tr->tx_buf += tr->tx_pend;
      dma_context.width = DMA_XFERCFG_WIDTH_8;
    }

    /* Configure transmit channel */
    dma_context.channel           = spi->dma_tx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFOWR);
    dma_context.dst_increment     = tr->ctrl & (3UL << 14);
    dma_context.src_increment     = tr->ctrl & (3UL << 12);
    dma_context.write             = true;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)tr->tx_buf, num);
  }
}

/**
  \fn          void SPI_R;X_DMA_IRQHandler (const SPI_RESOURCES *spi)
  \brief       SPI RX DMA interrupt handler
  \param[in]   spi  Pointer to SPI resources
*/
void SPI_RX_DMA_IRQHandler (const SPI_RESOURCES *spi) {
  SPI_TRANSFER_INFO *tr = &spi->info->xfer;
  DMA_PERIPHERAL_CONTEXT_T  dma_context;
  uint32_t num;

  tr->rx_cnt += tr->rx_pend;

  if (tr->rx_cnt < tr->num) {
    /* Determine number of data to receive */
    num = tr->num - tr->rx_cnt;

    if (num > 1024U) {
      num = 1024U;
    }

    tr->rx_pend = num;

    /* Increment buffer pointer and set transfer width */
    if (spi->info->flags & SPI_16B_FRAME) {
      tr->rx_buf += tr->rx_pend * 2;
      dma_context.width = DMA_XFERCFG_WIDTH_16;
      num *= 2;
    } else {
      tr->rx_buf += tr->rx_pend;
      dma_context.width = DMA_XFERCFG_WIDTH_8;
    }

    /* Configure receive channel */
    dma_context.channel           = spi->dma_rx->ch;
    dma_context.register_location = (volatile uint32_t *)&(spi->reg->FIFORD);
    dma_context.dst_increment     = (tr->ctrl >> 16) & (3UL << 14);
    dma_context.src_increment     = (tr->ctrl >> 16) & (3UL << 12);
    dma_context.write             = false;

    Chip_DMASERVICE_SingleBuffer (&dma_context, (uint32_t)tr->rx_buf, num);
  }
  else {
    if (spi->reg->CFG & SPI_CFG_MASTER_EN) {
      spi->reg->STAT = SPI_STAT_EOT;
    }

    /* All data received, transfer completed */
    tr->num = 0U;
    spi->info->status.busy = 0U;

    /* Send event */
    if (spi->info->cb_event != NULL) {
      spi->info->cb_event (ARM_SPI_EVENT_TRANSFER_COMPLETE);
    }
  }
}
#endif


#if defined(RTE_SPI0) && (RTE_SPI0 != 0)
/* SPI0 Wrapper Functions */
static int32_t        SPI0_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI0_Resources); }
static int32_t        SPI0_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI0_Resources); }
static int32_t        SPI0_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI0_Resources); }
static int32_t        SPI0_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI0_Resources); }
static int32_t        SPI0_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI0_Resources); }
static int32_t        SPI0_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI0_Resources); }
static uint32_t       SPI0_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI0_Resources); }
static int32_t        SPI0_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI0_Resources); }
static ARM_SPI_STATUS SPI0_GetStatus           (void)                                              { return SPI_GetStatus (&SPI0_Resources); }
       void           SPI0_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI0_Resources); }

#if defined(SPI0_DMA_EN)
void  SPI0_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI0_Resources);
}
void  SPI0_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI0_Resources);
}
#endif

/* Driver_SPI0 */
ARM_DRIVER_SPI Driver_SPI0 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI0_Initialize,
  SPI0_Uninitialize,
  SPI0_PowerControl,
  SPI0_Send,
  SPI0_Receive,
  SPI0_Transfer,
  SPI0_GetDataCount,
  SPI0_Control,
  SPI0_GetStatus
};
#endif

#if defined(RTE_SPI1) && (RTE_SPI1 != 0)
/* SPI1 Wrapper Functions */
static int32_t        SPI1_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI1_Resources); }
static int32_t        SPI1_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI1_Resources); }
static int32_t        SPI1_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI1_Resources); }
static int32_t        SPI1_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI1_Resources); }
static int32_t        SPI1_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI1_Resources); }
static int32_t        SPI1_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI1_Resources); }
static uint32_t       SPI1_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI1_Resources); }
static int32_t        SPI1_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI1_Resources); }
static ARM_SPI_STATUS SPI1_GetStatus           (void)                                              { return SPI_GetStatus (&SPI1_Resources); }
       void           SPI1_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI1_Resources); }

#if defined(SPI1_DMA_EN)
void  SPI1_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI1_Resources);
}
void  SPI1_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI1_Resources);
}
#endif

/* Driver_SPI1 */
ARM_DRIVER_SPI Driver_SPI1 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI1_Initialize,
  SPI1_Uninitialize,
  SPI1_PowerControl,
  SPI1_Send,
  SPI1_Receive,
  SPI1_Transfer,
  SPI1_GetDataCount,
  SPI1_Control,
  SPI1_GetStatus
};
#endif

#if defined(RTE_SPI2) && (RTE_SPI2 != 0)
/* SPI2 Wrapper Functions */
static int32_t        SPI2_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI2_Resources); }
static int32_t        SPI2_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI2_Resources); }
static int32_t        SPI2_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI2_Resources); }
static int32_t        SPI2_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI2_Resources); }
static int32_t        SPI2_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI2_Resources); }
static int32_t        SPI2_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI2_Resources); }
static uint32_t       SPI2_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI2_Resources); }
static int32_t        SPI2_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI2_Resources); }
static ARM_SPI_STATUS SPI2_GetStatus           (void)                                              { return SPI_GetStatus (&SPI2_Resources); }
       void           SPI2_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI2_Resources); }

#if defined(SPI2_DMA_EN)
void  SPI2_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI2_Resources);
}
void  SPI2_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI2_Resources);
}
#endif

/* Driver_SPI2 */
ARM_DRIVER_SPI Driver_SPI2 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI2_Initialize,
  SPI2_Uninitialize,
  SPI2_PowerControl,
  SPI2_Send,
  SPI2_Receive,
  SPI2_Transfer,
  SPI2_GetDataCount,
  SPI2_Control,
  SPI2_GetStatus
};
#endif

#if defined(RTE_SPI3) && (RTE_SPI3 != 0)
/* SPI3 Wrapper Functions */
static int32_t        SPI3_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI3_Resources); }
static int32_t        SPI3_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI3_Resources); }
static int32_t        SPI3_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI3_Resources); }
static int32_t        SPI3_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI3_Resources); }
static int32_t        SPI3_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI3_Resources); }
static int32_t        SPI3_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI3_Resources); }
static uint32_t       SPI3_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI3_Resources); }
static int32_t        SPI3_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI3_Resources); }
static ARM_SPI_STATUS SPI3_GetStatus           (void)                                              { return SPI_GetStatus (&SPI3_Resources); }
       void           SPI3_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI3_Resources); }

#if defined(SPI3_DMA_EN)
void  SPI3_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI3_Resources);
}
void  SPI3_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI3_Resources);
}
#endif

/* Driver_SPI3 */
ARM_DRIVER_SPI Driver_SPI3 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI3_Initialize,
  SPI3_Uninitialize,
  SPI3_PowerControl,
  SPI3_Send,
  SPI3_Receive,
  SPI3_Transfer,
  SPI3_GetDataCount,
  SPI3_Control,
  SPI3_GetStatus
};
#endif

#if defined(RTE_SPI4) && (RTE_SPI4 != 0)
/* SPI4 Wrapper Functions */
static int32_t        SPI4_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI4_Resources); }
static int32_t        SPI4_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI4_Resources); }
static int32_t        SPI4_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI4_Resources); }
static int32_t        SPI4_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI4_Resources); }
static int32_t        SPI4_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI4_Resources); }
static int32_t        SPI4_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI4_Resources); }
static uint32_t       SPI4_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI4_Resources); }
static int32_t        SPI4_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI4_Resources); }
static ARM_SPI_STATUS SPI4_GetStatus           (void)                                              { return SPI_GetStatus (&SPI4_Resources); }
       void           SPI4_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI4_Resources); }

#if defined(SPI4_DMA_EN)
void  SPI4_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI4_Resources);
}
void  SPI4_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI4_Resources);
}
#endif

/* Driver_SPI4 */
ARM_DRIVER_SPI Driver_SPI4 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI4_Initialize,
  SPI4_Uninitialize,
  SPI4_PowerControl,
  SPI4_Send,
  SPI4_Receive,
  SPI4_Transfer,
  SPI4_GetDataCount,
  SPI4_Control,
  SPI4_GetStatus
};
#endif

#if defined(RTE_SPI5) && (RTE_SPI5 != 0)
/* SPI5 Wrapper Functions */
static int32_t        SPI5_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI5_Resources); }
static int32_t        SPI5_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI5_Resources); }
static int32_t        SPI5_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI5_Resources); }
static int32_t        SPI5_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI5_Resources); }
static int32_t        SPI5_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI5_Resources); }
static int32_t        SPI5_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI5_Resources); }
static uint32_t       SPI5_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI5_Resources); }
static int32_t        SPI5_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI5_Resources); }
static ARM_SPI_STATUS SPI5_GetStatus           (void)                                              { return SPI_GetStatus (&SPI5_Resources); }
       void           SPI5_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI5_Resources); }

#if defined(SPI5_DMA_EN)
void  SPI5_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI5_Resources);
}
void  SPI5_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI5_Resources);
}
#endif

/* Driver_SPI5 */
ARM_DRIVER_SPI Driver_SPI5 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI5_Initialize,
  SPI5_Uninitialize,
  SPI5_PowerControl,
  SPI5_Send,
  SPI5_Receive,
  SPI5_Transfer,
  SPI5_GetDataCount,
  SPI5_Control,
  SPI5_GetStatus
};
#endif

#if defined(RTE_SPI6) && (RTE_SPI6 != 0)
/* SPI6 Wrapper Functions */
static int32_t        SPI6_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI6_Resources); }
static int32_t        SPI6_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI6_Resources); }
static int32_t        SPI6_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI6_Resources); }
static int32_t        SPI6_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI6_Resources); }
static int32_t        SPI6_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI6_Resources); }
static int32_t        SPI6_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI6_Resources); }
static uint32_t       SPI6_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI6_Resources); }
static int32_t        SPI6_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI6_Resources); }
static ARM_SPI_STATUS SPI6_GetStatus           (void)                                              { return SPI_GetStatus (&SPI6_Resources); }
       void           SPI6_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI6_Resources); }

#if defined(SPI6_DMA_EN)
void  SPI6_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI6_Resources);
}
void  SPI6_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI6_Resources);
}
#endif

/* Driver_SPI6 */
ARM_DRIVER_SPI Driver_SPI6 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI6_Initialize,
  SPI6_Uninitialize,
  SPI6_PowerControl,
  SPI6_Send,
  SPI6_Receive,
  SPI6_Transfer,
  SPI6_GetDataCount,
  SPI6_Control,
  SPI6_GetStatus
};
#endif

#if defined(RTE_SPI7) && (RTE_SPI7 != 0)
/* SPI7 Wrapper Functions */
static int32_t        SPI7_Initialize          (ARM_SPI_SignalEvent_t pSignalEvent)                { return SPI_Initialize (pSignalEvent, &SPI7_Resources); }
static int32_t        SPI7_Uninitialize        (void)                                              { return SPI_Uninitialize (&SPI7_Resources); }
static int32_t        SPI7_PowerControl        (ARM_POWER_STATE state)                             { return SPI_PowerControl (state, &SPI7_Resources); }
static int32_t        SPI7_Send                (const void *data, uint32_t num)                    { return SPI_Send (data, num, &SPI7_Resources); }
static int32_t        SPI7_Receive             (void *data, uint32_t num)                          { return SPI_Receive (data, num, &SPI7_Resources); }
static int32_t        SPI7_Transfer            (const void *data_out, void *data_in, uint32_t num) { return SPI_Transfer (data_out, data_in, num, &SPI7_Resources); }
static uint32_t       SPI7_GetDataCount        (void)                                              { return SPI_GetDataCount (&SPI7_Resources); }
static int32_t        SPI7_Control             (uint32_t control, uint32_t arg)                    { return SPI_Control (control, arg, &SPI7_Resources); }
static ARM_SPI_STATUS SPI7_GetStatus           (void)                                              { return SPI_GetStatus (&SPI7_Resources); }
       void           SPI7_IRQHandler          (void)                                              {        SPI_IRQHandler (&SPI7_Resources); }

#if defined(SPI7_DMA_EN)
void  SPI7_TX_DMA_Callback (int32_t val) {
  SPI_TX_DMA_IRQHandler (&SPI7_Resources);
}
void  SPI7_RX_DMA_Callback (int32_t val) {
  SPI_RX_DMA_IRQHandler (&SPI7_Resources);
}
#endif

/* Driver_SPI7 */
ARM_DRIVER_SPI Driver_SPI7 = {
  SPI_GetVersion,
  SPI_GetCapabilities,
  SPI7_Initialize,
  SPI7_Uninitialize,
  SPI7_PowerControl,
  SPI7_Send,
  SPI7_Receive,
  SPI7_Transfer,
  SPI7_GetDataCount,
  SPI7_Control,
  SPI7_GetStatus
};
#endif
