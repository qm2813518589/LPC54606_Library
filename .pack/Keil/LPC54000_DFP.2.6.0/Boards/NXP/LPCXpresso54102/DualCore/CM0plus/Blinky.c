/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Dual Core LED Flasher for LPC54102-LPCXpresso  (M0+ part)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "chip.h"                       // Device header
#include "Board_LED.h"


uint32_t ledMax;

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
  uint32_t   ledState;
  uint32_t *pledState;

  pledState = (uint32_t *)LPC_MBOX->BOX[0].IRQ; /* get address of ledState    */

  MAILBOX_mutexWait();
  ledState = *pledState;                        /* get value of ledState      */
  ledState = (ledState << 1);                   /* change ledState            */
  if (ledState > (1 << ledMax)) {
    ledState = 1;
  }
  *pledState = ledState;                        /* set new ledState           */
  MAILBOX_mutexRelease();

  LPC_MBOX->BOX[0].IRQCLR = 0xFFFFFFFF;         /* clear own mailbox          */
  LPC_MBOX->BOX[1].IRQSET = 1;                  /* signal change to M4        */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {

  SystemCoreClockUpdate();

  ledMax = LED_GetCount() - 1;

  NVIC_EnableIRQ(MAILBOX_IRQn);                 /* enable mailbox interrupt   */

  while (1) __NOP();
}
