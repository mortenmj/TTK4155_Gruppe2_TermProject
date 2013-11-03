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
	/* OC1A set to output */
	DDRB |= (1 << PB6);
	
	/* non-inverting mode for OC1B */
	TCCR1A |= (1 << COM1B1);
	
	/* Fast PWM */
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
		
	/* Prescaler 8 */
	TCCR1B |= (1 << CS11);
	
	/* We want a signal at 50Hz. With F_CPU = 8Mhz and prescale 8, we need ICR3 = 20k */
	ICR1 = 20000;
	
	/* Enable timer interrupt */
	TIMSK |= (1 << TOIE1);
	OCR1B = 16000;
}

uint16_t servo_val (uint8_t joy_val)
{
	return ((SERVO_MAX - SERVO_MIN) / CTRL_MAX_VAL) * joy_val + SERVO_MIN;
}