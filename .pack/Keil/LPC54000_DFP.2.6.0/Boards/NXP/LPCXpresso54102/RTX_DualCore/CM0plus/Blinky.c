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

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "chip.h"                       // Device header
#include "Board_LED.h"


osThreadId tid_mainThread;                      /* main thread ID             */


/*----------------------------------------------------------------------------
  MAILBOX_IRQHandler
 *----------------------------------------------------------------------------*/
void MAILBOX_IRQHandler(void) {
  uint32_t mbxVal;

  mbxVal = LPC_MBOX->BOX[0].IRQ;                /* get mailbox event          */
  LPC_MBOX->BOX[0].IRQCLR = 0xFFFFFFFF;         /* clear own mailbox          */
  switch (mbxVal & 0xF000) {
    case 0x1000:                                /* start action is requested  */
       LPC_MBOX->BOX[1].IRQSET = mbxVal;        /* acknowledge to M4          */
      break;
    case 0x2000:                                /* LED action is requested    */
       osSignalSet(tid_mainThread, (int32_t)mbxVal & 0xF);
                                                /* no acknowledge to M4       */
      break;
  }

}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {
  osEvent evt;

  tid_mainThread = osThreadGetId ();            /* get the own thread ID      */

  NVIC_EnableIRQ(MAILBOX_IRQn);                 /* enable mailbox interrupt   */

  for (;;) {
    evt = osSignalWait(0, osWaitForever);       /* wait for any signal        */
    if (evt.status == osEventSignal) {
      LED_SetOut (evt.value.signals);
    }
  }
}
