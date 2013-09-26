/*
 * menu.h
 *
 * Created: 25.09.2013 21:11:36
 *  Author: mortenmj
 */ 

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

#define MAX_TITLE 14
#define MAX_SUBMENUS 16

typedef struct menu_item
{
	char title[MAX_TITLE];
	void (*command)();
	
	uint8_t num_submenus;
	struct menu_item *parent;
	struct menu_item *children[MAX_SUBMENUS];
} menu_t;

menu_t menu_create (void);
void menu_destroy (void);
void menu_display (menu_t *m);
void menu_navigate (menu_t *m);

#endif /* MENU_H_ */