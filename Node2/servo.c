/*
 * servo.c
 *
 * Created: 30.10.2013 15:40:31
 *  Author: magnealv
 */

#include "servo.h"

#include <avr/interrupt.h>

void servo_init ()
{
	// OC3A set to output
	DDRE |= (1 << PE3);
	
	// non-inverting mode for OC3A
	TCCR3A |= (1 << COM3A1) | (1 << WGM31);
	
	// Mode 14
	TCCR3B |= ( 1 <<WGM33) | ( 1 <<WGM32);
		
	// Prescaler 8
	TCCR3B |= (1 << CS31);	
	
	ICR3 = 20000;
	
	// Enabling Timer Interrupt
	ETIMSK |= (1 << TOIE3);
	
	OCR3A = 0;
}