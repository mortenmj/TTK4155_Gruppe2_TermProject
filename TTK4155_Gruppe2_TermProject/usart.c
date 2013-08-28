/*
 * CFile1.c
 *
 * Created: 28.08.2013 14:41:57
 *  Author: mortenmj
 */ 

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE 19200
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#define READ_BUF_SIZE 64
#define SEND_BUF_SIZE 64

char send_buf[SEND_BUF_SIZE];
uint8_t send_buf_head = 0;
uint8_t send_buf_tail = 0;

void usart_init (void) {
	/* Set baud rate */
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	
	/* Enable rx & tx */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	
	/* Frame format: 8 bit data, 1 stop bit, no parity */
	UCSR0C |= (1 << URSEL0) | (1 << UCSZ01) | (1 << UCSZ00);

	/* Enable rx & tx interrupt */
	//UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
}

void usart_put (char data) {
	send_buf[send_buf_head] = data;
	if (++send_buf_head >= SEND_BUF_SIZE) {
		send_buf_head = 0;
	}
}

char usart_get (void) {
	char data;
	data = send_buf[send_buf_tail];
	if (++send_buf_tail >= SEND_BUF_SIZE) {
		send_buf_tail = 0;
	}
	
	return data;
}

uint8_t usart_buffer_contents (void) {
	return ((send_buf_tail - send_buf_tail) % SEND_BUF_SIZE);
}