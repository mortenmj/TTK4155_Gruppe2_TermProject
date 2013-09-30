/*
 * adc.h
 *
 * Created: 05.09.2013 15:03:02
 *  Author: mortenmj
 */ 

#ifndef ADC_H_
#define ADC_H_

#include "ring_buffer.h"

#define BUFFER_SIZE 10
#define ADC_MAX_VAL 255
#define ADC_MIN_VAL 0

enum adc_channels {X, Y, S1, S2, N_CHANNELS};
	
typedef struct adc_values
{
	uint8_t buf[N_CHANNELS][BUFFER_SIZE];
	struct ring_buffer buffers[N_CHANNELS];
	uint8_t avg[N_CHANNELS];
	
	enum adc_channels cur_buffer;
} adc_t;

void adc_init (void);
uint8_t* adc_get_values (void);

#endif /* ADC_H_ */