/*
 * ir_sensor.h
 *
 * Created: 02.11.2013 12:35:50
 *  Author: Normann
 */ 


#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define IR_TRESHOLD 25

typedef struct ir_state
{
	bool low;
	bool changed;
} ir_state_t;

void ir_sensor_init (void);

#endif /* IRSENSOR_H_ */