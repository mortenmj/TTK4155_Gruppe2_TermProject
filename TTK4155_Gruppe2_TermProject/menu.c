/*
 * menu.c
 *
 * Created: 25.09.2013 21:11:14
 *  Author: mortenmj
 */ 

#include "menu.h"

#include <stdint.h>
#include <stdlib.h>

menu_t menu_create (void)
{
	menu_t sub1 = { "Submenu 1", NULL, 0, NULL, NULL};
	menu_t sub2 = { "Submenu 2", NULL, 0, NULL, NULL};
	menu_t sub3 = { "Submenu 3", NULL, 0, NULL, NULL};
		
	menu_t main_menu = { "Main menu", NULL, 3, NULL, {&sub1, &sub2, &sub3} };
		
	sub1.parent = &main_menu;
	sub2.parent = &main_menu;
	sub3.parent = &main_menu;
		
	return main_menu;
}

void menu_display (menu_t *m)
{
	oled_set_line (0);
	oled_print(m->title);
	
	for (uint8_t i = 0; i < m->num_submenus; i++)
	{
		oled_set_line (i+1);
		oled_print("  ");		
		oled_print(m->children[i]->title);
	}
}

void menu_navigate (menu_t *m)
{
	if (m->num_submenus == 0)
	{
		/* Execute menu command */
		m->command();
		return;
	}
	
	/* Display submenus */
	menu_display (m);
	
	/*
	m = menu_get_selection (m);
	*/
	
	return m;
}