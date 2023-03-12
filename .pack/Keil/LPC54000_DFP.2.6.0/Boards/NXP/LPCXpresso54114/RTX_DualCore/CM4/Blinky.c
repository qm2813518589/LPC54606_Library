/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Dual Core LED Flasher for LPC54114-LPCXpresso  (M4 part)
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

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "chip.h"                       // Device header
#include "Board_LED.h"

#define M0plus_IMAGE_START    0x20000          /* M0+ Image located in flash  */


osThreadId tid_mainThread;                      /* main thread ID             */

/*----------------------------------------------------------------------------
  MAILBOX functions
 *----------------------------------------------------------------------------*/
static void MAILBOX_mutexRelease(void) {

  LPC_MBOX->MUTEX = 1;                          /* release mutex              */
}

static void MAILBOX_Initialize(void) {

  LPC_SYSCON->AHBCLKCTRL[0]    |= (1ul << 26);  /* enable clock for Mailbox   */
  LPC_SYSCON->PRESETCTRLSET[0]  = (1ul << 26);  /* assert reset for Mailbox   */
  LPC_SYSCON->PRESETCTRLCLR[0]  = (1ul << 26);  /* deassert reset for Mailbox */

  MAILBOX_mutexRelease();
}


/*----------------------------------------------------------------------------
  MAILBOX_IRQHandler
 *----------------------------------------------------------------------------*/
void MAILBOX_IRQHandler(void) {
  uint32_t mbxVal;

  mbxVal = LPC_MBOX->BOX[1].IRQ;                /* get mailbox event          */
  LPC_MBOX->BOX[1].IRQCLR = 0xFFFFFFFF;         /* clear own mailbox          */
  switch (mbxVal & 0x0000F000) {
    case 0x1000:                                /* START action is ack'ed     */
       osSignalSet(tid_mainThread, (int32_t)0x1000);
      break;
  }

}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {
  volatile uint32_t stopHereForM0plusDebugging = 0;

  osEvent evt;
  uint32_t ledMax = LED_GetCount();
  uint32_t ledState = 1;
  uint32_t i;

  tid_mainThread = osThreadGetId ();          /* get the own thread ID        */

  LED_Initialize();                           /* initialize the LEDs          */

  MAILBOX_Initialize();                       /* initialize mailbox (M4 only) */
  NVIC_EnableIRQ(MAILBOX_IRQn);               /* enable mailbox interrupt     */

  /* Boot M0+ core */
  /* Setup M0+ stack pointer and  reset vector ( from M0+ flash image) */
  Chip_CPU_CM0Boot((uint32_t *)(*(uint32_t *)(M0plus_IMAGE_START + 4)),
                   (uint32_t *)(*(uint32_t *)(M0plus_IMAGE_START + 0)) );

  stopHereForM0plusDebugging = 1;

  LPC_MBOX->BOX[0].IRQ = 0x1000;              /* perform a START action       */
  evt = osSignalWait(0x1000, osWaitForever);  /* wait for acknowledge         */
  if ((evt.status == osEventSignal)    &&
      (evt.value.signals == 0x1000)  ) {
    for (i = 0; i < 3; i++) {                 /* M0+ is up and running        */
        LED_On (0);
        osDelay(100);
        LED_Off(0);
        osDelay(100);
    }
  }

  while (1) {

    LPC_MBOX->BOX[0].IRQ = 0x2000  |          /* perform a LED action         */
                           ledState;          /* signal ledState to M0+       */

    osDelay(500);                             /* wait 500 ms                  */

    ledState = ledState << 1;                 /* switch to next LED           */
    if (ledState == (1 << ledMax)) {
      ledState = 1;
    }
  }

}
