/*
 * adc.c
 *
 * Created: 08.09.2013 13:30:53
 * Author: mortenmj
 */ 

#include <stdlib.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "serial.h"
#include "adc.h"

/* Constants for writing to ADMUX */
#define adc_AVCC						( ( unsigned char ) (1 << REFS0) )
#define adc_ADLAR						( ( unsigned char ) (1 << ADLAR) )

/* Constants for writing to ADCSRA */
#define adc_125KHZ						( ( unsigned char ) (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) )
#define adc_ENABLE						( ( unsigned char ) (1 << ADEN) )
#define adc_INT_ENABLE					( ( unsigned char ) (1 << ADIE) )
#define adc_START_CONVERSION			( ( unsigned char ) (1 << ADSC) )

static xQueueHandle adc_values;
xSemaphoreHandle adc_sem;

#define ENABLE_INTERRUPT()												\
{																	\
	unsigned char ucByte;											\
																	\
	ucByte = ADCSRA;												\
	ucByte |= adc_INT_ENABLE;										\
	ADCSRA = ucByte;												\
}
/*-----------------------------------------------------------*/

#define DISABLE_INTERRUPT()												\
{																	\
	unsigned char ucInByte;											\
																	\
	ucInByte = ADCSRA;												\
	ucInByte &= ~adc_INT_ENABLE;									\
	ADCSRA = ucInByte;												\
}
/*-----------------------------------------------------------*/

static unsigned char adc_get_port ( void )
{
	return ADMUX & 0x1F;
}

static void adc_set_port ( unsigned char port )
{
	unsigned char ucByte;
	
	/* Clear bottom 5 bits of ADMUX, then set them from port */
	ucByte = ADMUX;
	ucByte &= 0xE0;
	ucByte |= port;
	ADMUX = ucByte;
}

void adc_init ( unsigned char queue_length )
{
	
	portENTER_CRITICAL();
	{
		adc_values = xQueueCreate( queue_length, ( unsigned char ) sizeof( signed char ) );
		vSemaphoreCreateBinary ( adc_sem );
		
		/* Take the semaphore so that the queue isn't accessed before it is populated */
		xSemaphoreTake ( adc_sem, 0 );
		
		/* Set prescaler at 128 for 125kHz operation */
		ADCSRA |= adc_125KHZ;
		
		/* Set reference voltage to AVCC and left shift result */
		ADMUX = ( adc_AVCC | adc_ADLAR );
	
		/* Enable ADC */
		ADCSRA |= adc_ENABLE;
			
		ENABLE_INTERRUPT();
		adc_start_conversion ();
	}
	portEXIT_CRITICAL();
}

signed char adc_get_value ( signed char *val, portTickType block_time )
{
	if ( xQueueReceive ( adc_values, val, block_time ) != pdPASS )
	{
		return pdFAIL;
	}
	
	return pdPASS;
}

void adc_start_conversion ( void )
{
	ADCSRA |= adc_START_CONVERSION;
}

unsigned char adc_take_semaphore ( void )
{
	return xSemaphoreTake ( adc_sem, 0 );
}

void adc_close ( void )
{
unsigned char ucByte;
	
	portENTER_CRITICAL();
	{
		DISABLE_INTERRUPT();
		ucByte = ADCSRA;
		ucByte &= ~adc_ENABLE;
		ADCSRA = ucByte;
	}
	portEXIT_CRITICAL();		
}

ISR (ADC_vect)
{
portCHAR data;
signed char xHigherPriorityTaskWoken = pdFALSE;
const char port = adc_get_port ();
	
	/* Get the value and post it on the queue of ADC values.
	If the post causes a task to wake force a context switch as the woken task may have a higher priority than the task we have interrupted. */
	data = ADCH;
	xQueueSendFromISR( adc_values, &data, &xHigherPriorityTaskWoken );
	
	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD ();
	}
	
	/* If the current result is from port 0, the next should be from port 1 and vice versa */
	if (port == adcPORT0) 
	{
		adc_set_port ( adcPORT1 );
		
		/* After one conversion, we immediately start the second. */
		adc_start_conversion ();
	} else
	{
		adc_set_port ( adcPORT0 );
		
		/* After the second conversion, we give the semaphore */
		xSemaphoreGiveFromISR ( adc_sem, &xHigherPriorityTaskWoken );
	}
	
	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD ();
	}
}