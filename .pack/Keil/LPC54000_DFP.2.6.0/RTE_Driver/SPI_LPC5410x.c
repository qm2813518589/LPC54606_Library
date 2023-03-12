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
 * $Date:        16. February 2016
 * $Revision:    V1.0
 *
 * Driver:       Driver_SPI0, Driver_SPI1
 * Configured:   via RTE_Device.h configuration file
 * Project:      SPI Driver for NXP LPC5410x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   I2C Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_SPI# = 0       use SPI0
 *   Connect to hardware via Driver_SPI# = 1       use SPI1
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    - Initial release
 */

#include "SPI_LPC5410x.h"

#define ARM_SPI_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0) /* driver version */

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

#if RTE_SPI0_SSEL_PIN_EN
static SPI_SSEL spi0_ssel = { RTE_SPI0_SSEL_PORT,
                              RTE_SPI0_SSEL_PIN,
                              RTE_SPI0_SSEL_FUNC,
                              RTE_SPI0_SSEL_NUM };
#endif

/* SPI0 Resources */
static SPI_RESOURCES SPI0_Resources = {
  LPC_SPI0,
  SPI0_IRQn,
  SYSCON_CLOCK_SPI0,
  { RTE_SPI0_MOSI_PORT, RTE_SPI0_MOSI_PIN, RTE_SPI0_MOSI_FUNC },
  { RTE_SPI0_MISO_PORT, RTE_SPI0_MISO_PIN, RTE_SPI0_MISO_FUNC },
  { RTE_SPI0_SCK_PORT,  RTE_SPI0_SCK_PIN,  RTE_SPI0_SCK_FUNC },
  #if RTE_SPI0_SSEL_PIN_EN
  &spi0_ssel,
  #else
  NULL,
  #endif
  &SPI0_Ctrl
};
#endif /* RTE_SPI0 */


#if (RTE_SPI1)
/* SPI1 Control Information */
static SPI_INFO SPI1_Ctrl = { 0 };

#if RTE_SPI1_SSEL_PIN_EN
static SPI_SSEL spi1_ssel = { RTE_SPI1_SSEL_PORT,
                              RTE_SPI1_SSEL_PIN,
                              RTE_SPI1_SSEL_FUNC,
                              RTE_SPI1_SSEL_NUM };
#endif

/* SPI1 Resources */
static SPI_RESOURCES SPI1_Resources = {
  LPC_SPI1,
  SPI1_IRQn,
  SYSCON_CLOCK_SPI1,
  { RTE_SPI1_MOSI_PORT, RTE_SPI1_MOSI_PIN, RTE_SPI1_MOSI_FUNC },
  { RTE_SPI1_MISO_PORT, RTE_SPI1_MISO_PIN, RTE_SPI1_MISO_FUNC },
  { RTE_SPI1_SCK_PORT,  RTE_SPI1_SCK_PIN,  RTE_SPI1_SCK_FUNC },
  #if RTE_SPI1_SSEL_PIN_EN
  &spi1_ssel,
  #else
  NULL,
  #endif
  &SPI1_Ctrl
};
#endif /* RTE_SPI1 */


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

  if (spi->info->flags & SPI_INIT)  { return ARM_DRIVER_OK; }

  /* Enable power */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO0);

  /* Configure SPI pins */
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->mosi.port, spi->mosi.pin, spi->mosi.func | SPI_PIN_IOCON);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->miso.port, spi->miso.pin, spi->miso.func | SPI_PIN_IOCON);
  Chip_IOCON_PinMuxSet (LPC_IOCON, spi->sck.port,  spi->sck.pin,  spi->sck.func  | SPI_PIN_IOCON);

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

      /* Reset status */
      spi->info->status.busy       = 0U;
      spi->info->status.data_lost  = 0U;

      /* Enable peripheral interrupts in NVIC */
      NVIC_ClearPendingIRQ (spi->irqn);
      NVIC_EnableIRQ       (spi->irqn);

      /* Enable SPI interrupts */
      Chip_SPI_EnableInts (spi->reg, SPI_INTENSET_RXDYEN |
                                     SPI_INTENSET_RXOVEN |
                                     SPI_INTENSET_TXUREN);

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

  if ((data == NULL) || (num == 0U))        { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((spi->info->flags & SPI_SETUP) == 0U) { return ARM_DRIVER_ERROR; }
  if ( spi->info->status.busy)              { return ARM_DRIVER_ERROR_BUSY; }

  spi->info->status.busy       = 1U;
  spi->info->status.data_lost  = 0U;

  spi->info->xfer.rx_buf = NULL;
  spi->info->xfer.tx_buf = (uint8_t *)data;
  spi->info->xfer.num    = num;
  spi->info->xfer.rx_cnt = 0U;
  spi->info->xfer.tx_cnt = 0U;

  Chip_SPI_SetTXCTRLRegBits (spi->reg, SPI_TXCTL_RXIGNORE);

  Chip_SPI_EnableInts (spi->reg, SPI_INTENSET_TXDYEN);

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

  Chip_SPI_EnableInts (spi->reg, SPI_INTENSET_TXDYEN);

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

  if (spi->reg->CFG & SPI_CFG_MASTER_EN) {
    
  }
  Chip_SPI_EnableInts (spi->reg, SPI_INTENSET_TXDYEN);

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
    Chip_SPI_EnableMasterMode (spi->reg);

    /* Set SCK frequency */
    Chip_SPIM_SetClockRate    (spi->reg, arg);

    /* Set SSEL number to assert in master mode */
    Chip_SPIM_AssertSSEL (spi->reg, spi->ssel->num);
  } else {
    Chip_SPI_EnableSlaveMode (spi->reg);
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
  Chip_SPI_SetTXCTRLRegBits (spi->reg, SPI_TXDATCTL_FLEN(bits - 1));

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
  uint32_t stat;
  uint32_t data;
  bool end;

  end   = false;
  event = 0U;
  stat  = Chip_SPI_GetPendingInts (spi->reg);

  if (stat & SPI_INTSTAT_RXOV) {
    /* Receiver overrun */
    Chip_SPI_ClearStatus (spi->reg, SPI_STAT_RXOV);

    spi->info->status.data_lost = 1U;
    event |= ARM_SPI_EVENT_DATA_LOST;
  }

  if (stat & SPI_INTSTAT_TXUR) {
    /* Transmitter underfun */
    Chip_SPI_ClearStatus (spi->reg, SPI_STAT_TXUR);

    spi->info->status.data_lost = 1U;
    event |= ARM_SPI_EVENT_DATA_LOST;
  }

  if (stat & SPI_INTSTAT_TXRDY) {
    /* Transmitter Ready */
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
      Chip_SPI_WriteTXData (spi->reg, data);
      tr->tx_cnt++;

      if (tr->tx_cnt == tr->num) {
        /* All data sent */
        end = true;

        /* Disable TXRDY interrupt */
        Chip_SPI_DisableInts (spi->reg, SPI_INTENSET_TXDYEN);
      }
    }
    else {
      /* Unexpected transfer, data is lost */
      event |= ARM_SPI_EVENT_DATA_LOST;
    }
  }

  if (stat & SPI_INTSTAT_RXRDY) {
    /* Receiver Ready */
    data = Chip_SPI_ReadRawRXFifo (spi->reg);

    if (tr->rx_cnt < tr->num) {
      if (tr->rx_buf) {
        *(tr->rx_buf++) = (uint8_t)data;

        if (spi->info->flags & SPI_16B_FRAME) {
          *(tr->rx_buf++) = (uint8_t)(data >> 8);
        }
      }
      tr->rx_cnt++;

      if (tr->rx_cnt == tr->num) {
        /* All data received */
        end = true;
      }
    }
    else {
      spi->info->status.data_lost = 1U;
      event |= ARM_SPI_EVENT_DATA_LOST;
    }
  }

  if (end) {
    /* Transfer completed */
    if (spi->reg->CFG & SPI_CFG_MASTER_EN) {
      spi->reg->STAT = SPI_STAT_EOT;
    }
    if (spi->reg->TXCTRL & SPI_TXCTL_RXIGNORE) {
      Chip_SPI_ClearTXCTRLRegBits (spi->reg, SPI_TXCTL_RXIGNORE);
    }

    event |= ARM_SPI_EVENT_TRANSFER_COMPLETE;

    tr->num = 0U;
    spi->info->status.busy = 0U;
  }

  /* Send event */
  if ((event != 0U) && (spi->info->cb_event != NULL)) {
    spi->info->cb_event (event);
  }
}


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
