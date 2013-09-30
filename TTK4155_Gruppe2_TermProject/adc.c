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
 
static inline uint8_t adc_average (struct ring_buffer *buffer)
 {
	 uint16_t val = 0;
		
	 for (int i = 0; i < buffer->size; i++) {
		 val += ring_buffer_get(buffer);
	 }
	 
	 return (val / buffer->size);
}

static inline void adc_update(struct ring_buffer *buffer)
{
	uint8_t data = ext_adc[0];
	
	ring_buffer_get(buffer);
	ring_buffer_put(buffer, data);
	adc.avg[adc.cur_buffer] = adc_average(buffer);
}

void adc_init (void)
 {
	 /* Enable pin interrupt */
	 GICR |= (1 << INT0);
	 
	 /* Set up buffers for moving average */
	 for (int i = 0; i < N_CHANNELS; i++) {
		 adc.buffers[i] = ring_buffer_init(adc.buf[i], BUFFER_SIZE);
	 }
	 
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
	 return adc.avg;
 }	 
  
ISR (TIMER1_COMPA_vect)
 
 {
	 adc_start_conversion(adc.cur_buffer);
}

ISR (INT0_vect)
{
	adc_update(&adc.buffers[adc.cur_buffer]);
	 
	switch (adc.cur_buffer) {
		case X:
			adc.cur_buffer = Y;
			break;
		case Y:
			adc.cur_buffer = S1;
			break;
		case S1:
			adc.cur_buffer = S2;
			break;
		default:
			adc.cur_buffer = X;
			break;
	}
}