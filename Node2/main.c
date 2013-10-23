/*
 * main.c
 *
 * Created: 16.10.2013 14:46:56
 *  Author: mortenmj
 */ 

#include "config.h"

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "can.h"

static FILE uart_stdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{	
	uart_init ();
	can_init (0x10);
	
	uint8_t count = 0;
	sei();
		
	stdout = &uart_stdout;
	
	_delay_ms (2);
	
	printf ("Initialized\n");
		
	while(1) {
		can_frame_t frame;
		frame.identifier = 0x00;
		frame.size = count%8;
		for(uint8_t i = 0; i < frame.size; i++) {
			frame.data[i] = ~(128-i)+count;
		}
		uint8_t s = can_send_frame(&frame);
		printf("Sent data %d %s\r",count, (s?"SUCCESS":"FAIL"));
	 
		can_frame_t recieved;
	 
		_delay_ms(100);
		uint8_t status = mcp2515_read_status();
		printf("  Status = 0x%x\r", status);
		s = can_recieve_frame(&recieved);
		printf("  Receieved data: L1: %d L2: %d %s\r", frame.size, recieved.size, (s?"SUCCESS":"FAIL"));
	 
		_delay_ms(100);
		if(frame.size == recieved.size) {
			printf("  Identical length\r");
			uint8_t passed = 1;
			for(uint8_t i = 0; i < recieved.size; i++) {
				if(frame.data[i] != recieved.data[i]) {
					passed = 0;
					continue;
				}
			}
			if(passed) {
				printf("    AND identical data\r");
			}
			else {
				printf("    FAIL! NOT identical data\r");
			 
			}
		} else {
			printf("  FAIL! NOT identical length\r");
		}
		status = mcp2515_read_status();
		printf("  Status = 0x%x\r", status);
		_delay_ms(500);
	 
		count++;
	}
}