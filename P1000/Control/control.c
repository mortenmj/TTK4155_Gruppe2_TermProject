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

#include "event.h"
#include "game.h"
#include "buttons.h"
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
	signed char valx, valy;
	unsigned char ls, rs, lb, rb;
	
	can_frame_t out_frame;
	can_frame_t in_frame;
	out_frame.id = 1;
	out_frame.dlc = 6;
	
	event_t evnt = {EVENT_GAME, 0};
	
	xLastWakeTime = xTaskGetTickCount ();
	
	/* Button init */
	buttons_init ();
	
	/* CAN init */
	can_init ();
	
	/* ADC init */
	adc_init ( ctrlNUM_ADC_VALUES );
	
	/* Touch init */
	touch_init ( 30, 30, 30, 30 );
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, ctrlTASK_FREQUENCY );

		if ( adc_conversion_complete () == pdTRUE )
		{
			adc_disable ();
			adc_get_value ( &valx, 0 );
			adc_get_value ( &valy, 0 );
			touch_measure ( &ls, &rs, &lb, &rb );
			
			out_frame.data[0] = valx;
			out_frame.data[1] = valy;
			out_frame.data[2] = ls;
			out_frame.data[3] = rs;
			out_frame.data[4] = lb;
			out_frame.data[5] = rb;
			
			can_transmit (&out_frame);
			can_receive (&in_frame, 0);
			
			if (in_frame.data[0] == GAME_SENSOR_TRIGGERED)
			{
				evnt.val = GAME_SENSOR_TRIGGERED;
				vSerialPutString(NULL, "trig\n", 5);
				event_put (&evnt, 0);
				in_frame.data[0] = 0;
			}
			else if (in_frame.data[0] == GAME_SENSOR_CLEARED)
			{
				evnt.val = GAME_SENSOR_CLEARED;
				vSerialPutString(NULL, "clear\n", 6);
				event_put (&evnt, 0);
				in_frame.data[0] = 0;
			}
			
			adc_enable ();
			adc_conversion_start ();
		}
	}
}

/* Handle interrupts from SPI connections */
ISR ( PCINT0_vect )
{
	//mcp2515_read ( MCP2515_CANINTF, &xCan.interrupt );
}