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
 * Driver:       Driver_I2C0, Driver_I2C1, Driver_I2C2
 * Configured:   via RTE_Device.h configuration file
 * Project:      I2C Driver for NXP LPC5410x
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                 Value   I2C Interface
 *   ---------------------                 -----   -------------
 *   Connect to hardware via Driver_I2C# = 0       use I2C0
 *   Connect to hardware via Driver_I2C# = 1       use I2C1
 *   Connect to hardware via Driver_I2C# = 2       use I2C2
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    - Initial release
 */

#include "I2C_LPC5410x.h"

#define ARM_I2C_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0) /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
  ARM_I2C_API_VERSION,
  ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = {
  1            /* supports 10-bit addressing */
};


#if (RTE_I2C0)
/* I2C0 Control Information */
static I2C_INFO I2C0_Ctrl = { 0 };

/* I2C0 Resources */
static I2C_RESOURCES I2C0_Resources = {
  LPC_I2C0,
  I2C0_IRQn,
  SYSCON_CLOCK_I2C0,
  { RTE_I2C0_SCL_PORT, RTE_I2C0_SCL_PIN, RTE_I2C0_SCL_FUNC },
  { RTE_I2C0_SDA_PORT, RTE_I2C0_SDA_PIN, RTE_I2C0_SDA_FUNC },
  &I2C0_Ctrl
};
#endif /* RTE_I2C0 */


#if (RTE_I2C1)
/* I2C1 Control Information */
static I2C_INFO I2C1_Ctrl = { 0 };

/* I2C1 Resources */
static I2C_RESOURCES I2C1_Resources = {
  LPC_I2C1,
  I2C1_IRQn,
  SYSCON_CLOCK_I2C1,
  { RTE_I2C1_SCL_PORT, RTE_I2C1_SCL_PIN, RTE_I2C1_SCL_FUNC },
  { RTE_I2C1_SDA_PORT, RTE_I2C1_SDA_PIN, RTE_I2C1_SDA_FUNC },
  &I2C1_Ctrl
};
#endif /* RTE_I2C1 */


#if (RTE_I2C2)
/* I2C2 Control Information */
static I2C_INFO I2C2_Ctrl = { 0 };

/* I2C2 Resources */
static I2C_RESOURCES I2C2_Resources = {
  LPC_I2C2,
  I2C2_IRQn,
  SYSCON_CLOCK_I2C2,
  { RTE_I2C2_SCL_PORT, RTE_I2C2_SCL_PIN, RTE_I2C2_SCL_FUNC },
  { RTE_I2C2_SDA_PORT, RTE_I2C2_SDA_PIN, RTE_I2C2_SDA_FUNC },
  &I2C2_Ctrl
};
#endif /* RTE_I2C2 */


/**
  \fn          ARM_DRIVER_VERSION I2C_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION I2C_GetVersion (void) {
  return DriverVersion;
}

/**
  \fn          ARM_I2C_CAPABILITIES I2C_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_I2C_CAPABILITIES
*/
static ARM_I2C_CAPABILITIES I2C_GetCapabilities (void) {
  return DriverCapabilities;
}

/**
  \fn          int32_t I2Cx_Initialize (ARM_I2C_SignalEvent_t cb_event,
                                        I2C_RESOURCES         *i2c)
  \brief       Initialize I2C Interface.
  \param[in]   cb_event  Pointer to \ref ARM_I2C_SignalEvent
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c) {

  if (i2c->info->flags & I2C_INIT) { return ARM_DRIVER_OK; }

  /* Enable power */
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO0);
  Chip_Clock_EnablePeriphClock (SYSCON_CLOCK_GPIO1);

  /* Configure I2C pins */
  Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_STANDARD);
  Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_STANDARD);

  /* Reset Run-Time information structure */
  memset (i2c->info, 0x00, sizeof (I2C_INFO));

  i2c->info->cb_event = cb_event;
  i2c->info->flags    = I2C_INIT;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_Uninitialize (I2C_RESOURCES *i2c)
  \brief       De-initialize I2C Interface.
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_Uninitialize (I2C_RESOURCES *i2c) {

  i2c->info->flags = 0U;

  /* Unconfigure SCL and SDA pins */
  Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_GPIO);
  Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_GPIO);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_PowerControl (ARM_POWER_STATE state,
                                          I2C_RESOURCES   *i2c)
  \brief       Control I2C Interface Power.
  \param[in]   state  Power state
  \param[in]   i2c    Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_PowerControl (ARM_POWER_STATE state, I2C_RESOURCES *i2c) {

  switch (state) {
    case ARM_POWER_OFF:
      /* Power off I2C peripheral */
      NVIC_DisableIRQ (i2c->irqn);

      /* Enable power */
      Chip_Clock_EnablePeriphClock (i2c->clk_msk);

      if (i2c->info->status.busy) {
        if (i2c->info->status.mode) {
          Chip_I2CM_SendStop (i2c->reg);
        }
        else {
          if (i2c->info->status.direction != 0) {
            /* Slave receiver */
            Chip_I2CS_SlaveNACK (i2c->reg);
          }
        }
      }

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.direction        = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;

      i2c->info->flags  &= ~I2C_POWER;

      /* Disable master/slave interface */
      Chip_I2CM_Disable (i2c->reg);
      Chip_I2CS_Disable (i2c->reg);

      /* Deinit peripheral */
      Chip_I2C_DeInit (i2c->reg);
      break;

    case ARM_POWER_FULL:
      if ((i2c->info->flags & I2C_INIT)  == 0U) { return ARM_DRIVER_ERROR; }
      if ((i2c->info->flags & I2C_POWER) != 0U) { return ARM_DRIVER_OK; }

      /* Init peripheral (enable clock + reset) */
      Chip_I2C_Init (i2c->reg);

      /* Enable master interface */
      Chip_I2CM_Enable (i2c->reg);

      /* Enable master interrupts */
      Chip_I2C_EnableInt (i2c->reg, I2C_INTENSET_MSTRARBLOSS |
                                    I2C_INTENSET_MSTSTSTPERR);

      /* Enable I2C interrupts in NVIC */
      NVIC_ClearPendingIRQ (i2c->irqn);
      NVIC_EnableIRQ       (i2c->irqn);

      i2c->info->flags |= I2C_POWER;
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_MasterTransmit (uint32_t       addr,
                                            const uint8_t *data,
                                            uint32_t       num,
                                            bool           xfer_pending,
                                            I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to transmit to I2C Slave
  \param[in]   num           Number of data bytes to transmit
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_MasterTransmit (uint32_t       addr,
                                    const uint8_t *data,
                                    uint32_t       num,
                                    bool           xfer_pending,
                                    I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((addr & ~(ARM_I2C_ADDRESS_10BIT | ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  if ((Chip_I2CM_GetStatus (i2c->reg) & I2C_STAT_MSTPENDING) == 0U) {
    /* Communication in progress */
    return ARM_DRIVER_ERROR_BUSY;
  }

  /* Update driver status and setup transfer */
  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 0U;
  i2c->info->status.arbitration_lost = 0U;
  i2c->info->status.bus_error        = 0U;

  i2c->info->xfer.data = (uint8_t *)data;
  i2c->info->xfer.num  = num;
  i2c->info->xfer.cnt  = 0U;
  i2c->info->xfer.addr = (uint16_t)addr;
  i2c->info->xfer.ctrl = 0U;

  if (xfer_pending) {
    i2c->info->xfer.ctrl |= XFER_CTRL_XPENDING;
  }
  if ((addr & ARM_I2C_ADDRESS_10BIT) == 0U) {
    i2c->info->xfer.ctrl |= XFER_CTRL_ADDR_DONE;
  }

  /* Write address and direction bit into data buffer */
  addr  &= 0x7F;
  addr <<= 1;
  Chip_I2CM_WriteByte (i2c->reg, addr);

  /* Generate start */
  Chip_I2CM_SendStart (i2c->reg);

  /* Enable pending interrupt */
  Chip_I2C_EnableInt (i2c->reg, I2C_INTENSET_MSTPENDING);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_MasterReceive (uint32_t       addr,
                                           uint8_t       *data,
                                           uint32_t       num,
                                           bool           xfer_pending,
                                           I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[out]  data          Pointer to buffer for data to receive from I2C Slave
  \param[in]   num           Number of data bytes to receive
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_MasterReceive (uint32_t       addr,
                                   uint8_t       *data,
                                   uint32_t       num,
                                   bool           xfer_pending,
                                   I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((addr & ~(ARM_I2C_ADDRESS_10BIT | ARM_I2C_ADDRESS_GC)) > 0x3FFU) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  if ((Chip_I2CM_GetStatus (i2c->reg) & I2C_STAT_MSTPENDING) == 0U) {
    /* Communication in progress */
    return ARM_DRIVER_ERROR_BUSY;
  }

  /* Update driver status and setup transfer */
  i2c->info->status.busy             = 1U;
  i2c->info->status.mode             = 1U;
  i2c->info->status.direction        = 1U;
  i2c->info->status.arbitration_lost = 0U;
  i2c->info->status.bus_error        = 0U;

  i2c->info->xfer.data = data;
  i2c->info->xfer.num  = num;
  i2c->info->xfer.cnt  = 0U;
  i2c->info->xfer.addr = (uint16_t)addr;
  i2c->info->xfer.ctrl = 0U;

  if (xfer_pending) {
    i2c->info->xfer.ctrl |= XFER_CTRL_XPENDING;
  }
  if ((addr & ARM_I2C_ADDRESS_10BIT) == 0U) {
    i2c->info->xfer.ctrl |= XFER_CTRL_ADDR_DONE;
  }

  /* Write address and direction bit into data buffer */
  addr  &= 0x7F;
  addr <<= 1;
  Chip_I2CM_WriteByte (i2c->reg, addr | 1);

  /* Generate start */
  Chip_I2CM_SendStart (i2c->reg);

  /* Enable pending interrupt */
  Chip_I2C_EnableInt (i2c->reg, I2C_INTENSET_MSTPENDING);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_SlaveTransmit (const uint8_t *data,
                                           uint32_t       num,
                                           I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Slave.
  \param[in]   data  Pointer to buffer with data to transmit to I2C Master
  \param[in]   num   Number of data bytes to transmit
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_SlaveTransmit (const uint8_t *data,
                                   uint32_t       num,
                                   I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->xfer.data = (uint8_t *)data;
  i2c->info->xfer.num  = num;
  i2c->info->xfer.cnt  = 0U;
  i2c->info->xfer.ctrl = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_SlaveReceive (uint8_t       *data,
                                          uint32_t       num,
                                          I2C_RESOURCES *i2c)
  \brief       Start receiving data as I2C Slave.
  \param[out]  data  Pointer to buffer for data to receive from I2C Master
  \param[in]   num   Number of data bytes to receive
  \param[in]   i2c   Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_SlaveReceive (uint8_t       *data,
                                  uint32_t       num,
                                  I2C_RESOURCES *i2c) {

  if ((data == NULL) || (num == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->info->status.busy) {
    return (ARM_DRIVER_ERROR_BUSY);
  }

  i2c->info->status.bus_error    = 0U;
  i2c->info->status.general_call = 0U;

  i2c->info->xfer.data = data;
  i2c->info->xfer.num  = num;
  i2c->info->xfer.cnt  = 0U;
  i2c->info->xfer.ctrl = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t I2Cx_GetDataCount (I2C_RESOURCES *i2c)
  \brief       Get transferred data count.
  \return      number of data bytes transferred; -1 when Slave is not addressed by Master
*/
static int32_t I2Cx_GetDataCount (I2C_RESOURCES *i2c) {
  return ((int32_t)i2c->info->xfer.cnt);
}

/**
  \fn          int32_t I2Cx_Control (uint32_t       control,
                                     uint32_t       arg,
                                     I2C_RESOURCES *i2c)
  \brief       Control I2C Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   i2c      pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_Control (uint32_t control, uint32_t arg, I2C_RESOURCES *i2c) {
  uint32_t clk;
  uint32_t i, j;
  uint32_t scl_mode, sda_mode;
  bool state;

  if ((i2c->info->flags & I2C_POWER) == 0U) {
    /* I2C not powered */
    return ARM_DRIVER_ERROR;
  }

  switch (control) {
    case ARM_I2C_OWN_ADDRESS:
      if (arg & ARM_I2C_ADDRESS_GC) {
        /* Set general call slave address */
        Chip_I2CS_SetSlaveAddr    (i2c->reg, 3, 0);
        Chip_I2CS_EnableSlaveAddr (i2c->reg, 3);
      } else {
        Chip_I2CS_DisableSlaveAddr (i2c->reg, 3);
      }

      if (arg & ARM_I2C_ADDRESS_10BIT) {
        /* Extend slave address to 10-bits */
        Chip_I2CS_SetSlaveQual0 (i2c->reg, true, 0x78U | ((arg & 0x3FFU) >> 8));
        i2c->info->flags |= I2C_SLAVE10BIT;
      } else {
        Chip_I2CS_SetSlaveQual0 (i2c->reg, false, 0);
        i2c->info->flags &= ~I2C_SLAVE10BIT;
      }

      if (arg & 0x7F) {
        /* Set 7-bit slave address */
        Chip_I2CS_SetSlaveAddr    (i2c->reg, 0, arg & 0x7FU);
        Chip_I2CS_EnableSlaveAddr (i2c->reg, 0);
      } else {
        Chip_I2CS_DisableSlaveAddr (i2c->reg, 0);
      }

      if (arg) {
        /* Enable slave mode */
        Chip_I2CS_Enable (i2c->reg);
        /* Enable slave interrupts */
        Chip_I2C_EnableInt (i2c->reg, I2C_INTENSET_SLVPENDING);
      } else {
        /* Disable slave interrupts */
        Chip_I2C_DisableInt (i2c->reg, I2C_INTENSET_SLVPENDING);
        /* Disable slave mode */
        Chip_I2CS_Disable (i2c->reg);
      }
      break;

    case ARM_I2C_BUS_SPEED:
      /* Set Bus Speed */
      switch (arg) {
        case ARM_I2C_BUS_SPEED_STANDARD:
          /* Standard Speed (100kHz) */
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_STANDARD);
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_STANDARD);
          clk = 100000;
          break;

        case ARM_I2C_BUS_SPEED_FAST:
          /* Fast Speed (400kHz) */
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_FAST);
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_FAST);
          clk = 400000;
          break;

        case ARM_I2C_BUS_SPEED_FAST_PLUS:
          /* Fast+ Speed (1MHz) */
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_FAST_PLUS);
          Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_FAST_PLUS);
          clk = 1000000;
          break;

        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }
      Chip_I2CM_SetBusSpeed (i2c->reg, clk);
      break;

    case ARM_I2C_BUS_CLEAR:
      /* Execute Bus clear */
      SystemCoreClockUpdate();

      /* Frequency is set to approx. standard speed (100kHz) */
      clk = SystemCoreClock / 100000 / 4;

      NVIC_DisableIRQ (i2c->irqn);

      scl_mode = LPC_IOCON->PIO[i2c->scl.port][i2c->scl.pin];
      sda_mode = LPC_IOCON->PIO[i2c->sda.port][i2c->sda.pin];

      /* Configure SCL and SDA as GPIO outputs */
      Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, I2C_PIN_IOCON_GPIO);
      Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, I2C_PIN_IOCON_GPIO);

      Chip_GPIO_SetPinDIROutput(LPC_GPIO, i2c->scl.port, i2c->scl.pin);
      Chip_GPIO_SetPinDIRInput (LPC_GPIO, i2c->sda.port, i2c->sda.pin);

      /* Pull SCL and SDA high */
      Chip_GPIO_SetPinState (LPC_GPIO, i2c->scl.port, i2c->scl.pin, true);
      Chip_GPIO_SetPinState (LPC_GPIO, i2c->sda.port, i2c->sda.pin, true);

      for (i = 0U; i < 9U; i++) {
        if (Chip_GPIO_ReadPortBit(LPC_GPIO, i2c->sda.port, i2c->sda.pin) == true) {
          /* Break if slave released SDA line */
          break;
        }
        /* Clock high */
        Chip_GPIO_SetPinState(LPC_GPIO, i2c->scl.port, i2c->scl.pin, true);
        for (j = 0; j < clk; j++);
        
        /* Clock low */
        Chip_GPIO_SetPinState(LPC_GPIO, i2c->scl.port, i2c->scl.pin, false);
        for (j = 0; j < clk; j++);
      }

      /* Check SDA state */
      state = Chip_GPIO_ReadPortBit(LPC_GPIO, i2c->sda.port, i2c->sda.pin);

      /* Generate STOP condition - SDA goes high while SCL is high */
      Chip_GPIO_SetPinDIROutput (LPC_GPIO, i2c->sda.port, i2c->sda.pin);

      Chip_GPIO_SetPinState (LPC_GPIO, i2c->sda.port, i2c->sda.pin, false);
      for (j = 0; j < clk; j++);
      Chip_GPIO_SetPinState (LPC_GPIO, i2c->scl.port, i2c->scl.pin, true);
      for (j = 0; j < clk; j++);
      Chip_GPIO_SetPinState (LPC_GPIO, i2c->sda.port, i2c->sda.pin, true);

      /* Configure SDA and SCL pins as I2C peripheral pins */
      Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->scl.port, i2c->scl.pin, scl_mode);
      Chip_IOCON_PinMuxSet (LPC_IOCON, i2c->sda.port, i2c->sda.pin, sda_mode);

      /* Send event */
      if (i2c->info->cb_event) {
        i2c->info->cb_event (ARM_I2C_EVENT_BUS_CLEAR);
      }

      NVIC_ClearPendingIRQ (i2c->irqn);
      NVIC_EnableIRQ       (i2c->irqn);

      return (state) ? ARM_DRIVER_OK : ARM_DRIVER_ERROR;

    case ARM_I2C_ABORT_TRANSFER:
      /* Abort Master/Slave transfer */
      NVIC_DisableIRQ (i2c->irqn);

      if (i2c->info->status.mode) {
        /* Master mode */
        Chip_I2CM_SendStop (i2c->reg);

        /* Wait until master idle */
        while (Chip_I2CM_GetMasterState(i2c->reg) != 0);

        Chip_I2CM_Disable (i2c->reg);
      } else {
        /* Slave mode */
        Chip_I2CS_Disable (i2c->reg);
      }

      i2c->info->xfer.data = NULL;
      i2c->info->xfer.cnt  = 0U;

      i2c->info->status.busy             = 0U;
      i2c->info->status.mode             = 0U;
      i2c->info->status.general_call     = 0U;
      i2c->info->status.arbitration_lost = 0U;
      i2c->info->status.bus_error        = 0U;

      /* Enable master */
      Chip_I2CM_Enable (i2c->reg);

      if (i2c->reg->SLVADR[0] & 1) {
        /* Slave address is set, enable slabe */
        Chip_I2CS_Enable (i2c->reg);
      }

      NVIC_ClearPendingIRQ (i2c->irqn);
      NVIC_EnableIRQ       (i2c->irqn);
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_I2C_STATUS I2Cx_GetStatus (I2C_RESOURCES *i2c)
  \brief       Get I2C status.
  \param[in]   i2c      pointer to I2C resources
  \return      I2C status \ref ARM_I2C_STATUS
*/
static ARM_I2C_STATUS I2Cx_GetStatus (I2C_RESOURCES *i2c) {
  return (i2c->info->status);
}


/**
  \fn          void I2Cx_IRQHandler (I2C_RESOURCES *i2c)
  \brief       I2C Event Interrupt handler.
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2Cx_IRQHandler (I2C_RESOURCES *i2c) {
  I2C_TRANSFER_INFO *tr = &i2c->info->xfer;
  uint32_t status, event;
  uint8_t  data;

  event  = 0U;
  status = Chip_I2C_GetPendingInt (i2c->reg);

  if (status & I2C_MASTER_INTERRUPTS) {
    /* Master interrupt is pending */
    if (status & I2C_INTENSET_MSTPENDING) {
      /* Master is Pending */
      switch (Chip_I2CM_GetMasterState(i2c->reg)) {
        case I2C_STAT_MSTCODE_IDLE:   /* Master idle */
          /* Transfer done */
          event = ARM_I2C_EVENT_TRANSFER_DONE;

          i2c->info->status.busy = 0U;

          /* Disable pending interrupt */
          Chip_I2C_DisableInt (i2c->reg, I2C_INTENSET_MSTPENDING);
          break;

          case I2C_STAT_MSTCODE_RXREADY: /* Rx ready. Received data available */
            if (tr->cnt < tr->num) {
              /* Interrupt data transfer */
              tr->data[tr->cnt] = Chip_I2CM_ReadByte(i2c->reg);
              tr->cnt++;
            }

            if (tr->cnt < tr->num) {
              Chip_I2CM_MasterContinue (i2c->reg);
            }
            else {
              /* Transfer done */
              if ((tr->ctrl & XFER_CTRL_XPENDING) == 0) {
                /* Generate stop */
                Chip_I2CM_SendStop (i2c->reg);
              }
              else {
                /* Transfer done */
                event = ARM_I2C_EVENT_TRANSFER_DONE;

                i2c->info->status.busy = 0U;

                /* Disable pending interrupt */
                Chip_I2C_DisableInt (i2c->reg, I2C_INTENSET_MSTPENDING);
              }
            }
            break;

          case I2C_STAT_MSTCODE_TXREADY: /* Tx ready. Data can be transmitted */
            if (tr->ctrl & XFER_CTRL_ADDR_DONE) {
              /* Addressing done, transfer data */
              if (tr->cnt < tr->num) {
                /* Interrupt data transfer */
                Chip_I2CM_WriteByte (i2c->reg, tr->data[tr->cnt]);
                tr->cnt++;

                Chip_I2CM_MasterContinue (i2c->reg);
              }
              else {
                /* Transfer done */
                event = ARM_I2C_EVENT_TRANSFER_DONE;

                i2c->info->status.busy = 0U;

                /* Disable pending interrupt */
                Chip_I2C_DisableInt (i2c->reg, I2C_INTENSET_MSTPENDING);

                if ((tr->ctrl & XFER_CTRL_XPENDING) == 0) {
                  /* Generate stop */
                  Chip_I2CM_SendStop (i2c->reg);
                }
              }
            }
            else {
              /* Send 2nd byte of 10-bit address */
              Chip_I2CM_WriteByte      (i2c->reg, (uint8_t)tr->addr);
              Chip_I2CM_MasterContinue (i2c->reg);

              /* Addressing is now done */
              tr->ctrl |= XFER_CTRL_ADDR_DONE;
            }
            break;

          case I2C_STAT_MSTCODE_NACKADR: /* NACK Address. Slave NACKed address */
            event = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_ADDRESS_NACK;

            /* Generate stop */
            Chip_I2CM_SendStop (i2c->reg);
            break;

          case I2C_STAT_MSTCODE_NACKDAT: /* NACK Data. Slave NACKed transmitted data */
            event = ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_TRANSFER_INCOMPLETE;

            /* Generate stop */
            Chip_I2CM_SendStop (i2c->reg);
            break;

          default: /* Illegal master state, should never happen */
            Chip_I2CM_MasterContinue (i2c->reg);
            break;
      }
    }
    else {
      if (status & I2C_INTENSET_MSTRARBLOSS) {
        /* Arbitration lost */
        Chip_I2CM_ClearStatus(i2c->reg, I2C_STAT_MSTRARBLOSS);

        /* Update status */
        i2c->info->status.arbitration_lost = 1U;
        i2c->info->status.busy             = 0U;

        /* Reset transfer state */
        tr->data = NULL;

        /* Set arbitration lost event */
        event |= ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_ARBITRATION_LOST;

        if (tr->cnt < tr->num) {
          event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
        }
      }
      else /*status & I2C_INTENSET_MSTSTSTPERR */ {
        /* Start/Stop error */
        Chip_I2CM_ClearStatus(i2c->reg, I2C_STAT_MSTSTSTPERR);

        /* Update status */
        i2c->info->status.bus_error = 1U;
        i2c->info->status.busy      = 0U;

        /* Reset transfer state */
        tr->data = NULL;

        /* Set bus error event */
        event |= ARM_I2C_EVENT_TRANSFER_DONE | ARM_I2C_EVENT_BUS_ERROR;

        if (tr->cnt < tr->num) {
          event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
        }
      }
    }
  }
  else if (status & I2C_SLAVE_INTERRUPTS) {
    /* Slave interrupt is pending */
    if (status & I2C_INTENSET_SLVPENDING) {

      switch (Chip_I2CS_GetSlaveState (i2c->reg)) {
        case I2C_STAT_SLVCODE_ADDR: /* Address plus R/nW received */
          Chip_I2C_EnableInt (i2c->reg, I2C_INTENSET_SLVDESEL);

          /* Read received address, LSB is R/nW */
          data = Chip_I2CS_ReadByte(i2c->reg);

          if (data & 1) { event = ARM_I2C_EVENT_SLAVE_TRANSMIT; }
          else          { event = ARM_I2C_EVENT_SLAVE_RECEIVE;  }

          if (i2c->info->cb_event) {
            if (Chip_I2CS_GetSlaveMatchIndex(i2c->reg) == 3) {
              /* General call address match */
              i2c->info->cb_event (event | ARM_I2C_EVENT_GENERAL_CALL);
            } else {
              if (tr->data == NULL) {
                /* Transfer data is not set */
                i2c->info->cb_event (event);
              }
            }
          }
          event = 0U;

          if ((i2c->info->flags & I2C_SLAVE10BIT) == 0U) {
            /* 7-bit addressing */
            tr->ctrl |= XFER_CTRL_ADDR_DONE;
          }

          i2c->info->status.busy = 1U;

          /* Ack the address */
          Chip_I2CS_SlaveContinue (i2c->reg);
          break;

        case I2C_STAT_SLVCODE_RX:   /* Received data is available */
          if ((tr->ctrl & XFER_CTRL_ADDR_DONE) == 0U) {
            /* 10-bit addressing: 2nd address byte received */
            tr->ctrl |= XFER_CTRL_ADDR_DONE;

            Chip_I2CS_ReadByte(i2c->reg);
            Chip_I2CS_SlaveContinue (i2c->reg);
          }
          else {
            /* Data transfer */
            if (tr->data == NULL) {
              /* Data buffer not set */
              Chip_I2CS_SlaveNACK (i2c->reg);
            }
            else {
              /* IRQ data transfer */
              if (tr->cnt < tr->num) {
                tr->data[tr->cnt] = Chip_I2CS_ReadByte(i2c->reg);
                tr->cnt++;
              }
              Chip_I2CS_SlaveContinue (i2c->reg);
            }
          }
          break;

        case I2C_STAT_SLVCODE_TX:   /* Data can be transmitted */
          if (tr->data == NULL) {
            /* Data buffer not set */
            Chip_I2CM_WriteByte (i2c->reg, 0xFFU);
          }
          else {
            /* IRQ data transfer */
            if (tr->cnt < tr->num) {
              Chip_I2CM_WriteByte (i2c->reg, tr->data[tr->cnt]);
              tr->cnt++;
            }
          }
          Chip_I2CS_SlaveContinue (i2c->reg);
          break;
      }
    }
    else {
      if (status & I2C_INTENSET_SLVDESEL) {
        /* Slave deselected */
        Chip_I2C_DisableInt (i2c->reg, I2C_INTENSET_SLVDESEL);

        if (tr->data) {
          tr->data = NULL;

          event = ARM_I2C_EVENT_TRANSFER_DONE;

          if (tr->cnt < tr->num) {
            event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
          }

          if (i2c->info->status.general_call) {
            event |= ARM_I2C_EVENT_GENERAL_CALL;
          }
        }
        i2c->info->status.busy = 0U;
      }
    }
  }

  /* Callback event notification */
  if (event && i2c->info->cb_event) {
    i2c->info->cb_event (event);
  }
}


#if (RTE_I2C0)
/* I2C0 Driver wrapper functions */
static int32_t I2C0_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return (I2Cx_Initialize (cb_event, &I2C0_Resources));
}
static int32_t I2C0_Uninitialize (void) {
  return (I2Cx_Uninitialize (&I2C0_Resources));
}
static int32_t I2C0_PowerControl (ARM_POWER_STATE state) {
  return (I2Cx_PowerControl (state, &I2C0_Resources));
}
static int32_t I2C0_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C0_Resources));
}
static int32_t I2C0_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C0_Resources));
}
static int32_t I2C0_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveTransmit (data, num, &I2C0_Resources));
}
static int32_t I2C0_SlaveReceive (uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveReceive (data, num, &I2C0_Resources));
}
static int32_t I2C0_GetDataCount (void) {
  return (I2Cx_GetDataCount (&I2C0_Resources));
}
static int32_t I2C0_Control (uint32_t control, uint32_t arg) {
  return (I2Cx_Control (control, arg, &I2C0_Resources));
}
static ARM_I2C_STATUS I2C0_GetStatus (void) {
  return (I2Cx_GetStatus (&I2C0_Resources));
}
/* I2C0 interrupt handler */
void I2C0_IRQHandler (void) {
  I2Cx_IRQHandler (&I2C0_Resources);
}


/* I2C0 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C0 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C0_Initialize,
  I2C0_Uninitialize,
  I2C0_PowerControl,
  I2C0_MasterTransmit,
  I2C0_MasterReceive,
  I2C0_SlaveTransmit,
  I2C0_SlaveReceive,
  I2C0_GetDataCount,
  I2C0_Control,
  I2C0_GetStatus
};
#endif

#if (RTE_I2C1)
/* I2C1 Driver wrapper functions */
static int32_t I2C1_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return (I2Cx_Initialize (cb_event, &I2C1_Resources));
}
static int32_t I2C1_Uninitialize (void) {
  return (I2Cx_Uninitialize (&I2C1_Resources));
}
static int32_t I2C1_PowerControl (ARM_POWER_STATE state) {
  return (I2Cx_PowerControl (state, &I2C1_Resources));
}
static int32_t I2C1_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C1_Resources));
}
static int32_t I2C1_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C1_Resources));
}
static int32_t I2C1_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveTransmit (data, num, &I2C1_Resources));
}
static int32_t I2C1_SlaveReceive (uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveReceive (data, num, &I2C1_Resources));
}
static int32_t I2C1_GetDataCount (void) {
  return (I2Cx_GetDataCount (&I2C1_Resources));
}
static int32_t I2C1_Control (uint32_t control, uint32_t arg) {
  return (I2Cx_Control (control, arg, &I2C1_Resources));
}
static ARM_I2C_STATUS I2C1_GetStatus (void) {
  return (I2Cx_GetStatus (&I2C1_Resources));
}
/* I2C1 interrupt handler */
void I2C1_IRQHandler (void) {
  I2Cx_IRQHandler (&I2C1_Resources);
}


/* I2C1 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C1 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C1_Initialize,
  I2C1_Uninitialize,
  I2C1_PowerControl,
  I2C1_MasterTransmit,
  I2C1_MasterReceive,
  I2C1_SlaveTransmit,
  I2C1_SlaveReceive,
  I2C1_GetDataCount,
  I2C1_Control,
  I2C1_GetStatus
};
#endif

#if (RTE_I2C2)
/* I2C2 Driver wrapper functions */
static int32_t I2C2_Initialize (ARM_I2C_SignalEvent_t cb_event) {
  return (I2Cx_Initialize (cb_event, &I2C2_Resources));
}
static int32_t I2C2_Uninitialize (void) {
  return (I2Cx_Uninitialize (&I2C2_Resources));
}
static int32_t I2C2_PowerControl (ARM_POWER_STATE state) {
  return (I2Cx_PowerControl (state, &I2C2_Resources));
}
static int32_t I2C2_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C2_Resources));
}
static int32_t I2C2_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) {
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C2_Resources));
}
static int32_t I2C2_SlaveTransmit (const uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveTransmit (data, num, &I2C2_Resources));
}
static int32_t I2C2_SlaveReceive (uint8_t *data, uint32_t num) {
  return (I2Cx_SlaveReceive (data, num, &I2C2_Resources));
}
static int32_t I2C2_GetDataCount (void) {
  return (I2Cx_GetDataCount (&I2C2_Resources));
}
static int32_t I2C2_Control (uint32_t control, uint32_t arg) {
  return (I2Cx_Control (control, arg, &I2C2_Resources));
}
static ARM_I2C_STATUS I2C2_GetStatus (void) {
  return (I2Cx_GetStatus (&I2C2_Resources));
}
/* I2C2 interrupt handler */
void I2C2_IRQHandler (void) {
  I2Cx_IRQHandler (&I2C2_Resources);
}


/* I2C2 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C2 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C2_Initialize,
  I2C2_Uninitialize,
  I2C2_PowerControl,
  I2C2_MasterTransmit,
  I2C2_MasterReceive,
  I2C2_SlaveTransmit,
  I2C2_SlaveReceive,
  I2C2_GetDataCount,
  I2C2_Control,
  I2C2_GetStatus
};
#endif
