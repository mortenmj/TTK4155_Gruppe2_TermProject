/*
 * fsm.h
 *
 * Created: 19.11.2013 17:16:56
 *  Author: mortenmj
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

typedef struct
{
	portBASE_TYPE st;
	portBASE_TYPE ev;
	portBASE_TYPE (*fn)(void *ptr);
} fsm_transition_t;

typedef struct
{
	portBASE_TYPE type;
	void *ptr;
} fsm_event_t;

void fsm_init (void);
void fsm_update (void);
portBASE_TYPE fsm_event_put (fsm_event_t *event, portTickType block_time);
void fsm_event_put_from_isr (fsm_event_t *event);

#endif /* FSM_H_ */