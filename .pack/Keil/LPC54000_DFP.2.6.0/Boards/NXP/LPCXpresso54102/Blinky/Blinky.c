/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 - 2016 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "chip.h"                       // Device header
#include "Board_LED.h"                  // ::Board Support:LED


uint32_t LEDOn, LEDOff;
uint32_t ticks_1s;                      /* Flag activated each second         */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;
  static uint32_t ticks_10ms;

  if (ticks_10ms++ >= 99) {             /* Set Clock1s to 1 every second      */
    ticks_10ms = 0;
    ticks_1s   = 1;
  }

  switch (ticks++) {
    case  0: LEDOn  = 1; break;
    case 40: LEDOff = 1; break;
    case 45: ticks  = 0; break;

    default:
      if (ticks > 50) {
        ticks = 0;
      }
  }

}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ledMax = LED_GetCount() - 1;
  uint32_t ledNum = 0;

  SystemCoreClockUpdate();

  LED_Initialize();                            /* LED Initialization          */

  SysTick_Config(SystemCoreClock / 100);       /* Systick interrupt each 10ms */

  while(1) {                                   /* Loop forever                */

    if (LEDOn) {
      LEDOn = 0;
      LED_On (ledNum);                         /* Turn specified LED on       */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (ledNum);                        /* Turn specified LED off      */

      if (ledNum++ == ledMax) {
        ledNum = 0;
      }
    }

    if (ticks_1s) {
      ticks_1s = 0;
      printf("Hello World.\r\n");
    }
  }

}
