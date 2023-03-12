/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.42 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCD_X.c
Purpose     : Port routines for LPC5411x SPI
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************
*/

#include "chip.h"

#include "Driver_SPI.h"


/* SPI Interface: SPI5 (Flexcomm Interface 5)
 
   PINS: 
   - CS  = P1.1  (FC5_RTS_SCL_SSEL2)
   - SCK = P0.19 (FC5_SCK)
   - SDO = P0.18 (FC5_TXD_SCL_MISO)
   - SDI = P0.20 (FC5_RXD_SDA_MOSI)
*/

/* SPI Driver */
extern ARM_DRIVER_SPI         Driver_SPI5;
#define ptrSPI              (&Driver_SPI5)


/* Input/output pin description */
typedef struct io_pin {
  uint8_t port;               // Port number
  uint8_t pin;                // Pin number
} IO_PIN;

/* IOCON settings for GPIO pin */
#define PIN_IOCON_GPIO       (IOCON_FUNC0      | \
                              IOCON_GPIO_MODE  | \
                              IOCON_DIGITAL_EN | \
                              IOCON_INPFILT_OFF)

/* Backlight pin definition */
static const IO_PIN lcd_bl = { 0, 29 };


/*********************************************************************
*
*       Exported code
*
*********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values
*/
void LCD_X_Init(void) {
  /* Initialize and configure SPI */
  ptrSPI->Initialize  (NULL);
  ptrSPI->PowerControl(ARM_POWER_FULL);
  ptrSPI->Control     (ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);

  /* LCD Backlight pin configuration */
  Chip_IOCON_PinMuxSet (LPC_IOCON, lcd_bl.port, lcd_bl.pin, PIN_IOCON_GPIO);
  Chip_GPIO_SetPinDIROutput (LPC_GPIO, lcd_bl.port, lcd_bl.pin);
  /* Turn backlight on */
  Chip_GPIO_SetPinState (LPC_GPIO, lcd_bl.port, lcd_bl.pin, true);
}

/*********************************************************************
*
*       LCD_X_ClrCS
*
* Purpose:
*   Sets the chip select pin to low
*/
void LCD_X_ClrCS(void) {
  ptrSPI->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
}

/*********************************************************************
*
*       LCD_X_SetCS
*
* Purpose:
*   Sets the chip select pin to high
*/
void LCD_X_SetCS(void) {
  ptrSPI->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}

/*********************************************************************
*
*       LCD_X_WriteM
*
* Purpose:
*   Writes multiple bytes to controller
*/
void LCD_X_WriteM(U8 * pData, int NumBytes) {
  ptrSPI->Send(pData, NumBytes);
  while (ptrSPI->GetStatus().busy);
}

/*********************************************************************
*
*       LCD_X_ReadM
*
* Purpose:
*   Reads multiple bytes from the controller
*/
void LCD_X_ReadM(U8 * pData, int NumBytes) {
  ptrSPI->Receive(pData, NumBytes);
  while (ptrSPI->GetStatus().busy);
}

/*************************** End of file ****************************/
