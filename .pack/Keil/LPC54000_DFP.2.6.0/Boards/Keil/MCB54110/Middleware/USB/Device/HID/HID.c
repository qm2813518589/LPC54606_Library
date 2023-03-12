/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HID.c
 * Purpose: USB Device Human Interface Device example program
 *----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "rl_usb.h"
#include "Board_Joystick.h"
#include "Board_LED.h"

int main (void) {
  uint8_t joy, last_joy = 0;

  LED_Initialize     ();
  Joystick_Initialize();

  USBD_Initialize    (0);               /* USB Device 0 Initialization        */
  USBD_Connect       (0);               /* USB Device 0 Connect               */

  while (1) {                           /* Loop forever                       */
    joy = (uint8_t)(Joystick_GetState ());
    if (joy != last_joy) {
      last_joy = joy;
      USBD_HID_GetReportTrigger(0, 0, &joy, 1);
    }
    osDelay(100);                       /* 100 ms delay for sampling buttons  */
  }
}
