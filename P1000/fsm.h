/**
 * @file
 *
 * @ingroup FSM
 *
 * @brief Finite state machine
 */

/*
 * fsm.h
 *
 * Created: 19.11.2013 17:16:56
 *  Author: mortenmj
 */

/**
 * @defgroup FSM FSM
 *
 * @ingroup FSM
 *
 * @brief Finite state machine for the P1000
 *
 * @{
 */

#ifndef FSM_H_
#define FSM_H_

#define ST_ANY		-1
#define ST_MENU		0
#define ST_PLAY		1
#define ST_HISCORE	2
#define ST_ABOUT	3

#define EV_ANY		-1
#define EV_BUTTON	50
#define EV_START	51
#define EV_STOP		53
#define EV_HISCORE	54
#define EV_ABOUT	55
#define EV_SAVE		56
#define EV_CLEAR	57


#define FSM_EVENT_QUEUE_LEN 20

/**
 *  @brief FSM transition
 *
 * Struct that describes a transition in the FSM.
 */
typedef struct
{   /**
	 * @brief State
	 */
	portBASE_TYPE st;

    /**
	 * @brief Event
	 */
	portBASE_TYPE ev;

    /**
	 * @brief Transition function
	 */
	portBASE_TYPE (*fn)(void *ptr);
} fsm_transition_t;

/**
 *  @brief FSM event
 *
 * Variables of this type are posted to the queue. The FSM retrieves events from the queue and reacts to them.
 */
typedef struct
{
    /**
	 * @brief Event type
	 */
	portBASE_TYPE type;

    /**
	* @brief Data pointer
	*/
	void *ptr;
} fsm_event_t;

/**
 * @brief Initialize FSM
 */
void fsm_init (void);

/**
 * @brief Update FSM
 *
 * This will check the event queue for pending events, and react to them if there are any.
 */
void fsm_update (void);

/**
 * @brief Write an FSM event to the event queue
 *
 * @param event The event to write
 * @param block_time The time to wait if the queue is unavailable.
 */
portBASE_TYPE fsm_event_put (fsm_event_t *event, portTickType block_time);

/**
 * @brief Write a menu event to the event queue, ISR version.
 *
 * @param event The event to write
 * @return pdPASS on success, or pdFAIL.
 */
void fsm_event_put_from_isr (fsm_event_t *event);

/**
 * @brief Get FSM state
 *
 * @return The state
 */
portBASE_TYPE fsm_get_state (void);

/** @} */

#endif /* FSM_H_ */