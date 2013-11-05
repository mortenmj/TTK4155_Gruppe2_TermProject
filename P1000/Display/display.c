/*
 * display.c
 *
 * Created: 02.10.2013 23:45:09
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "task.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "u8glib/u8g.h"
#include "display.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 200 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

u8g_t u8g;
struct xInputState xInput;

static unsigned char ucGetButtons ( void )
{
	/* Mask out LEDs from the port */
	return SWPORT & ((1 << SW0) | (1 << SW1) | (1 << SW2) | (1 << SW3));
}

static void vDebounceButtons ( void )
{
	unsigned char n;
	
	xInput.ucState = ucGetButtons ();
	++xInput.ucIndex;
	n = 0xFF;
	
	for ( unsigned char i = 0; i < dispMAX_CHECKS; i++ )
	{
		n &= xInput.ucValues[i];
	}
	xInput.ucState ^= n;
	
	if ( xInput.ucIndex >= dispMAX_CHECKS )
	{
		xInput.ucIndex = 0;
	}
}

/* Constants for writing to TCCR0A */
#define dispTIMER_ENABLE_CTC				( ( unsigned char ) ( 1 << WGM01 ) )

/* Constants for writing to TCCRnB. */
#define dispTIMER_64_PRESCALE				( ( unsigned char ) ( ( 1 << CS01 ) | ( 1 << CS00 ) ) )

/* Constants for writing to TIMSK0 */
#define dispTIMER_ENABLE_COMPA				( ( unsigned char ) ( 1 << OCIE0A ) )

void vDisplay ( void *pvParameters )
{
	portTickType xLastWakeTime;
	struct xInputState xInput = { 0, {{0}}, 0 };
	xLastWakeTime = xTaskGetTickCount ();
	
	menu_init ();
	
	portENTER_CRITICAL();
	{
		/* Initialize timer to debounce input buttons */
		TCCR0A |= dispTIMER_ENABLE_CTC;
		TCCR0B |= dispTIMER_64_PRESCALE;
		TIMSK0 |= dispTIMER_ENABLE_COMPA;
		
		/* Compare at 250 gives a timer freq of 1Khz */
		OCR0A = 250;
	}
	portEXIT_CRITICAL();		
	
	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );
	
	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, dispTASK_FREQUENCY );
		
		vDebounceButtons ();
		
		u8g_FirstPage(&u8g);
		do
		{
			menu_display (&u8g);
		} while ( u8g_NextPage( &u8g ) );
	}
}

ISR (TIMER0_OVF_vect)
{
	//vDebounceButtons ();
	vSerialPutString(NULL, "isr\n", 4);
}
