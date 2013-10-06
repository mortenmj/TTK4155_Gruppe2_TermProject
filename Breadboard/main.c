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

/* Using stdio for this is completely retarded */
#include <stdio.h>

 int uart_putchar_printf(uint16_t data, FILE *stream) {
	if (data == '\n') {
		uart_putchar('\r');
	}
	uart_putchar(data);
	return 0;
}

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
	menu_init();	
	cli();
	uart_init();
	extmem_init();
	adc_init();
	oled_init();
	sei();
	
	stdout = &uart_stdout;
	
	printf ("AVR initialized\n");
/*
	oled_set_line (0);
	oled_print("morten");
		
	oled_set_line (1);
	oled_print("mjelva");
	
	oled_set_line (2);
	oled_print("er");
	
	oled_set_line (3);
	oled_print("best");
	
	oled_set_position (0, 7);
	oled_print ("hurra");
*/
	menu_display ();
	
	uint8_t *joystick_values;
	direction_t joystick_direction;
	
	while (1)
	{
		joystick_values = adc_get_values();
		joystick_direction = getdirection(joystick_values);
		menu_update_fsm(joystick_direction);
		menu_display();
	}
}


