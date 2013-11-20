/**
 * @file
 *
 * @ingroup Control
 *
 * @brief Control module
 */

/*
 * control.h
 *
 * Created: 02.10.2013 23:26:59
 *  Author: mortenmj
 */

/**
 * @defgroup Control Control module
 *
 * @ingroup Control
 *
 * @brief Provides overall control of the board, and communication with other nodes
 *
 * @{
 */

#ifndef CONTROL_H_
#define CONTROL_H_

typedef enum
{
    GAME_START = 0,
    GAME_SENSOR_TRIGGERED = 1,
    GAME_SENSOR_CLEARED = 2,
    N_GAME_EVENTS
} game_event_t;

/**
 * @brief The main function for the Control module
 *
 * @param[in] pvParameters Parameter for the task being created. See FreeRTOS documentation for details.d
 *
 */
void vControl ( void *pvParameters );

/**
 * @brief Retrieve the task handle.
 *
 * @return The xTaskHandle for the Control task.
 *
 */
xTaskHandle xControlGetHandle (void);

/**
 * @brief Set the task handle.
 *
 * @param handle The xTaskHandle for the Control task.
 *
 */
void vControlSetHandle (xTaskHandle handle);

/** @} */

#endif /* CONTROL_H_ */