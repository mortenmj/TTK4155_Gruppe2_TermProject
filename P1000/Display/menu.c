/*
 * menu.c
 *
 * Created: 25.09.2013 21:11:14
 *  Author: mortenmj & magnealv
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "menu.h"

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "fsm.h"
#include "u8glib/u8g.h"
#include "serial.h"

struct menu_item *current_menu;
unsigned char sub_selected;
uint8_t button_flag;

static xQueueHandle event_queue;

static void menu_func_play (void)
{
	fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
	event->type = EV_START;
	event->ptr = NULL;
	fsm_event_put (event, portMAX_DELAY);
}

static void menu_func_hiscore (void)
{
	fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
	event->type = EV_HISCORE;
	event->ptr = NULL;
	fsm_event_put (event, portMAX_DELAY);
}

static void menu_func_about (void)
{
	fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
	event->type = EV_ABOUT;
	event->ptr = NULL;
	fsm_event_put (event, portMAX_DELAY);
}

static void menu_func_save (void)
{
	fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
	event->type = EV_SAVE;
	event->ptr = NULL;
	fsm_event_put (event, portMAX_DELAY);
}

static void menu_func_clear (void)
{
	fsm_event_t *event = pvPortMalloc (sizeof (fsm_event_t));
	event->type = EV_CLEAR;
	event->ptr = NULL;
	fsm_event_put (event, portMAX_DELAY);
}

void menu_init (void)
{
    portENTER_CRITICAL();
    {
        event_queue = xQueueCreate (MENU_EVENT_QUEUE_LEN, (unsigned portBASE_TYPE) sizeof (menu_event_t));

	    struct menu_item *root = pvPortMalloc (sizeof (struct menu_item));
	    struct menu_item *play = pvPortMalloc (sizeof (struct menu_item));
	    struct menu_item *hiscore = pvPortMalloc (sizeof (struct menu_item));
		struct menu_item *about = pvPortMalloc (sizeof (struct menu_item));
		struct menu_item *save = pvPortMalloc (sizeof (struct menu_item));
	    struct menu_item *clear = pvPortMalloc (sizeof (struct menu_item));
	    

	    root->num_submenus = 5;
	    root->children[0] = play;
	    root->children[1] = hiscore;
	    root->children[2] = about;
		root->children[3] = save;
		root->children[4] = clear;
	    
	    root->parent = root;

	    strcpy (play->title, "Play");
	    play->num_submenus = 0;
	    play->parent = root;
	    play->command = menu_func_play;

	    strcpy (hiscore->title, "Highscore");
	    hiscore->num_submenus = 0;
	    hiscore->parent = root;
	    hiscore->command = menu_func_hiscore;

	    strcpy (about->title, "About");
	    about->num_submenus = 0;
	    about->parent = root;
	    about->command = menu_func_about;
		
		strcpy (save->title, "Save");
		save->num_submenus = 0;
		save->parent = root;
		save->command = menu_func_save;
		
		strcpy (clear->title, "Clear");
		clear->num_submenus = 0;
		clear->parent = root;
		clear->command = menu_func_clear;

	    current_menu = root;
	    sub_selected = 0;
    }
    portEXIT_CRITICAL();
}

void menu_update (void)
{
	menu_event_t event;
    if (xQueueReceive (event_queue, (void *) &event, 0) == pdFALSE)
    {
        /* No new event */
        return;
    }

	switch (event)
	{
		case LEFT:
		if (current_menu != current_menu->parent)
		{
			current_menu = current_menu->parent;
			sub_selected = 0;
		}
		break;
		case RIGHT:
		/* Attempt to execute menu function */
		if (current_menu->children[sub_selected]->num_submenus == 0 && current_menu->children[sub_selected]->command > 0)
		{
			/* Execute menu command */
			current_menu->children[sub_selected]->command();
			break;
		}

		/* Attempt to navigate to submenu */
		if (current_menu->children[sub_selected]->num_submenus != 0)
		{
			current_menu = current_menu->children[sub_selected];
			sub_selected = 0;
		}

		break;
		case UP:
		if (sub_selected == 0)
		{
			sub_selected = (current_menu->num_submenus - 1);
		}
		else
		{
			sub_selected--;
		}
		break;

		case DOWN:
		if (sub_selected == (current_menu->num_submenus - 1))
		{
			sub_selected = 0;

		}
		else
		{
			sub_selected++;
		}
		break;

		default:
		break;
	}
}

portBASE_TYPE menu_event_put (menu_event_t event, portTickType block_time)
{
	return xQueueSend (event_queue, (void *) &event, block_time);
}

void menu_event_put_from_isr (menu_event_t event)
{
	signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendFromISR (event_queue, (void *) &event, &xHigherPriorityTaskWoken );

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD ();
	}
}

void menu_display (u8g_t *u8g)
{
	uint8_t h;
	u8g_uint_t w, d;
	u8g_uint_t arrow_pos;
	char arrow[2] = ">";

	u8g_SetFont (u8g, u8g_font_6x10);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);

	h = u8g_GetFontAscent(u8g) - u8g_GetFontDescent(u8g);
	w = u8g_GetWidth(u8g);
	arrow_pos = u8g_GetWidth(u8g) - u8g_GetStrWidth(u8g, arrow);;

	for (uint8_t i = 0; i < current_menu->num_submenus; i++)
	{
		/* draw all menu items */
		d = (w - u8g_GetStrWidth(u8g, current_menu->children[i]->title)) / 2;
		u8g_SetDefaultForegroundColor(u8g);

		if ( sub_selected == i )
		{
			/* Highlight current selected menu item */
			u8g_DrawBox(u8g, 0, i*h+1, w, h);
			u8g_SetDefaultBackgroundColor(u8g);
		}

		if (current_menu->children[i]->num_submenus > 0)
		{
			u8g_DrawStr(u8g, arrow_pos, i*h, arrow);
		}

		u8g_DrawStr(u8g, d, i*h, current_menu->children[i]->title);
	 }
}

void menu_about_display (u8g_t *u8g)
{
	u8g_SetFont (u8g, u8g_font_6x10);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);

	u8g_DrawStr(u8g, 5, 10, "Magne Normann");
	u8g_DrawStr(u8g, 5, 25, "Morten Mjelva");
	u8g_DrawStr(u8g, 5, 40, "Powered by FreeRTOS");
}