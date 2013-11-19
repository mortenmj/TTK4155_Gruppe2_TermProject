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

#include "fsm.h"

#include "buttons.h"
#include "game.h"
#include "adc.h"
#include "serial.h"
#include "touch.h"

#include "can.h"
#include "MCP2515define.h"

/* Task frequency */
#define ctrlTASK_FREQUENCY				( ( const portTickType ) 5 )

/* Number of ADC values to queue */
#define ctrlNUM_ADC_VALUES				( 2 )

xTaskHandle task_handle;

xTaskHandle xControlGetHandle (void)
{
    return task_handle;
}

void vControlSetHandle (xTaskHandle handle)
{
    task_handle = handle;
}

void vControl ( void *pvParameters )
{
	portTickType xLastWakeTime;
	signed char valx, valy;
	unsigned char ls, rs, lb, rb;

	can_frame_t out_frame;
	can_frame_t in_frame;
	out_frame.id = 1;
	out_frame.dlc = 6;

	xLastWakeTime = xTaskGetTickCount ();

	/* Button init */
	buttons_init ();

	/* FSM init */
	fsm_init ();

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
				// TODO: set triggered state
				fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
				event->type = EV_STOP;
				event->ptr = NULL;
				fsm_event_put (event, portMAX_DELAY);
				in_frame.data[0] = 0;
			}
			else if (in_frame.data[0] == GAME_SENSOR_CLEARED)
			{
				// TODO: set cleared state
				in_frame.data[0] = 0;
			}

			adc_enable ();
			adc_conversion_start ();
		}

		fsm_update ();
	}
}