/*
 * control.c
 *
 * Created: 02.10.2013 23:26:51
 *  Author: mortenmj
 */ 

#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "control.h"

#include "adc.h"
#include "serial.h"

#include "can.h"
#include "MCP2515define.h"

/* Task frequency */
#define ctrlTASK_FREQUENCY				( ( const portTickType ) 500 )

/* Number of ADC values to queue */
#define ctrlNUM_ADC_VALUES				( 2 )

void vControl ( void *pvParameters )
{
	portTickType xLastWakeTime;
	can_frame_t frame_out = { 1, {'m', 'o', 'r', 't', 0, 0, 0, 0}, 8 };
	can_frame_t frame_in;
	signed char valx, valy;
	
	xLastWakeTime = xTaskGetTickCount ();
	
	/* CAN init */
	can_init ();
	
	/* ADC init */
	adc_init ( ctrlNUM_ADC_VALUES );
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, ctrlTASK_FREQUENCY );

		vSerialPutString( NULL, "ctrl\n", 5 );
		can_transmit ( &frame_out );
		
		//vSerialPutString ( NULL, frame_in.data, 3 );
		//vSerialPutString ( NULL, "\n", 1 );
		
		if ( adc_take_semaphore () == pdTRUE )
		{
			adc_get_value ( &valx, 0 );
			adc_get_value ( &valy, 0 );
			
			adc_start_conversion ();
		}
	}
}

/* Handle interrupts from SPI connections */
ISR ( PCINT0_vect )
{
	//mcp2515_read ( MCP2515_CANINTF, &xCan.interrupt );
}