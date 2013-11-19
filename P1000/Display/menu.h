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

#define MAX_TITLE 14
#define MAX_SUBMENUS 16
#define MENU_EVENT_QUEUE_LEN 10

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
	UP = 2,
	DOWN = 3,
	LEFT = 0,
	RIGHT = 1,
	N_MENU_EVENTS
} menu_event_t;

void menu_init (void);
void menu_update (void);

portBASE_TYPE menu_event_put (menu_event_t event, portTickType block_time);
void menu_event_put_from_isr (menu_event_t event);

void menu_display (u8g_t *u8g);
void menu_hiscore_display (u8g_t *u8g);
void menu_about_display (u8g_t *u8g);

#endif /* MENU_H_ */