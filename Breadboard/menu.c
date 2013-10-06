/*
 * menu.c
 *
 * Created: 25.09.2013 21:11:14
 *  Author: mortenmj & magnealv
 */ 

#include "menu.h"

#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "joystick.h"
#include "oled.h"

menu_t current_menu;
State_t current_state;
direction_t current_direction;
uint8_t button_flag;

void menu_init (void)
{ 
	menu_t sub1 = { "Submenu 1", NULL, 0, 0, NULL, NULL};
	menu_t sub2 = { "Submenu 2", NULL, 0, 0, NULL, NULL};
	menu_t sub3 = { "Submenu 3", NULL, 0, 0, NULL, NULL};
		
	menu_t main_menu = { "Main menu", NULL, 3, 0, NULL, {&sub1, &sub2, &sub3} };
		
	sub1.parent = &main_menu;
	sub2.parent = &main_menu;
	sub3.parent = &main_menu;
		
	current_menu = main_menu;
	
	/* Set up timer to trigger menu update */
	/* Set CTC mode and 1024 prescale */
	TCCR3B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
		 
	/* Enable timer 3: Output Compare A Match Interrupt Enable*/
	ETIMSK |= (1 << OCIE3A);
		 
	/* With F_CPU / PRESCALE / SPS */
	/* Trigger interrupt every 20 ms */
	OCR3A = 960;
}

void menu_display ()
{
	oled_set_line (0);
	oled_print(current_menu.title,0);
	
	for (uint8_t i = 0; i < current_menu.num_submenus; i++)
	{
		oled_set_line (i+1);
		oled_print("  ", 0);
		if(current_menu.child_selected == i)			
		{ 
			/* Print out highlighted */
			oled_print(current_menu.children[i]->title, 1);
		}
		else
		{
			oled_print(current_menu.children[i]->title, 0);
		}
	}
}

/* TODO: FIX joystick timer  */
void menu_update()
{	
	if(!button_flag)	// if pushed (active low)
	{
		if (current_menu.children[current_menu.child_selected]->num_submenus == 0)
		{
			/* Execute menu command */
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
				/* Execute menu command */
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
}