/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2016 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/


#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "LPC54608.h"                   // Device header
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons


/*----------------------------------------------------------------------------
 * blinkLED: blink LED and check button state
 *----------------------------------------------------------------------------*/
void blinkLED(void const *argument) {
  int32_t max_num = LED_GetCount();
  int32_t num = 0;

  for (;;) {
    LED_On(num);                                           /* Turn specified LED on */
    osSignalWait(0x0001, osWaitForever);
    LED_Off(num);                                          /* Turn specified LED off */
    osSignalWait(0x0001, osWaitForever);

    num++;                                                 /* Change LED number */
    if (num >= max_num) {
      num = 0;                                             /* Restart with first LED */
    }
  }

}

osThreadId tid_blinkLED;
osThreadDef (blinkLED, osPriorityNormal, 1, 0);


/* FRO Clock, 96MHz */
void SystemCoreClockConfigure (void)
{
	/* Power up the FRO and set this as the base clock */
	SYSCON->PDRUNCFGCLR[0] = (1UL << 4);

		/* Use 12MHz FRO as the default base clock */
	SYSCON->MAINCLKSELA = 0U;
	SYSCON->MAINCLKSELB = 0U;

	SYSCON->FLASHCFG = (SYSCON->FLASHCFG & ~(0xF << 12)) | (4U << 12);

	/* ASYSNC SYSCON needs to be on or all serial peripheral won't work.
	   Be careful if PLL is used or not, ASYNC_SYSCON source needs to be
	   selected carefully. */
  SYSCON->ASYNCAPBCTRL = 0x01U;
	ASYNC_SYSCON->ASYNCAPBCLKSELA = 0U;

  SYSCON->FROCTRL = SYSCON->FROCTRL | SYSCON_FROCTRL_WRTRIM(1) | SYSCON_FROCTRL_HSPDCLK(1) | SYSCON_FROCTRL_SEL(1);

	SYSCON->MAINCLKSELA = 3U;
	SYSCON->MAINCLKSELB = 0U;


}



/*----------------------------------------------------------------------------
 * main: initialize and start the system
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t button_msk = (1U << Buttons_GetCount()) - 1;

  osKernelInitialize ();                                   /* initialize CMSIS-RTOS */

  SystemCoreClockConfigure();
  SystemCoreClockUpdate();

  /* initialize peripherals */
  LED_Initialize();                                        /* LED Initialization */
  Buttons_Initialize();                                    /* Buttons Initialization */

  /* create threads */
  tid_blinkLED = osThreadCreate (osThread(blinkLED), NULL);

  osKernelStart ();                                        /* start thread execution */

  for (;;) {                                               /* main must not be terminated! */
    osDelay(500);
    while (Buttons_GetState() & (button_msk));             /* Wait while holding USER button */
    osSignalSet(tid_blinkLED, 0x0001);
  }
}
