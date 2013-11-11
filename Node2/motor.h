/*
 * motor.h
 *
 * Created: 06.11.2013 13:18:48
 *  Author: mortenmj
 */ 

#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init (void);
int16_t motor_read (void);
void motor_write (uint8_t val);
void motor_update (uint8_t output);

#endif /* MOTOR_H_ */