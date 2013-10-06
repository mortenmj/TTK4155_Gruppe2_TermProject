/*
 * uart.h
 *
 * Created: 28.08.2013 14:57:22
 *  Author: mortenmj
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

void uart_init (void);
void uart_putchar (char data);
char uart_getchar (void);
bool uart_char_waiting (void);

#endif /* USART_H_ */
