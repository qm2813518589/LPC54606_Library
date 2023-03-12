/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Dual Core LED Flasher for MCB54110 (M0+ part)
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

/*----------------------------------------------------------------------------
  MAILBOX functions
 *----------------------------------------------------------------------------*/
static void MAILBOX_mutexWait(void) {

  while (LPC_MBOX->MUTEX == 0) __NOP();         /* wait forever for mutex     */
}

static void MAILBOX_mutexRelease(void) {

  LPC_MBOX->MUTEX = 1;                          /* release mutex              */
}


/*----------------------------------------------------------------------------
  MAILBOX_IRQHandler
 *----------------------------------------------------------------------------*/
void MAILBOX_IRQHandler(void) {
  uint32_t   bit;
  uint32_t *pbit;

  pbit = (uint32_t *)LPC_MBOX->BOX[0].IRQ;      /* get address of BitNum      */

  MAILBOX_mutexWait();

  bit   = *pbit;                                /* get value of BitNum        */
  bit  += 1;
  *pbit = bit;                                  /* net new BitNum value       */

  MAILBOX_mutexRelease();

  LPC_MBOX->BOX[0].IRQCLR = 0xFFFFFFFF;         /* clear own mailbox          */
  LPC_MBOX->BOX[1].IRQSET = 1;                  /* signal change to M4        */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {

  SystemCoreClockUpdate();

  NVIC_EnableIRQ(MAILBOX_IRQn);                 /* enable mailbox interrupt   */

  while (1) __NOP();
}
