/*
 * IRsensor.c
 *
 * Created: 02.11.2013 12:35:34
 *  Author: Normann
 */ 
#include "IRsensor.h"

void IRsensor_init(){
	DDRD = 1<<PD0;		// Set PD0 as input (Using for interrupt INT0)
	PORTD = 1<<PD0;		// Enable PD0 pull-up resistor
		
	EIMSK = 1<<INT0;					// Enable INT0
	
	/* set mode */
	//MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	/* or */
	MCUCR = 1<<ISC01;	// Trigger INT0 on falling edge
	
	sei();				//Enable Global Interrupt
}