/*
 * adc.c
 *
 * Created: 08.09.2013 13:30:53
 * Author: mortenmj
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "adc.h"
#include "ring_buffer.h"

adc_t adc;

volatile char *ext_adc = (char *) 0x1400;

static inline void adc_start_conversion (uint8_t channel)
{
	 /* Read any address in the ADC address space to trigger conversion */
	 ext_adc[0] = 0x04 + channel;
}

static inline uint8_t adc_read (void)
{
	 return ext_adc[0];
}

static inline void adc_update(void)
{
	uint8_t data = ext_adc[0];
	adc.values[adc.cur_channel] = data;
}

void adc_init (void)
 {
	 /* Enable pin interrupt */
	 GICR |= (1 << INT0);
	 
	 /* Set up timer to trigger ADC conversion */
	 /* Set CTC mode and 1024 prescale */
	 TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	 
	 /* Enable timer */
	 TIMSK |= (1 << OCIE1A);
	 	 
	 /* With F_CPU / PRESCALE / SPS */
	 OCR1A = 16;
}
 
uint8_t* adc_get_values (void)
 {
	 return adc.values;
 }	 
  
ISR (TIMER1_COMPA_vect)
 
 {
	 adc_start_conversion(adc.cur_channel);
}

ISR (INT0_vect)
{
	adc_update ();
	 
	switch (adc.cur_channel) {
		case X:
			adc.cur_channel = Y;
			break;
		case Y:
			adc.cur_channel = S1;
			break;
		case S1:
			adc.cur_channel = S2;
			break;
		default:
			adc.cur_channel = X;
			break;
	}
}