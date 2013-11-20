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
 * @defgroup ADC ADC driver
 *
 * @ingroup Drivers
 *
 * @brief High-level ADC driver
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

/**
 * @brief Initialize ADC
 */
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
 * @brief Blocks until all ADC conversions are finished.
 */
unsigned char adc_conversion_complete ( void );

/**
 * @brief Start a new round of conversions.
 *
 * The ADC interrupt will iterate through all available ADC channels and queue the results. Once the process is complete, the interrupt will give
 * a semaphore which can be claimed with adc_take_semaphore () so that the queued values may be retrieved with adc_get_value ().
 */
void adc_conversion_start ( void );

/**
 * @brief Enable the ADC.
 */
void adc_enable ( void );

/**
 * @brief Disable the ADC.
 */
void adc_disable ( void );

/** @} */

#endif /* ADC_H_ */