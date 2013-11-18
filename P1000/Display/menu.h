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

enum menu_event
{
	UP = 3,
	DOWN = 2,
	LEFT = 0,
	RIGHT = 1,
	N_EVENTS
};

void menu_init (void);
void menu_destroy (void);
void menu_display (u8g_t *u8g);
void menu_update (uint8_t event);

#endif /* MENU_H_ */