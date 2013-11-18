/*
 * menu.c
 *
 * Created: 25.09.2013 21:11:14
 *  Author: mortenmj & magnealv
 */ 

#include "FreeRTOS.h"
#include "task.h"

#include "menu.h"

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"

struct menu_item *current_menu;
unsigned char sub_selected;
uint8_t button_flag;

static void test1 (void)
{
	vSerialPutString(NULL, "hello\n", 6);
}

static void test2 (void)
{
	vSerialPutString(NULL, "morten er best\n", 15);
}

static void test3 (void)
{
	vSerialPutString(NULL, "hurra\n", 6);
}
void menu_init (void)
{
	vSerialPutString( NULL, "menu_init()\n", 12);

	struct menu_item *root = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub0 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub1 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub2 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub3 = pvPortMalloc (sizeof (struct menu_item));
	
	struct menu_item *sub21 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub22 = pvPortMalloc (sizeof (struct menu_item));
	
	root->num_submenus = 4;
	root->children[0] = sub0;
	root->children[1] = sub1;
	root->children[2] = sub2;
	root->children[3] = sub3;
	root->parent = root;
	
	strcpy (sub0->title, "Start");
	sub0->num_submenus = 0;
	sub0->parent = root;
	sub0->command = test1;
	
	strcpy (sub1->title, "Stop");
	sub1->num_submenus = 0;
	sub1->parent = root;
	sub1->command = test2;
	
	strcpy (sub2->title, "Reset");;
	sub2->num_submenus = 2;
	sub2->parent = root;
	
	strcpy (sub3->title, "Test");;
	sub3->num_submenus = 0;
	sub3->parent = root;
	
	sub2->children[0] = sub21;
	sub2->children[1] = sub22;
	
	strcpy (sub21->title, "Submenu 2-1");;
	sub21->num_submenus = 0;
	sub21->parent = sub2;

	strcpy (sub22->title, "Submenu 2-2");;
	sub22->num_submenus = 0;
	sub22->parent = sub2;
	sub22->command = test3;

	current_menu = root;
	sub_selected = 0;
}

void menu_display (u8g_t *u8g)
{
	uint8_t i, h;
	u8g_uint_t w, d;
	u8g_uint_t arrow_pos;
	char arrow[2] = ">";
	
	u8g_SetFont (u8g, u8g_font_9x18);
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

void menu_update (uint8_t event)
{
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