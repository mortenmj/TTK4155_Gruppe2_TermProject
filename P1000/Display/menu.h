/**
 * @file
 *
 * @ingroup Menu
 *
 * @brief Menu sub-module
 */

/*
 * menu.h
 *
 * Created: 25.09.2013 21:11:36
 *  Author: mortenmj & magnealv
 */

#ifndef MENU_H_
#define MENU_H_

/**
 * @defgroup Menu Menu module
 *
 * @ingroup Display
 *
 * @brief Provides functions for creating and updating the menu
 *
 * @{
 */

#include <stdint.h>
#include "u8glib/u8g.h"

#define MAX_TITLE 14
#define MAX_SUBMENUS 16
#define MENU_EVENT_QUEUE_LEN 10

/**
 *  @brief Menu item
 *
 * Struct that describes an item in the menu
 */
struct menu_item
{
	/**
	* @brief Menu item title
	*/
	char title[MAX_TITLE];
	
	/**
	 * @brief Command to execute on menu item activation
	 */
	void (*command)();
	
	/**
	 * @brief Number of submenus for this node
	 */
	unsigned char num_submenus;
	
	/**
	 * @brief Parent node for this item
	 */
	struct menu_item *parent;
	
	/**
	 * @brief List of child nodes for this node
	 */
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

/**
 * @brief Initialize menu
 */
void menu_init (void);

/**
 * @brief Update menu
 */
void menu_update (void);

/**
 * @brief Write a menu event to the event queue
 *
 * @param event The event to write
 * @param block_time The time to wait if the queue is unavailable.
 */
portBASE_TYPE menu_event_put (menu_event_t event, portTickType block_time);

/**
 * @brief Write a menu event to the event queue, ISR version.
 *
 * @param event The event to write
 * @return pdPASS on success, or pdFAIL.
 */
void menu_event_put_from_isr (menu_event_t event);

/**
 * @brief Function to display the menu
 *
 * @param u8g The display device
 */
void menu_display (u8g_t *u8g);

/**
 * @brief Function to display the highscore
 *
 * @param u8g The display device
 */
void menu_hiscore_display (u8g_t *u8g);

/**
 * @brief Function to display the about page
 *
 * @param u8g The display device
 */
void menu_about_display (u8g_t *u8g);

/** @} */

#endif /* MENU_H_ */