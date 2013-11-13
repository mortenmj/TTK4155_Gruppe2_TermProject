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
	unsigned char ls, rs, lb, rb;
	
	xLastWakeTime = xTaskGetTickCount ();
	
	/* CAN init */
	can_init ();
	
	/* ADC init */
	adc_init ( ctrlNUM_ADC_VALUES );
	
	/* Touch init */
	touch_init ( 30, 30, 30, 30 );
	
	char cls[6];
	char crs[6];
	char clb[4];
	char crb[4];
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, ctrlTASK_FREQUENCY );

		if ( adc_conversion_complete () == pdTRUE )
		{
			adc_disable ();
			adc_get_value ( &valx, 0 );
			adc_get_value ( &valy, 0 );
			touch_measure ( &ls, &rs, &lb, &rb );
			
			utoa (ls, cls, 10);
			utoa (rs, crs, 10);
			utoa (lb, clb, 10);
			utoa (rb, crb, 10);
			
			vSerialPutString (NULL, cls, 6);
			vSerialPutString (NULL, "\t", 1);
			vSerialPutString (NULL, crs, 6);
			vSerialPutString (NULL, "\t", 1);
			vSerialPutString (NULL, clb, 4);
			vSerialPutString (NULL, "\t", 1);
			vSerialPutString (NULL, crb, 4);
			vSerialPutString (NULL, "\n", 1);

			frame.id = 1;
			frame.data[0] = valx;
			frame.data[1] = valy;
			frame.data[2] = ls;
			frame.data[3] = rs;
			frame.data[4] = lb;
			frame.data[5] = rb;
			frame.dlc = 6;
			can_transmit ( &frame );
			
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