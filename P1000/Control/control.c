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
#include "touch.h"

#include "can.h"
#include "MCP2515define.h"

/* Task frequency */
#define ctrlTASK_FREQUENCY				( ( const portTickType ) 5 )

/* Number of ADC values to queue */
#define ctrlNUM_ADC_VALUES				( 2 )

void vControl ( void *pvParameters )
{
	portTickType xLastWakeTime;
	can_frame_t frame;
	signed char valx, valy;
	signed char ls, rs, lb, rb;
	
	xLastWakeTime = xTaskGetTickCount ();
	
	/* CAN init */
	can_init ();
	
	/* ADC init */
	adc_init ( ctrlNUM_ADC_VALUES );
	
	/* Touch init */
	touch_init ( 30, 30, 30, 30 );
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, ctrlTASK_FREQUENCY );

		if ( adc_take_semaphore () == pdTRUE )
		{
			adc_get_value ( &valx, 0 );
			adc_get_value ( &valy, 0 );
			touch_measure ( &ls, &rs, &lb, &rb );
			
			frame.id = 1;
			frame.data[0] = 0;
			frame.data[1] = valx;
			frame.data[2] = valy;
			frame.data[3] = 0;
			frame.data[4] = ls;
			frame.data[5] = rs;
			frame.data[6] = lb;
			frame.data[7] = rb;
			frame.dlc = 8;
			
			can_transmit ( &frame );
			
			adc_start_conversion ();
		}
	}
}

/* Handle interrupts from SPI connections */
ISR ( PCINT0_vect )
{
	//mcp2515_read ( MCP2515_CANINTF, &xCan.interrupt );
}