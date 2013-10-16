/*
 * uart.c
 *
 * Created: 28.08.2013 14:41:57
 *  Author: mortenmj
 */ 

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ring_buffer.h"

#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
#define BUFFER_SIZE 80

uint8_t in_buffer[BUFFER_SIZE];
uint8_t out_buffer[BUFFER_SIZE];
struct ring_buffer ring_buffer_in;
struct ring_buffer ring_buffer_out;

void uart_init (void) {
	/* Set baud rate */
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;	
	
	/* Enable rx & tx */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	
	/* Frame format: 8 bit data, 1 stop bit, no parity */
	UCSR0C |= (1 << URSEL0) | (1 << UCSZ01) | (1 << UCSZ00);
	
	ring_buffer_in = ring_buffer_init(in_buffer, BUFFER_SIZE);
	ring_buffer_out = ring_buffer_init(out_buffer, BUFFER_SIZE);
}


void uart_putchar (char data) {
	cli();
	if (ring_buffer_is_empty (&ring_buffer_out)) {
		/* Since the buffer is empty, we must enable the data ready interrupt */
		UCSR0B |= (1 << UDRIE0);
	}
	
	ring_buffer_put (&ring_buffer_out, data);
	
	sei();
}

char uart_getchar (void) {
	return ring_buffer_get(&ring_buffer_in);
}

bool uart_char_waiting (void) {
	return !ring_buffer_is_empty(&ring_buffer_in);
}

/* TODO: Write received byte to buffer which is read at a later time */
ISR (USART0_RXC_vect)
{
	ring_buffer_put(&ring_buffer_in, UDR0);
}


ISR (USART0_UDRE_vect)
{
	/* Check if there is data in the buffer and send it */
	if (!ring_buffer_is_empty(&ring_buffer_out)) {
		UDR0 = ring_buffer_get(&ring_buffer_out);
	} else {
		/* No more data, turn off data ready interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}
