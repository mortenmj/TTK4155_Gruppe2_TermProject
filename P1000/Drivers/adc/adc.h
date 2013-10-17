/**
 * @file
 *
 * @ingroup ADC
 *
 * @brief High-level ADC API
 */

/*
 * adc.h
 *
 * Created: 05.09.2013 15:03:02
 *  Author: mortenmj
 */ 

#ifndef ADC_H_
#define ADC_H_

/**
 * @defgroup ADC ADC API
 *
 * @ingroup Control
 *
 * @brief Provides functions for the ADC
 *
 * @{
 */

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

/**
 * @brief Retrieve a value from the ADC conversion queue
 *
 * @param[out] val Address where the ADC value is written
 * @param block_time How long to block when attempting to read the queue
 *
 */
signed char adc_get_value ( signed char *val, portTickType block_time );

/**
 * @brief Start a new round of conversions.
 *
 * The ADC interrupt will iterate through all available ADC channels and queue the results. Once the process is complete, the interrupt will give
 * a semaphore which can be claimed with adc_take_semaphore () so that the queued values may be retrieved with adc_get_value ().
 */
void adc_start_conversion ( void );

/**
 * @brief Takes the semaphore that locks the ADC converted values queue.
 */
unsigned char adc_take_semaphore ( void );

void adc_close ( void );

#endif /* ADC_H_ */