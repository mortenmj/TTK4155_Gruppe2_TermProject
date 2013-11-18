/*
 * buttons.c
 *
 * Created: 18.11.2013 11:22:15
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "buttons.h"
#include "serial.h"

/* Constants for writing to TCCR0A */
#define dispTIMER_ENABLE_CTC				( ( unsigned char ) ( 1 << WGM01 ) )

/* Constants for writing to TCCRnB. */
#define dispTIMER_64_PRESCALE				( ( unsigned char ) ( ( 1 << CS01 ) | ( 1 << CS00 ) ) )

/* Constants for writing to TIMSK0 */
#define dispTIMER_ENABLE_COMPA				( ( unsigned char ) ( 1 << OCIE0A ) )

#define MAX_CHECKS 10
#define NUM_BUTTONS 4
#define BUTTON_QUEUE_LENGTH 16

volatile uint8_t debounced_state;
volatile uint8_t prev_debounced_state;
uint8_t state[MAX_CHECKS];
uint8_t index;

static xQueueHandle button_events;

static inline uint8_t buttons_raw_state (void)
{
	/* Only select switches from the port */
	return ~SWPIN & ((1 << SW0) | (1 << SW1) | (1 << SW2) | (1 << SW3));
}

static inline void buttons_debounce (void)
{
	uint8_t i,j;
	prev_debounced_state = debounced_state;
	state[index] = buttons_raw_state ();
	++index;
	j = 0xff;
	for(i = 0; i < MAX_CHECKS; i++)
	j = j & state[i];
	debounced_state = j;
	if (index>=MAX_CHECKS) index=0;
}

static inline void buttons_update (uint8_t button, uint8_t value)
{
	signed char xHigherPriorityTaskWoken = pdFALSE;
	
	if ((debounced_state & (1 << button)) && (~prev_debounced_state & (1 << button)))
	{
		xQueueSendFromISR ( button_events, &value, &xHigherPriorityTaskWoken );
		
		if( xHigherPriorityTaskWoken != pdFALSE )
		{
			taskYIELD ();
		}
	}
}

void buttons_init (void)
{
	/* Set pull-ups */
	SWPORT |= (1 << SW0) | (1 << SW1) | (1 << SW2) | (1 << SW3);
	
	button_events = xQueueCreate( BUTTON_QUEUE_LENGTH, ( unsigned char ) sizeof( unsigned char ) );
	
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
}

uint8_t buttons_get (uint8_t *button, portTickType block_time)
{
	if (xQueueReceive (button_events, button, block_time) != pdPASS)
	{
		return pdFAIL;
	}
	
	return pdPASS;
}

ISR (TIMER0_COMPA_vect)
{
	buttons_debounce ();
	
	/* Check for new events */
	buttons_update (SW0, 0);
	buttons_update (SW1, 1);
	buttons_update (SW2, 2);
	buttons_update (SW3, 3);
}