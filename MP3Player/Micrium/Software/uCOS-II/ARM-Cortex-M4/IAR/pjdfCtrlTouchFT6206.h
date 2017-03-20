/*
    pjdfCtrlLcdILI9341.h
    LCD control definitions exposed to applications

    Developed for University of Washington embedded systems programming certificate
    
    2016/2 Nick Strathy wrote/arranged it after a framework by Paul Lever
*/

#ifndef __PJDFCTRLTOUCHFT6206_H__
#define __PJDFCTRLTOUCHFT6206_H__

// Control definitions for VS1053 MP3 device driver

#define PJDF_CTRL_TOUCH_REG 0x00
#define PJDF_CTRL_NUM_TOUCHES_REG 0x01

/*
// Selecting one of the the following two deselects the other one:
#define PJDF_CTRL_TOUCH_SELECT_DATA  0x01   // Assert data chip-select when operating SPI link to ILI9341
#define PJDF_CTRL_TOUCH_SELECT_COMMAND  0x02   // Assert command-chip select when operating the SPI linke ILI9341

#define PJDF_CTRL_TOUCH_SET_I2C_HANDLE 0x03
*/


#endif