/*
 * servo.h
 *
 * Created: 30.10.2013 15:41:07
 *  Author: magnealv
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <avr/io.h>

#define SERVO_MIN 900
#define SERVO_MAX 2100
#define PERIOD 20 000

#define PWM_MIN (CTRL_MAX_VAL/PERIOD)*SERVO_MIN
#define PWM_MAX (CTRL_MAX_VAL/PERIOD)*SERVO_MAX
#define CTRL_MAX_VAL 255

void servo_init();

#endif /* SERVO_H_ */