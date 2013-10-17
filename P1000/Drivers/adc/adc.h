/*
 * adc.h
 *
 * Created: 05.09.2013 15:03:02
 *  Author: mortenmj
 */ 

#ifndef ADC_H_
#define ADC_H_

typedef enum
{
	adcPORT0,
	adcPORT1,
	adcPORT2,
	adcPORT3,
	adcPORT4,
	adcPORT5,
	adcPORT6,
	adcPORT7		
} eADCPort;

void adc_init ( unsigned char queue_length );
signed char adc_get_value ( signed char *val, portTickType block_time );
void adc_start_conversion ( void );
unsigned char adc_take_semaphore ( void );
void adc_close ( void );

#endif /* ADC_H_ */