/************************************************************************************

Copyright (c) 2001-2016  University of Washington Extension.

Module Name:

tasks.c

Module Description:

The tasks that are executed by the test application.

2016/2 Nick Strathy adapted it for NUCLEO-F401RE 

************************************************************************************/
#include <stdarg.h>

#include "bsp.h"
#include "print.h"
#include "mp3Util.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#include "train_crossing.h"
#include "rwby.h"

Adafruit_ILI9341 lcdCtrl = Adafruit_ILI9341(); // The LCD controller

Adafruit_FT6206 touchCtrl = Adafruit_FT6206(); // The touch controller

Adafruit_GFX_Button playControlButton = Adafruit_GFX_Button(); // Used for play and stop button
Adafruit_GFX_Button pauseControlButton = Adafruit_GFX_Button(); // Used for pause button
Adafruit_GFX_Button nextControlButton = Adafruit_GFX_Button();

#define PENRADIUS 3

long MapTouchToScreen(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define BUFSIZE 256

/************************************************************************************

Allocate the stacks for each task.
The maximum number of tasks the application can have is defined by OS_MAX_TASKS in os_cfg.h

************************************************************************************/

static OS_STK   LcdTouchDemoTaskStk[APP_CFG_TASK_START_STK_SIZE];
static OS_STK   Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE];

HANDLE hSPI = 0;
HANDLE hMp3 = 0;


// Task prototypes
void LcdTouchDemoTask(void* pdata);
void Mp3DemoTask(void* pdata);

#define CURRENTACTIONQUEUEMAXENTRIES 2 // [play status, song title] possibly % mp3 played
OS_EVENT * currentActionQueue; // Use a queue for our status
void * currentActionQueuePointers[CURRENTACTIONQUEUEMAXENTRIES];

// Useful functions
void PrintToLcdWithBuf(char *buf, int size, char *format, ...);

// Globals
BOOLEAN nextSong = OS_FALSE;

// TODO: Determine why using a mailbox to pass this info was super slow.
int8_t currentSong = 0;
#define MAXSONGS 2
char songList[][20] = {"RWBY", "Train Crossing"};

/************************************************************************************

This task is the initial task running, started by main(). It starts
the system tick timer and creates all the other tasks. Then it deletes itself.

************************************************************************************/
void StartupTask(void* pdata)
{
    char buf[BUFSIZE];
    
    PjdfErrCode pjdfErr;
    INT32U length;
    //static HANDLE hSD = 0;
    
    INT8U err;
    
    PrintWithBuf(buf, BUFSIZE, "StartupTask: Begin\n");
    PrintWithBuf(buf, BUFSIZE, "StartupTask: Starting timer tick\n");
    
    // Start the system tick
    OS_CPU_SysTickInit(OS_TICKS_PER_SEC);
    
    // Set up our message queue
    PrintWithBuf(buf, BUFSIZE, "StartupTask: Setting up play status message queue\n");
    currentActionQueue = OSQCreate(currentActionQueuePointers, CURRENTACTIONQUEUEMAXENTRIES); 
    
    PrintWithBuf(buf, BUFSIZE, "Opening MP3 driver: %s\n", PJDF_DEVICE_ID_MP3_VS1053);
    // Open handle to the MP3 decoder driver
    hMp3 = Open(PJDF_DEVICE_ID_MP3_VS1053, 0);
    if (!PJDF_IS_VALID_HANDLE(hMp3)) while(1);
    
    PrintWithBuf(buf, BUFSIZE, "Opening MP3 SPI driver: %s\n", MP3_SPI_DEVICE_ID);
    // We talk to the MP3 decoder over a SPI interface therefore
    // open an instance of that SPI driver and pass the handle to 
    // the MP3 driver.
    hSPI = Open(MP3_SPI_DEVICE_ID, 0);
    if (!PJDF_IS_VALID_HANDLE(hSPI)) while(1);
    
    length = sizeof(HANDLE);
    pjdfErr = Ioctl(hMp3, PJDF_CTRL_MP3_SET_SPI_HANDLE, &hSPI, &length);
    if(PJDF_IS_ERROR(pjdfErr)) while(1);
    // Send initialization data to the MP3 decoder and run a test
    PrintWithBuf(buf, BUFSIZE, "Starting MP3 device test\n");
    Mp3Init(hMp3);
    
    
    /*
    Turning off for now
    // Initialize SD card
    PrintWithBuf(buf, PRINTBUFMAX, "Opening handle to SD driver: %s\n", PJDF_DEVICE_ID_SD_ADAFRUIT);
    hSD = Open(PJDF_DEVICE_ID_SD_ADAFRUIT, 0);
    if (!PJDF_IS_VALID_HANDLE(hSD)) while(1);
    
    
    PrintWithBuf(buf, PRINTBUFMAX, "Opening SD SPI driver: %s\n", SD_SPI_DEVICE_ID);
    // We talk to the SD controller over a SPI interface therefore
    // open an instance of that SPI driver and pass the handle to 
    // the SD driver.
    hSPI = Open(SD_SPI_DEVICE_ID, 0);
    if (!PJDF_IS_VALID_HANDLE(hSPI)) while(1);
    
    length = sizeof(HANDLE);
    pjdfErr = Ioctl(hSD, PJDF_CTRL_SD_SET_SPI_HANDLE, &hSPI, &length);
    if(PJDF_IS_ERROR(pjdfErr)) while(1);
    */
    // Create the test tasks
    PrintWithBuf(buf, BUFSIZE, "StartupTask: Creating the application tasks\n");
    
    
    
    // The maximum number of tasks the application can have is defined by OS_MAX_TASKS in os_cfg.h
    //OSTaskCreate(Mp3DemoTask, (void*)0, &Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST1_PRIO);
    OSTaskCreate(LcdTouchDemoTask, (void*)0, &LcdTouchDemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST2_PRIO);
    
    // Delete ourselves, letting the work be done in the new tasks.
    PrintWithBuf(buf, BUFSIZE, "StartupTask: deleting self\n");
    OSTaskDel(OS_PRIO_SELF);
}

static void DrawLcdContents()
{
    INT8U err;
    int8_t currentAction;
    char actionText[20];
    char *songTitle;
    currentAction = *(int8_t *)OSQPend(currentActionQueue, 0, &err);
    songTitle = (char *)OSQPend(currentActionQueue, 0, &err);
    char buf[BUFSIZE];
    switch(currentAction) {
    case 0: // Currently stopped
        playControlButton.initButton(
                                     &lcdCtrl,
                                     ILI9341_TFTWIDTH-120, // x-center
                                     ILI9341_TFTHEIGHT-160, // y center of button
                                     60, // Width
                                     40, // Height
                                     ILI9341_YELLOW, // outline
                                     ILI9341_BLACK, // fill
                                     ILI9341_YELLOW, // text
                                     "Play", // label
                                     1); // text size
        
        // Print a message on the LCD
        // TODO: Individually clear each area that needs updating rather than the whole screen
        /*
        lcdCtrl.fillRect(40, 60, 200, 40, ILI9341_BLACK); // Clear the text area
        lcdCtrl.fillRect(40, 60, ILI9341_TFTWIDTH-130, ILI9341_TFTHEIGHT-120, ILI9341_BLACK); // Clear the pause button
        */
        
        lcdCtrl.fillScreen(ILI9341_BLACK);
        playControlButton.drawButton();
        strcpy(actionText, "Stopped");
        lcdCtrl.setCursor(40, 60);
        lcdCtrl.setTextColor(ILI9341_WHITE);  
        lcdCtrl.setTextSize(2);
        PrintToLcdWithBuf(buf, BUFSIZE, actionText);
        lcdCtrl.setCursor(40, 80);
        PrintToLcdWithBuf(buf, BUFSIZE, songTitle);
        
        break;
    case 1: // Currently playing
        lcdCtrl.fillScreen(ILI9341_BLACK);
        playControlButton.initButton(
                                     &lcdCtrl,
                                     ILI9341_TFTWIDTH-120, // x-center
                                     ILI9341_TFTHEIGHT-160, // y center of button
                                     60, // Width
                                     40, // Height
                                     ILI9341_YELLOW, // outline
                                     ILI9341_BLACK, // fill
                                     ILI9341_YELLOW, // text
                                     "Stop", // label
                                     1); // text size
        pauseControlButton.initButton(
                                      &lcdCtrl,
                                      ILI9341_TFTWIDTH-120, // x-center
                                      ILI9341_TFTHEIGHT-120, // y center of button
                                      60, // Width
                                      40, // Height
                                      ILI9341_YELLOW, // outline
                                      ILI9341_BLACK, // fill
                                      ILI9341_YELLOW, // text
                                      "Pause", // label
                                      1); // text size
        nextControlButton.initButton(
                                     &lcdCtrl,
                                     ILI9341_TFTWIDTH-120, // x-center
                                     ILI9341_TFTHEIGHT-80, // y center of button
                                     60, // Width
                                     40, // Height
                                     ILI9341_YELLOW, // outline
                                     ILI9341_BLACK, // fill
                                     ILI9341_YELLOW, // text
                                     "Next", // label
                                     1); // text size
        
        // Print a message on the LCD
        //lcdCtrl.fillRect(40, 60, 200, 40, ILI9341_BLACK); // Clear the text area
        strcpy(actionText, "Playing");
        lcdCtrl.setCursor(40, 60);
        lcdCtrl.setTextColor(ILI9341_WHITE);  
        lcdCtrl.setTextSize(2);
        PrintToLcdWithBuf(buf, BUFSIZE, actionText);
        lcdCtrl.setCursor(40, 80);
        PrintToLcdWithBuf(buf, BUFSIZE, songTitle);
        playControlButton.drawButton();
        pauseControlButton.drawButton();
        nextControlButton.drawButton();
        break;
    case 2: // Pause
        lcdCtrl.fillScreen(ILI9341_BLACK);
        playControlButton.initButton(
                                     &lcdCtrl,
                                     ILI9341_TFTWIDTH-120, // x-center
                                     ILI9341_TFTHEIGHT-160, // y center of button
                                     60, // Width
                                     40, // Height
                                     ILI9341_YELLOW, // outline
                                     ILI9341_BLACK, // fill
                                     ILI9341_YELLOW, // text
                                     "Stop", // label
                                     1); // text size
        
        // Print a message on the LCD
        lcdCtrl.fillRect(40, 60, 200, 40, ILI9341_BLACK); // Clear the text area
        lcdCtrl.setCursor(40, 60);
        strcpy(actionText, "Paused");
        lcdCtrl.setCursor(40, 60);
        lcdCtrl.setTextColor(ILI9341_WHITE);  
        lcdCtrl.setTextSize(2);
        PrintToLcdWithBuf(buf, BUFSIZE, actionText);
        lcdCtrl.setCursor(40, 80);
        PrintToLcdWithBuf(buf, BUFSIZE, songTitle);
        pauseControlButton.initButton(
                                      &lcdCtrl,
                                      ILI9341_TFTWIDTH-120, // x-center
                                      ILI9341_TFTHEIGHT-120, // y center of button
                                      60, // Width
                                      40, // Height
                                      ILI9341_YELLOW, // outline
                                      ILI9341_BLACK, // fill
                                      ILI9341_YELLOW, // text
                                      "Resume", // label
                                      1); // text size
        nextControlButton.initButton(
                                     &lcdCtrl,
                                     ILI9341_TFTWIDTH-120, // x-center
                                     ILI9341_TFTHEIGHT-80, // y center of button
                                     60, // Width
                                     40, // Height
                                     ILI9341_YELLOW, // outline
                                     ILI9341_BLACK, // fill
                                     ILI9341_YELLOW, // text
                                     "Next", // label
                                     1); // text size
        playControlButton.drawButton();
        pauseControlButton.drawButton();
        nextControlButton.drawButton();
        break;
    default:
        lcdCtrl.fillScreen(ILI9341_BLACK);
        lcdCtrl.setCursor(40, 60);
        lcdCtrl.setTextColor(ILI9341_WHITE);  
        lcdCtrl.setTextSize(2);
        PrintToLcdWithBuf(buf, BUFSIZE, "Invalid state!");
        playControlButton.drawButton();
        
    }
    
}

/************************************************************************************

Runs LCD/Touch demo code

************************************************************************************/
void LcdTouchDemoTask(void* pdata)
{
    PjdfErrCode pjdfErr;
    INT32U length;
    INT8U err;
    
    int8_t currentAction;
    boolean pauseButtonActive = false;
    boolean playStatus = false;
    

    
    char buf[BUFSIZE];
    PrintWithBuf(buf, BUFSIZE, "LcdTouchDemoTask: starting\n");
    
    PrintWithBuf(buf, BUFSIZE, "Opening LCD driver: %s\n", PJDF_DEVICE_ID_LCD_ILI9341);
    // Open handle to the LCD driver
    HANDLE hLcd = Open(PJDF_DEVICE_ID_LCD_ILI9341, 0);
    if (!PJDF_IS_VALID_HANDLE(hLcd)) while(1);
    
    PrintWithBuf(buf, BUFSIZE, "Opening LCD SPI driver: %s\n", LCD_SPI_DEVICE_ID);
    // We talk to the LCD controller over a SPI interface therefore
    // open an instance of that SPI driver and pass the handle to 
    // the LCD driver.
    HANDLE hSPI = Open(LCD_SPI_DEVICE_ID, 0);
    if (!PJDF_IS_VALID_HANDLE(hSPI)) while(1);
    
    length = sizeof(HANDLE);
    pjdfErr = Ioctl(hLcd, PJDF_CTRL_LCD_SET_SPI_HANDLE, &hSPI, &length);
    if(PJDF_IS_ERROR(pjdfErr)) while(1);
    
    PrintWithBuf(buf, BUFSIZE, "Initializing LCD controller\n");
    lcdCtrl.setPjdfHandle(hLcd);
    lcdCtrl.begin();
    
    currentAction = 0;
    char *songTitle = "Press play!";
    err = OSQPost(currentActionQueue, &currentAction);
    err = OSQPost(currentActionQueue, songTitle);
    
    DrawLcdContents();
    
    PrintWithBuf(buf, BUFSIZE, "Initializing FT6206 touchscreen controller\n");
    if (! touchCtrl.begin(40)) {  // pass in 'sensitivity' coefficient
        PrintWithBuf(buf, BUFSIZE, "Couldn't start FT6206 touchscreen controller\n");
        while (1);
    }
    
    int currentcolor = ILI9341_RED;
    
    while (1) { 
        boolean touched = false;
        
        // TODO: Poll for a touch on the touch panel
        OS_CPU_SR  cpu_sr;
        OS_ENTER_CRITICAL();   
        touched = touchCtrl.touched();
        OS_EXIT_CRITICAL();
        if (! touched) {
            OSTimeDly(50);
            continue;
        }
        
        TS_Point rawPoint;
        
        // TODO: Retrieve a point  
        OS_ENTER_CRITICAL();   
        rawPoint = touchCtrl.getPoint();
        OS_EXIT_CRITICAL();
        if (rawPoint.x == 0 && rawPoint.y == 0)
        {
            continue; // usually spurious, so ignore
        }
        
        // transform touch orientation to screen orientation.
        TS_Point p = TS_Point();
        p.x = MapTouchToScreen(rawPoint.x, 0, ILI9341_TFTWIDTH, ILI9341_TFTWIDTH, 0);
        p.y = MapTouchToScreen(rawPoint.y, 0, ILI9341_TFTHEIGHT, ILI9341_TFTHEIGHT, 0);
        
        //lcdCtrl.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
        if (playControlButton.contains(p.x, p.y) == true) {
            // The play / stop button has been pressed
            //lcdCtrl.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
            length = BspMp3DeactLen;
            //OSTimeDly(10);
            playStatus = !playStatus;
            switch(playStatus) {
            case 0:
                currentAction = 0;
                pauseButtonActive = false;
                songTitle = songList[currentSong];
                OSTaskDel(APP_TASK_TEST1_PRIO);
                break;
            case 1:
                currentAction = 1;
                pauseButtonActive = true;
                songTitle = songList[currentSong];
                OSTaskCreate(Mp3DemoTask, (void*)0, &Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST1_PRIO);
                break;
            default:
                pauseButtonActive = false;
                currentAction = -1;
            }
            err = OSQPost(currentActionQueue, &currentAction);
            err = OSQPost(currentActionQueue, songTitle);
            DrawLcdContents();
        }
        if (pauseControlButton.contains(p.x, p.y) == true && currentAction > 0) {
            // The play / stop button has been pressed
            lcdCtrl.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
            length = BspMp3DeactLen;
            //OSTimeDly(10);
            pauseButtonActive = !pauseButtonActive;
            switch(pauseButtonActive) {
            case 0:
                currentAction = 2;
                songTitle = songList[currentSong];
                OSTaskSuspend(APP_TASK_TEST1_PRIO);
                break;
            case 1:
                currentAction = 1;
                songTitle = songList[currentSong];
                OSTaskResume(APP_TASK_TEST1_PRIO);
                break;
            default:
                currentAction = -1;
            }
            err = OSQPost(currentActionQueue, &currentAction);
            err = OSQPost(currentActionQueue, songTitle);
            DrawLcdContents();
        }
        if (nextControlButton.contains(p.x, p.y) && currentAction > 0) {
            currentSong += 1;
            if (currentSong == MAXSONGS)
                currentSong = 0;
            songTitle = songList[currentSong];
            err = OSQPost(currentActionQueue, &currentAction);
            err = OSQPost(currentActionQueue, songTitle);
            OSTaskDel(APP_TASK_TEST1_PRIO);
            OSTaskCreate(Mp3DemoTask, (void*)0, &Mp3DemoTaskStk[APP_CFG_TASK_START_STK_SIZE-1], APP_TASK_TEST1_PRIO);
            DrawLcdContents();
        }
    }
}
/************************************************************************************

Runs MP3 demo code

************************************************************************************/
void Mp3DemoTask(void* pdata)
{
    char buf[BUFSIZE];
    PrintWithBuf(buf, BUFSIZE, "Mp3DemoTask: starting\n");
    int count = 0;
    
    while (1)
    {
        OSTimeDly(10);
        PrintWithBuf(buf, BUFSIZE, "Begin streaming sound file  count=%d\n", ++count);
        // TODO: This is a poor design, needs to be cleaned up
        switch(currentSong) {
        case 0:
            Mp3Stream(hMp3, (INT8U*)RWBY, sizeof(RWBY)); 
            break;
        case 1:
            Mp3Stream(hMp3, (INT8U*)Train_Crossing, sizeof(Train_Crossing));
            break;
        }
        PrintWithBuf(buf, BUFSIZE, "Done streaming sound file  count=%d\n", count);
    }
}


// Renders a character at the current cursor position on the LCD
static void PrintCharToLcd(char c)
{
    lcdCtrl.write(c);
}

/************************************************************************************

Print a formated string with the given buffer to LCD.
Each task should use its own buffer to prevent data corruption.

************************************************************************************/
void PrintToLcdWithBuf(char *buf, int size, char *format, ...)
{
    va_list args;
    va_start(args, format);
    PrintToDeviceWithBuf(PrintCharToLcd, buf, size, format, args);
    va_end(args);
}




