/*
 * usart.h
 *
 * Created: 28.08.2013 14:57:22
 *  Author: mortenmj
 */ 


#ifndef USART_H_
#define USART_H_

void usart_init (void);
void usart_put (char data);
char usart_get (void);
uint8_t usart_buffer_contents (void);

#endif /* USART_H_ */