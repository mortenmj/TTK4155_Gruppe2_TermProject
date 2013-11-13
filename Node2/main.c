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
#include "motor.h"
#include "solenoid.h"

static FILE uart_stdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

can_frame_t frame;
ir_state_t ir;
uint8_t joy_x, joy_y, ls, rs, lb, rb;

int main(void)
{
	uart_init ();
	spi_init ();
	can_init ();
	servo_init ();
	ir_sensor_init ();
	motor_init ();
	solenoid_init ();
		
	sei();
		
	stdout = &uart_stdout;
	
	_delay_ms (200);
	
	printf ("Initialized\n");

	char cjoy_x[4];
	char cjoy_y[4];
	char cls[4];
	char crs[4];
	char clb[4];
	char crb[4];
	char motor[8];
	uint16_t motorval;
	
	while(1)
	{
		can_receive (&frame);
		joy_x = frame.data[0];
		joy_y = frame.data[1];
		ls = frame.data[2];
		rs = frame.data[3];
		lb = frame.data[4];
		rb = frame.data[5];

		utoa (joy_x, cjoy_x, 10);
		utoa (joy_y, cjoy_y, 10);
		utoa (ls, cls, 10);
		utoa (rs, crs, 10);
		utoa (lb, clb, 10);
		utoa (rb, crb, 10);

		printf("x=%s\t", cjoy_x);
		printf("y=%s\n", cjoy_y);
		printf("ls=%s\t", cls);
		printf("rs=%s\t", crs);
		printf("lb=%s\t", clb);
		printf("rb=%s\n", crb);
		printf("\n\n");
		
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
		
		if (rb)
		{
			solenoid_fire ();
		}
		else
		{
			solenoid_unfire ();
		}
		
		motor_write (joy_x);
	}
}

ISR (TIMER1_OVF_vect)
{
	OCR1B = servo_val (255-rs);
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