/**
 * @file
 *
 * @ingroup Display
 *
 * @brief Display module
 */

/*
 * display.h
 *
 * Created: 02.10.2013 23:45:00
 *  Author: mortenmj
 */


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "u8glib/u8g.h"

/**
 * @brief The main function for the Display module
 *
 * @param[in] pvParameters Parameter for the task being created. See FreeRTOS documentation for details.d
 *
 */
void vDisplay ( void *pvParameters );

/**
 * @brief Convenience function to retrieve the task handle.
 *
 * @return The xTaskHandle for the Display task.
 *
 */
xTaskHandle xDisplayGetHandle (void);

/**
 * @brief Convenience function to set the task handle.
 *
 * @param handle The xTaskHandle for the Display task.
 *
 */
void vDisplaySetHandle (xTaskHandle handle);

/**
 * @brief Set the display handle function.
 *
 * @param func The function to use for updating the display.
 *
 */
void vDisplaySetDisplayFunc (void (*func)(u8g_t *u8g));

#endif /* DISPLAY_H_ */