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

#include "ring_buffer.h"
#include "adc.h"
#include "menu.h"
#include "oled.h"
#include "uart.h"
#include "spi.h"
#include "can.h"

#include "MCP2515.h"
#include "MCP2515define.h"

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
	//can_frame_t in_frame;	
	//can_frame_t out_frame = {1, { 'm', 'o', 'r', 't', 'e', 'n', }, 6};
	uint8_t cnf1, cnf2, cnf3;
	
	uart_init ();
	stdout = &uart_stdout;
	printf ("Initializing...\n");
	
	spi_init ();
	can_init ();
	sei();
		
	printf ("Initialized\n");

	while (1)
	{
	    mcp2515_read (MCP2515_CNF1, &cnf1);
	    mcp2515_read (MCP2515_CNF2, &cnf2);
	    mcp2515_read (MCP2515_CNF3, &cnf3);
		
		printf("cnf1: %d, cnf2: %d, cnf3: %d", cnf1, cnf2, cnf2);
		
	    _delay_ms (500);
	}
}