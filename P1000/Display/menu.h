/*
 * menu.h
 *
 * Created: 25.09.2013 21:11:36
 *  Author: mortenmj & magnealv
 */ 

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include "u8glib/u8g.h"
//#include "joystick.h"

#define MAX_TITLE 14
#define MAX_SUBMENUS 16

struct menu_item
{
	char title[MAX_TITLE];
	void (*command)();
	unsigned char num_submenus;
	struct menu_item *parent;
	struct menu_item *children[MAX_SUBMENUS];
};


typedef enum
{
	IDLE,
	EXCITED,
	N_STATES
} state_t;

typedef struct state_item
{
	state_t state;
	//direction_t direction;
} State_t;

void menu_init (void);
void menu_destroy (void);
void menu_display (u8g_t *u8g);
void menu_update (void);
//void menu_update_fsm(direction_t joystick);

#endif /* MENU_H_ */