/*
 * menu.h
 *
 * Created: 25.09.2013 21:11:36
 *  Author: mortenmj & magnealv
 */ 

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include "joystick.h"

#define MAX_TITLE 14
#define MAX_SUBMENUS 16

typedef struct menu_item
{
	char title[MAX_TITLE];
	void (*command)();
	
	uint8_t num_submenus;
	uint8_t child_selected;
	struct menu_item *parent;
	struct menu_item *children[MAX_SUBMENUS];
} menu_t;

menu_t menu_create (void);
void menu_destroy (void);
void menu_display (menu_t *m);
menu_t* menu_update (menu_t *m, direction_t joystick, uint8_t button);

#endif /* MENU_H_ */