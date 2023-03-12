/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Dual Core LED Flasher for MCB54110 (M4 part)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2016 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "chip.h"                       // Device header
#include "Board_LED.h"

#define M0plus_IMAGE_START    0x20000          /* M0+ Image located in flash  */


volatile uint32_t BitNum;      /* must be global because we use it's addresss */
volatile uint32_t M0plus_Trigger;


/*----------------------------------------------------------------------------
  MAILBOX functions
 *----------------------------------------------------------------------------*/
static void MAILBOX_mutexWait(void) {

  while (LPC_MBOX->MUTEX == 0) __NOP();    /* wait forever for mutex       */
}

static void MAILBOX_mutexRelease(void) {

  LPC_MBOX->MUTEX = 1;                     /* release mutex                */
}

static void MAILBOX_Initialize(void) {

  LPC_SYSCON->AHBCLKCTRL[0]    |= (1ul << 26);  /* enable clock for Mailbox     */
  LPC_SYSCON->PRESETCTRLSET[0]  = (1ul << 26);  /* assert reset for Mailbox     */
  LPC_SYSCON->PRESETCTRLCLR[0]  = (1ul << 26);  /* deassert reset for Mailbox   */

  MAILBOX_mutexRelease();
}


/*----------------------------------------------------------------------------
  SysTick IRQ Handler @ 10ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;

  if (ticks++ > 24) {
    ticks = 0;
    M0plus_Trigger = 1;
  }

}

/*----------------------------------------------------------------------------
  MAILBOX_IRQHandler
 *----------------------------------------------------------------------------*/
void MAILBOX_IRQHandler(void) {

  LPC_MBOX->BOX[1].IRQCLR = 0xFFFFFFFF;       /* clear own mailbox            */

  if (BitNum >= LED_GetCount()) {
    BitNum = 0;
  }

  LED_SetOut(1 << BitNum);                    /* output new LED state         */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {
  volatile uint32_t stopHereForM0plusDebugging = 0;

  BitNum = 0;                                 /* value is changed from M0+    */

  SystemCoreClockUpdate();
  LED_Initialize();

  MAILBOX_Initialize();                       /* initialize mailbox (M4 only) */

  NVIC_SetPriority (MAILBOX_IRQn, 1);
  NVIC_EnableIRQ(MAILBOX_IRQn);               /* enable mailbox interrupt     */

  SysTick_Config(SystemCoreClock/100);        /* SysTick irq each 10 ms       */

  /* Boot M0+ core */
  /* Setup M0+ stack pointer and  reset vector ( from M0+ flash image) */
  Chip_CPU_CM0Boot((uint32_t *)(*(uint32_t *)(M0plus_IMAGE_START + 4)),
                   (uint32_t *)(*(uint32_t *)(M0plus_IMAGE_START + 0)) );

  stopHereForM0plusDebugging = 1;

  while (1) {

    if (M0plus_Trigger) {
      M0plus_Trigger  = 0;

      MAILBOX_mutexWait();
      LPC_MBOX->BOX[0].IRQ = (uint32_t)&BitNum; /* signal address to M0+       */
      MAILBOX_mutexRelease();
    }
  }
}
