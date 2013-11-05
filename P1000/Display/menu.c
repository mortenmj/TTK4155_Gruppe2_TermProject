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
//#include "joystick.h"
//#include "oled.h"

#include "serial.h"

struct menu_item *current_menu;
unsigned char sub_selected;
State_t current_state;
//direction_t current_direction;
uint8_t button_flag;

void kuk_copy(char *str1, char *str2, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		str1[i] = str2[i];
	}
	str1[len+1] = '\0';
}

void menu_init (void)
{
	vSerialPutString( NULL, "menu_init()\n", 12);

	struct menu_item *root = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub0 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub1 = pvPortMalloc (sizeof (struct menu_item));
	struct menu_item *sub2 = pvPortMalloc (sizeof (struct menu_item));
	
	root->num_submenus = 4;
	root->children[0] = sub0;
	root->children[1] = sub1;
	root->children[2] = sub2;
	root->parent = root;
	
	strcpy (sub0->title, "Submenu 0");
	sub0->num_submenus = 0;
	sub0->parent = root;
	
	strcpy (sub1->title, "Submenu 1");
	sub1->num_submenus = 0;
	sub1->parent = root;
	
	strcpy (sub2->title, "Submenu 2");;
	sub2->num_submenus = 0;
	sub2->parent = root;

	current_menu = root;
	sub_selected = 0;
}

void menu_display (u8g_t *u8g)
{
	uint8_t i, h;
	u8g_uint_t w, d;
	u8g_SetFont (u8g, u8g_font_5x8);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);
	
	h = u8g_GetFontAscent(u8g) - u8g_GetFontDescent(u8g);
	w = u8g_GetWidth(u8g);
	
	for (uint8_t i = 0; i < 3; i++)
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

		u8g_DrawStr(u8g, d, i*h, current_menu->children[i]->title);
	 }
}

/* TODO: FIX joystick timer  */
/*
void menu_update()
{	
	if(!button_flag)	// if pushed (active low)
	{
		if (current_menu.children[current_menu.child_selected]->num_submenus == 0)
		{
			/ * Execute menu command * /
			current_menu.children[current_menu.child_selected]->command();
		}
		current_menu = *current_menu.children[current_menu.child_selected];	
		button_flag = 1;
	}
	
	switch(current_direction)
	{
		case LEFT:
			current_menu = *current_menu.parent;
			break;
		case RIGHT:
			if (current_menu.children[current_menu.child_selected]->num_submenus == 0)
			{
				/ * Execute menu command * /
				current_menu.children[current_menu.child_selected]->command();
			}
			current_menu = *current_menu.children[current_menu.child_selected];
			break;
		case UP:
			if(current_menu.child_selected == 0)
			{	
				current_menu.child_selected = current_menu.num_submenus; 
				break;
			}
			else
			{ 
				current_menu.child_selected--; 
				break;
			}
		case DOWN:
			if(current_menu.child_selected == current_menu.num_submenus)
			{	
				current_menu.child_selected = 0; 
				break;
			}
			else
			{ 
				current_menu.child_selected++; 
				break;
			}
		default:
			break;
	}
}

void menu_update_fsm(direction_t joystick){
	current_direction = joystick;
	if (current_state.direction == current_direction)
	{
		//do nothing
	}
	else
	{
		if (current_direction == CENTER)
		{
			current_state.direction = current_direction;
			current_state.state = IDLE;
		}
		else
		{
			//set_timer()
			current_state.direction = current_direction;
			current_state.state = EXCITED;
			menu_update();
		}
	}
}

ISR (TIMER3_COMPA_vect)
{
	current_state.direction = CENTER;
	current_state.state = IDLE;
}*/