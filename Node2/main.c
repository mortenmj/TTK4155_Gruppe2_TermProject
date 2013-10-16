/*
 * main.c
 *
 * Created: 16.10.2013 14:46:56
 *  Author: mortenmj
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	
	uart_init ();
	stdout = &uart_stdout;
	
	printf ("Initialized\n");
	
	DDRD |= PD6;
	
    while(1)
    {
		DDRD ^= PD6;
		_delay_ms (300);
    }
}