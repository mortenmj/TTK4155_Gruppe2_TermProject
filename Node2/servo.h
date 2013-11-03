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
#define PERIOD 20000
#define CTRL_MAX_VAL 255

#define PWM_MIN CTRL_MAX_VAL*(SERVO_MIN/PERIOD)
#define PWM_MAX CTRL_MAX_VAL*(SERVO_MAX/PERIOD)

void servo_init();

#endif /* SERVO_H_ */