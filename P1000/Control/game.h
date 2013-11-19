/*
 * game.h
 *
 * Created: 19.11.2013 14:50:01
 *  Author: mortenmj
 */ 


#ifndef GAME_H_
#define GAME_H_

typedef enum
{
	GAME_START = 0,
	GAME_SENSOR_TRIGGERED = 1,
	GAME_SENSOR_CLEARED = 2,
	N_GAME_EVENTS
} game_event_t;

#endif /* GAME_H_ */