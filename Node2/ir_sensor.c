/*
 * ir_sensor.c
 *
 * Created: 02.11.2013 12:35:34
 *  Author: Normann
 */ 

/* Constants for writing to ADMUX */
#define adc_AVCC						(1 << REFS0)
#define adc_ADLAR						(1 << ADLAR)

/* Constants for writing to ADCSRA */
#define adc_128_PRESCALE				(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
#define adc_FREE_RUN					(1 << ADFR)
#define adc_ENABLE						(1 << ADEN)
#define adc_INT_ENABLE					(1 << ADIE)
#define adc_START_CONVERSION			(1 << ADSC)

#include "ir_sensor.h"

void ir_sensor_init (void)
{
	/* Set prescaler at 128 for 125kHz operation */
	ADCSRA |= adc_128_PRESCALE;
			
	/* Set reference voltage to AVCC and left shift result */
	ADMUX = (adc_AVCC | adc_ADLAR);
			
	/* Enable ADC */
	ADCSRA |= (adc_FREE_RUN | adc_ENABLE | adc_START_CONVERSION | adc_INT_ENABLE);
}