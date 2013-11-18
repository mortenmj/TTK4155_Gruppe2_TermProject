/*
 * buttons.h
 *
 * Created: 18.11.2013 11:22:24
 *  Author: mortenmj
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_



void buttons_init (void);
uint8_t buttons_get (uint8_t *button, portTickType block_time);

#endif /* BUTTONS_H_ */