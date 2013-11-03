/*
 * main.c
 *
 * Created: 16.10.2013 14:46:56
 *  Author: mortenmj
 */ 

#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "can.h"
#include "spi.h"
#include "servo.h"
#include "ir_sensor.h"

static FILE uart_stdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

can_frame_t frame;
ir_state_t ir;

int main(void)
{	
	uart_init ();
	spi_init ();
	can_init ();
	servo_init ();
	ir_sensor_init ();
	
	sei();
		
	stdout = &uart_stdout;
	
	_delay_ms (200);
	
	printf ("Initialized\n");

	char msgtype[4];
	char joy_x[4];
	char joy_y[4];
	char joy_btn[4];
	char ls[4];
	char rs[4];
	char lb[4];
	char rb[4];
	
	while(1)
	{
//		can_receive (&frame);

/*
		utoa (frame.data[0], msgtype, 10);
		utoa (frame.data[1], joy_x, 10);
		utoa (frame.data[2], joy_y, 10);
		utoa (frame.data[3], joy_btn, 10);
		utoa (frame.data[4], ls, 10);
		utoa (frame.data[5], rs, 10);
		utoa (frame.data[6], lb, 10);
		utoa (frame.data[7], rb, 10);*/
/*
				
		printf("message type: %s\n", msgtype);
		printf("joystick: ");
		printf("x=%s\t", joy_x);
		printf("y=%s\n", joy_y);
		
		printf("touch: ");
		printf("ls=%s\t", ls);
		printf("rs=%s\t", rs);
		printf("lb=%s\t", lb);
		printf("rb=%s\n", rb);
		printf("\n\n");*/
		
		if (ir.low && ir.changed)
		{
			ir.changed = false;
			printf("game over\n");
			// game over
		}
		else if (!ir.low && ir.changed)
		{
			ir.changed = false;
			printf("new game\n");
			// new game
		}			
		
		_delay_ms (100);
	}
}

ISR (TIMER1_OVF_vect)
{
	OCR1B = servo_val (frame.data[1]);
}

ISR (ADC_vect)
{
	if ((ADCH < IR_TRESHOLD) && !ir.low)
	{
		ir.changed = true;
		ir.low = true;
	}
	else if ((ADCH >= IR_TRESHOLD) && ir.low)
	{
		ir.changed = true;
		ir.low = false;
	}		
}