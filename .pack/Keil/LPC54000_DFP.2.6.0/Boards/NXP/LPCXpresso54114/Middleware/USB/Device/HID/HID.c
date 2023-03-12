/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "rl_usb.h"
#include "Board_Buttons.h"
#include "Board_LED.h"

int main (void) {
  uint8_t but, last_but = 0;

  LED_Initialize    ();
  Buttons_Initialize();

  USBD_Initialize    (0);               /* USB Device 0 Initialization        */
  USBD_Connect       (0);               /* USB Device 0 Connect               */

  while (1) {                           /* Loop forever                       */
    but = (uint8_t)(Buttons_GetState ());
    if (but != last_but) {
      last_but = but;
      USBD_HID_GetReportTrigger(0, 0, &but, 1);
    }
    osDelay(100);                       /* 100 ms delay for sampling buttons  */
  }
}
