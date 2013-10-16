/*
 * can.h
 *
 * Created: 16.10.2013 10:51:28
 *  Author: mortenmj
 */ 


#ifndef CAN_H_
#define CAN_H_


#include <avr/io.h>
#include "mcp2515.h"

typedef struct {
	uint16_t identifier;
	uint8_t size;
	uint8_t data[8];
	} can_frame_t;


void can_init(uint16_t listenAddress);
uint8_t can_send_frame(can_frame_t* frame);

uint8_t can_recieve_frame(can_frame_t* frame);


#endif /* CAN_H_ */