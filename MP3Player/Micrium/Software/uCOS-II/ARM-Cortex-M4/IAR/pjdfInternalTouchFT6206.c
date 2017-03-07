/*
    pjdfInternalMp3Vs1053.c
    The implementation of the internal PJDF interface pjdfInternal.h targeted for the
    VS1053 MP3 decoder.

    Developed for University of Washington embedded systems programming certificate
    
    2016/2 Nick Strathy wrote/arranged it after a framework by Paul Lever
*/

#include <Adafruit_FT6206.h>
//#include "bsp.h"
#include "pjdf.h"
#include "pjdfInternal.h"
#include "bspI2c.h"


/*
// SPI link, etc for VS1053 MP3 decoder hardware
typedef struct _PjdfContextMp3VS1053
{
    HANDLE spiHandle; // SPI communication link to VS1053
    INT8U chipSelect; // 0 means command, 1 means data
} PjdfContextMp3VS1053;

static PjdfContextMp3VS1053 mp3VS1053Context = { 0 };

static const INT16U Mp3SpiDataRate = MP3_SPI_DATARATE;
static const INT32U SizeofMp3SpiDataRate = sizeof(Mp3SpiDataRate);
*/

// OpenTouch
// Nothing to do.
static PjdfErrCode OpenTouch(DriverInternal *pDriver, INT8U flags)
{
    return PJDF_ERR_NONE; 
}

// CloseTouch
// First Version do nothing
static PjdfErrCode CloseTouch(DriverInternal *pDriver)
{
    return PJDF_ERR_NONE;
}

// ReadTouch
// Writes the contents of the buffer to the given device, and concurrently
// gets the resulting data back from the device via I2C. 
//
// pDriver: pointer to an initialized FT6206 driver
// pBuffer: An array to read the data into
// pCount: the length of the array
// Returns: PJDF_ERR_NONE if there was no error, otherwise an error code.
static PjdfErrCode ReadTouch(DriverInternal *pDriver, void* pBuffer, INT32U* pCount)
{
    PjdfErrCode retval;
    uint8_t reg;
    uint32_t bufferSize = *pCount;
    //uint8_t* bufferArray = (uint8_t*)pBuffer;
    
    //PjdfContextMp3VS1053 *pContext = (PjdfContextMp3VS1053*) pDriver->deviceContext;
    //HANDLE hSPI = pContext->spiHandle;
    
    // Currently no need for exclusive lock
    //retval = Ioctl(hSPI, PJDF_CTRL_SPI_WAIT_FOR_LOCK, 0, 0);   // wait for exclusive access
    //if (retval != PJDF_ERR_NONE) while(1);
    
    /*
    switch (pContext->chipSelect) {
    case 0: // send command 
        MP3_VS1053_MCS_ASSERT(); // assert command chip-select
        retval = Read(hSPI, pBuffer, pCount);
        MP3_VS1053_MCS_DEASSERT(); // de-assert command chip-select
        break;
    default:
        while(1); // must be in command mode
    }
    retval = Ioctl(hSPI, PJDF_CTRL_SPI_RELEASE_LOCK, 0, 0);
    if (retval != PJDF_ERR_NONE) while(1);
    */
    
    if (bufferSize == 1) {
      // Assume this is the touched call, need to figure out a better work around.
      // This is similiar to the readregister8
      reg = FT6206_REG_NUMTOUCHES;
    } else {
      reg = 0;
    }
      
    // use i2c
    I2C_start(I2C1, FT6206_ADDR<<1, I2C_Direction_Transmitter); 
    I2C_write(I2C1, (uint8_t)reg);
    I2C_stop(I2C1);
    I2C_start(I2C1, FT6206_ADDR<<1, I2C_Direction_Receiver);
    // Wire.requestFrom((uint8_t)FT6206_ADDR, (uint8_t)1);
    
    uint8_t i;
    if (bufferSize == 1) {
      i = 0;
      ((int8_t *)pBuffer)[i] = I2C_read_nack(I2C1);
    } else {
      for (i=0; i<bufferSize; i++)
        ((int8_t *)pBuffer)[i] = I2C_read_ack(I2C1);
      ((int8_t *)pBuffer)[i] = I2C_read_nack(I2C1);
    }

    retval = PJDF_ERR_NONE;
    return retval;
}


// WriteTouch
// Writes the contents of the buffer to the given device.
//
// pDriver: pointer to an initialized VS1053 MP3 driver
// pBuffer: the data to write to the device
// pCount: the number of bytes to write
// Returns: PJDF_ERR_NONE if there was no error, otherwise an error code.
static PjdfErrCode WriteTouch(DriverInternal *pDriver, void* pBuffer, INT32U* pCount)
{
    // For now keep this simple and focus on the read code.  Write is only used 
    // in the calibration calls
    PjdfErrCode retval;
    retval = PJDF_ERR_NONE;
    return retval;
}

// IoctlMP3
// pDriver: pointer to an initialized VS1053 MP3 driver
// request: a request code chosen from those in pjdfCtrlMp3VS1053.h
// pArgs [in/out]: pointer to any data needed to fulfill the request
// pSize: the number of bytes pointed to by pArgs
// Returns: PJDF_ERR_NONE if there was no error, otherwise an error code.
static PjdfErrCode IoctlTouch(DriverInternal *pDriver, INT8U request, void* pArgs, INT32U* pSize)
{
    // For now keep this simple and focus on the read code. 
    PjdfErrCode retval;
    retval = PJDF_ERR_NONE;
    return retval;
}


// Initializes the given VS1053 MP3 driver.
PjdfErrCode InitTouchFT6206(DriverInternal *pDriver, char *pName)
{
    if (strcmp (pName, pDriver->pName) != 0) while(1); // pName should have been initialized in driversInternal[] declaration
    
    // Initialize semaphore for serializing operations on the device 
    pDriver->sem = OSSemCreate(1); 
    if (pDriver->sem == NULL) while (1);  // not enough semaphores available
    pDriver->refCount = 0; // number of Open handles to the device
    pDriver->maxRefCount = 1; // only one open handle allowed
    //pDriver->deviceContext = &mp3VS1053Context;
    
    BspMp3InitVS1053(); // Initialize related GPIO
  
    // Assign implemented functions to the interface pointers
    pDriver->Open = OpenTouch;
    pDriver->Close = CloseTouch;
    pDriver->Read = ReadTouch;
    pDriver->Write = WriteTouch;
    pDriver->Ioctl = IoctlTouch;
    
    pDriver->initialized = OS_TRUE;
    return PJDF_ERR_NONE;
}
