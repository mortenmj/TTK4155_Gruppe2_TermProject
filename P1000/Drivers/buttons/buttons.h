/**
 * @file
 *
 * @ingroup Buttons
 *
 * @brief Button driver
 */

/*
 * buttons.h
 *
 * Created: 18.11.2013 11:22:24
 *  Author: mortenmj
 */

/**
 * @defgroup Buttons Button driver
 *
 * @ingroup Drivers
 *
 * @brief Button driver
 *
 * @{
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON_UP		0
#define BUTTON_DOWN		1
#define BUTTON_LEFT		2
#define BUTTON_RIGHT	3

/**
 * @brief Initializes button manager
 *
 * This sets up a timer which samples the buttons periodically, debounces the input and then generates user input events for the FSM.
 */
void buttons_init (void);

/** @} */

#endif /* BUTTONS_H_ */