/*
 * TTK4155_Gruppe2_TermProject.c
 *
 * Created: 28.08.2013 08:34:25
 *  Author: magnealv
 */ 

#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "menu.h"
#include "oled.h"
#include "uart.h"
#include "can.h"

/* Using stdio for this is completely retarded */
#include <stdio.h>

static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

/* Retardation ends */

void sram_test(void)
{
	volatile uint16_t *ext_ram = (uint16_t *) 0x1800; // Start address for the SRAM
	
	uint8_t i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	uint16_t testvalue;
	printf("Starting SRAM test...\r\n");
	
	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			//_delay_ms(30);
			werrors++;
		}
	}
	
	for	(i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			//_delay_ms(30);
			rerrors++;
		}
	}
	
	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
	_delay_ms(20);
}

void extmem_init(void)
{
	/* Enable External Memory Interface */
	MCUCR |= (1 << SRE);
	
	/* Mask out JTAG pins PC4-PC7 */
	SFIOR |= (1 << XMM2);
}



int main(void)
{
	uart_init ();
	stdout = &uart_stdout;
	printf ("Initializing...\n");

	can_init (0x10);
	uint8_t count = 0;
	sei();
		
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