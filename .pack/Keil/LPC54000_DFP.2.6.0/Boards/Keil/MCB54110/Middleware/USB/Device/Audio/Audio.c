/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Audio.c
 * Purpose: USB Device Audio Example
 *----------------------------------------------------------------------------*/

#include <string.h>
#include "cmsis_os.h"
#include "rl_usb.h"

#include "chip.h"

int main (void) {

  PLL_SETUP_T pll_setup = {
    0x00047C80,
    0x0000042D,
    0x000000C2,
    {0x000634D3, (1 << 28)},
     24576000,
    PLL_SETUPFLAG_WAITLOCK | PLL_SETUPFLAG_POWERUP
  };

  Chip_Clock_SetMainClockSource (SYSCON_MAINCLKSRC_FRO12MHZ);

  Chip_SYSCON_PowerDown(SYSCON_PDRUNCFG_PD_SYS_PLL);
  while ((Chip_SYSCON_GetPowerStates() & SYSCON_PDRUNCFG_PD_SYS_PLL) == 0);
  Chip_Clock_SetSystemPLLSource(SYSCON_PLLCLKSRC_FRO12MHZ);
  Chip_Clock_SetPLLFreq(&pll_setup);

  SystemCoreClockUpdate();
  Chip_Clock_SetMCLKClockSource (SYSCON_MCLKSRC_PLL, 4);

  // Set FLEXCOMM (I2S) clock source to MCLK
  Chip_Clock_SetFLEXCOMMClockSource (6, SYSCON_FLEXCOMMCLKSELSRC_MCLK);
  Chip_Clock_SetFLEXCOMMClockSource (7, SYSCON_FLEXCOMMCLKSELSRC_MCLK);

  USBD_Initialize         (0);          /* USB Device 0 Initialization        */
  USBD_Connect            (0);          /* USB Device 0 Connect               */

  while (1) {
    osDelay(10000);
  }
}
