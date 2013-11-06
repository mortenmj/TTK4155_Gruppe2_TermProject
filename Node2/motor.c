/*
 * motor.c
 *
 * Created: 06.11.2013 13:18:40
 *  Author: mortenmj
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "motor.h"
#include "TWI_Master.h"

#define MOTOR_DDR				DDRA
#define MOTOR_PORT				PORTA
#define MOTOR_OUTPUT_ENABLE		PA7
#define MOTOR_RESET				PA6
#define MOTOR_SELECT_BYTE		PA5
#define MOTOR_ENABLE			PA4
#define MOTOR_DIR				PA3

#define MOTOR_DATA				PINC

uint8_t input;

void motor_init (void)
{
	MOTOR_DDR |= (1 << MOTOR_OUTPUT_ENABLE) | (1 << MOTOR_RESET) | (1 << MOTOR_SELECT_BYTE) | (1 << MOTOR_ENABLE) | (1 << MOTOR_DIR);
	MOTOR_PORT |= (1 << MOTOR_ENABLE) | (1 << MOTOR_RESET);

	/* Compare match at 10625 gives a timer freq of 100hz */
	OCR3A = 10400;

	/* Enable CTC */
	TCCR3A |= (1 << COM3A0);

	/* Prescale = 8 */
	TCCR3B = (1 << CS31) | (1 << WGM32);
	
	/* Enable compare match A interrupt */
	ETIMSK |= (1 << OCIE3A);

	/* Set TWI pins to output and initialize */
	DDRD |= (1 << PD0) | (1 << PD1);
	TWI_Master_Initialise ();
	
	motor_reset ();
	motor_write (0);
	
	return;
}

int16_t motor_read (void)
{
	int16_t byte = 0;
	
	MOTOR_PORT &= ~(1 << MOTOR_OUTPUT_ENABLE);
	MOTOR_PORT &= ~(1 << MOTOR_SELECT_BYTE);
	_delay_us (20);
	
	/* Read MSB */
	byte |= (MOTOR_DATA << 8);
	
	MOTOR_PORT |= (1 << MOTOR_SELECT_BYTE);
	_delay_us (20);
	
	/* Read LSB */
	byte |= (MOTOR_DATA << 0);
	
	motor_reset ();
	MOTOR_PORT |= (1 << MOTOR_OUTPUT_ENABLE);
	
	return byte;
}

void motor_write (uint8_t val)
{
	input = val;
}

void motor_reset (void)
{
	MOTOR_PORT &= ~(1 << MOTOR_RESET);
	_delay_us (1);
	MOTOR_PORT |= (1 << MOTOR_RESET);
}

ISR (TIMER3_COMPA_vect)
{
	unsigned char messageBuf[3];
	uint8_t val = input;

	if (val < 117)
	{
		/* Set direction left */
		MOTOR_PORT &= ~(1 << MOTOR_DIR);
	}
	else if (val > 137)
	{
		/* Set direction right */
		MOTOR_PORT |= (1 << MOTOR_DIR);
	}
	else
	{
		return;
	}
		
	/* Data should be 0 for val = 128 and 255 for val = 0 or 255 */
	val = abs(val - 128) * 2 - 1;
		
	/* Address */
	messageBuf[0] = 0x50;
		
	/* Command */
	messageBuf[1] = 0x00;
		
	/* Data */
	messageBuf[2] = val;
	TWI_Start_Transceiver_With_Data (messageBuf, 3);
		
	return;
}