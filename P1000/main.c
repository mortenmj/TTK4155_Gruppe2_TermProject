#include <stdlib.h>
#include <string.h>
#include <avr/io.h>

#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h>
#endif

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

#include "serial.h"
#include "spi.h"

/* Includes for our tasks */
#include "Control/control.h"
#include "Display/display.h"

/* Priority definitions for our tasks. */
#define mainCONTROL_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainDISPLAY_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* Baud rate used by the serial port tasks. */
#define mainCOM_BAUD_RATE				( ( unsigned long ) 28800 )

/* Task frequencies */
#define mainJOYSTICK_TASK_FREQUENCY		( ( const portTickType ) 1000 )

/* LED used by the serial port tasks.  This is toggled on each character Tx,
and mainCOM_TEST_LED + 1 is toggles on each character Rx. */
#define mainCOM_TEST_LED				( 4 )

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

/* The number of coroutines to create. */
#define mainNUM_FLASH_COROUTINES		( 3 )

/*
 * The idle hook is used to scheduler co-routines.
 */
void vApplicationIdleHook ( void );

/*
* The stack overflow hook is used to inspect stack overflow errors
*/
void vApplicationStackOverflowHook ( xTaskHandle xTask, signed portCHAR *pcTaskName );

/*
* The tick hook is called on every tick
*/
void vApplicationTickHook ( void );

/*-----------------------------------------------------------*/

int main( void )
{
	/* SPI init */
	spi_init ();

	/* UART init */
	xSerialPortInitMinimal ( mainCOM_BAUD_RATE, 100 );
	vSerialPutString ( NULL, (signed char *) "init\n", 5 );

	/* Create tasks */
    xTaskHandle disp_handle;
    xTaskHandle ctrl_handle;

	xTaskCreate ( vDisplay, (signed char * ) "Display", configDISPLAY_STACK_SIZE, NULL, mainDISPLAY_TASK_PRIORITY, &disp_handle );
    vDisplaySetHandle (disp_handle);

	xTaskCreate ( vControl, (signed char * ) "Control", configCONTROL_STACK_SIZE, NULL, mainCONTROL_TASK_PRIORITY, &ctrl_handle );
    vControlSetHandle (ctrl_handle);

	/* Start scheduler */
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{

}

void vApplicationStackOverflowHook ( xTaskHandle xTask, signed portCHAR *pcTaskName )
{
	vSerialPutString ( NULL, (signed char *) "overflow\n", 9);
}

void vApplicationTickHook ( void )
{

}