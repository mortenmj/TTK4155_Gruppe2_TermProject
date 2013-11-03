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

#define CTRL_MIN_VAL 0
#define CTRL_MAX_VAL 255
#define SERVO_MIN 920
#define SERVO_MAX 2200

void servo_init (void);
uint16_t servo_val (uint8_t joy_val);

#endif /* SERVO_H_ */