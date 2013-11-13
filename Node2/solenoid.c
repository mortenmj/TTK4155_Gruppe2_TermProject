/*
 * solenoid.c
 *
 * Created: 13.11.2013 12:39:40
 *  Author: mortenmj
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define SOL_DDR		DDRF
#define SOL_PORT	PORTF
#define SOL_MASK	(1 << PF1);

void solenoid_init (void)
{
	/* Set solenoid pin as output */
	SOL_DDR |= SOL_MASK;
}

void solenoid_fire (void)
{
	SOL_PORT &= ~SOL_MASK;
}

void solenoid_unfire (void)
{
	SOL_PORT |= SOL_MASK;
}