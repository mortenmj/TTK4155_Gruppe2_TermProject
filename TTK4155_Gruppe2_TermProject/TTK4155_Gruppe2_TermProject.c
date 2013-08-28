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

#include "usart.h"

int main(void)
{
	DDRA = 0xFF;
	usart_init();
	//sei();
	
    while(1)
    {
        //TODO:: Please write your application code 
		PORTA = 0xFF;
		_delay_ms(200);
		PORTA = 0x00;
		_delay_ms(200);
		
		while ( !(UCSR0A & (1 << UDRE0)));
		UDR0 = 'c';
    }
}

/* TODO: Write received byte to buffer which is read at a later time */
/*
ISR (USART0_RXC_vect)
{
	char recv;
	recv = UDR0;
	UDR0 = recv;
}
*/

/*
ISR (USART0_TXC_vect)
{

}
*/