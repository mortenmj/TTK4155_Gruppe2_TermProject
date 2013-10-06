/*
 * joystick.h
 *
 * Created: 29.09.2013 29:09:36
 *  Author: magnealv
 */ 
#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "adc.h"

#define JOYSTICK_EQUILIBRIUM (ADC_MAX_VAL + ADC_MIN_VAL)/2
#define JOYSTICK_SLACK 5

typedef enum {UP, DOWN, LEFT, RIGHT, CENTER} direction_t;

uint8_t getpercentage(uint8_t value);

direction_t getdirection(adc_t values);


#endif /* JOYSTICK_H_ */
