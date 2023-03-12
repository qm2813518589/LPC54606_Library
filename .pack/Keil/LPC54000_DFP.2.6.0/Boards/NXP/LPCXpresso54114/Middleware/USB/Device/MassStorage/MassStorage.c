/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    MassStorage.c
 * Purpose: USB Device Mass Storage Example
 *----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "rl_usb.h"

int main (void) {

  USBD_Initialize    (0);               /* USB Device 0 Initialization        */
  USBD_Connect       (0);               /* USB Device 0 Connect               */

  while (1) {
    osDelay(1000);
  }
}
