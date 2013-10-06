/*
 * menu.c
 *
 * Created: 25.09.2013 21:11:14
 *  Author: mortenmj & magnealv
 */ 

#include "menu.h"

#include <stdint.h>
#include <stdlib.h>
#include "oled.h"

menu_t menu_create (void)
{ 
	menu_t sub1 = { "Submenu 1", NULL, 0, 0, NULL, NULL};
	menu_t sub2 = { "Submenu 2", NULL, 0, 0, NULL, NULL};
	menu_t sub3 = { "Submenu 3", NULL, 0, 0, NULL, NULL};
		
	menu_t main_menu = { "Main menu", NULL, 3, 0, NULL, {&sub1, &sub2, &sub3} };
		
	sub1.parent = &main_menu;
	sub2.parent = &main_menu;
	sub3.parent = &main_menu;
		
	return main_menu;
}

void menu_display (menu_t *m)
{
	oled_set_line (0);
	oled_print(m->title,0);
	
	for (uint8_t i = 0; i < m->num_submenus; i++)
	{
		oled_set_line (i+1);
		oled_print("  ", 0);
		if(m->child_selected == i)			
		{ 
			/* Print out highlighted */
			oled_print(m->children[i]->title, 1);
		}
		else
		{
			oled_print(m->children[i]->title, 0);
		}
	}
}

/* TODO: FIX joystick timer  */
menu_t* menu_update(menu_t *m, direction_t joystick, uint8_t button)
{
	if(!button)	// if pushed (active low)
	{
		if (m->children[m->child_selected]->num_submenus == 0)
		{
			/* Execute menu command */
			m->children[m->child_selected]->command();
			return m;
		}
		return m->children[m->child_selected];		
	}
	
	switch(joystick)
	{
		case LEFT:
			return m->parent;
		case RIGHT:
			if (m->children[m->child_selected]->num_submenus == 0)
			{
				/* Execute menu command */
				m->children[m->child_selected]->command();
				return m;
			}
			return m->children[m->child_selected];
		case UP:
			if(m->child_selected == 0)
			{	
				m->child_selected = m->num_submenus; 
			}
			else
			{ 
				m->child_selected--; 
			}
			return m;
		case DOWN:
			if(m->child_selected == m->num_submenus)
			{	
				m->child_selected = 0; 
			}
			else
			{ 
				m->child_selected++; 
			}
			return m;
		default:
			return m;
	}
}