/*
 * fsm.c
 *
 * Created: 19.11.2013 17:16:45
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "Display/display.h"
#include "Display/menu.h"
#include "Display/score.h"

#include "fsm.h"
#include "serial.h"

menu_event_t *menu_event;

static portBASE_TYPE st_menu_ev_button (void *ptr)
{
	menu_event_t btn = *((menu_event_t *) ptr);
	menu_event_put (btn, 0);
	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_button\n", 18);

	return ST_MENU;
}

static portBASE_TYPE st_menu_ev_start (void *ptr)
{
	vDisplaySetDisplayFunc (&score_display);
    xTaskHandle disp = xDisplayGetHandle ();
	vTaskResume (disp);
	score_start ();

	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_start\n", 17);

	return ST_PLAY;
}

static portBASE_TYPE st_menu_ev_hiscore (void *ptr)
{
	vDisplaySetDisplayFunc (&score_hiscore_display);
	xTaskHandle disp = xDisplayGetHandle ();
    vTaskResume (disp);

	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_hiscore\n", 19);

	return ST_HISCORE;
}

static portBASE_TYPE st_menu_ev_about (void *ptr)
{
	vDisplaySetDisplayFunc (&menu_about_display);
	xTaskHandle disp = xDisplayGetHandle ();
	vTaskResume (disp);

	vSerialPutString (NULL, (const signed char * const) "lol\n", 4);
	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_about\n", 17);

	return ST_ABOUT;
}

static portBASE_TYPE st_menu_ev_save (void *ptr)
{
	score_save ();
	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_save\n", 16);

	return ST_MENU;
}

static portBASE_TYPE st_menu_ev_clear (void *ptr)
{
	score_clear ();
	vSerialPutString (NULL, (const signed char * const) "st_menu_ev_clear\n", 17);

	return ST_MENU;
}

static portBASE_TYPE st_play_ev_stop (void *ptr)
{
	score_reset ();
	vDisplaySetDisplayFunc (&menu_display);
    xTaskHandle disp = xDisplayGetHandle ();
    vTaskResume (disp);

	vSerialPutString (NULL, (const signed char * const) "st_play_ev_stop\n", 16);

	return ST_MENU;
}

static portBASE_TYPE st_hiscore_ev_button (void *ptr)
{
	vDisplaySetDisplayFunc (&menu_display);
    xTaskHandle disp = xDisplayGetHandle ();
    vTaskResume (disp);

	vSerialPutString (NULL, (const signed char * const) "st_hiscore_ev_button\n", 21);

	return ST_MENU;
}

static portBASE_TYPE st_about_ev_button (void *ptr)
{
	vDisplaySetDisplayFunc (&menu_display);
    xTaskHandle disp = xDisplayGetHandle ();
    vTaskResume (disp);

	vSerialPutString (NULL, (const signed char * const) "st_about_ev_button\n", 18);

	return ST_MENU;
}

static portBASE_TYPE st_any_ev_any (void *ptr)
{
	vDisplaySetDisplayFunc (&menu_display);
    xTaskHandle disp = xDisplayGetHandle ();
    vTaskResume (disp);
	
	vSerialPutString(NULL, (const signed char *const) "st_any_ev_any\n", 14);

	return ST_MENU;
}

fsm_transition_t trans[] = {
	{ ST_MENU,		EV_BUTTON,	&st_menu_ev_button		},
	{ ST_MENU,		EV_START,	&st_menu_ev_start		},
	{ ST_MENU,		EV_HISCORE,	&st_menu_ev_hiscore		},
	{ ST_MENU,		EV_ABOUT,	&st_menu_ev_about		},
	{ ST_MENU,		EV_SAVE,	&st_menu_ev_save		},
	{ ST_MENU,		EV_CLEAR,	&st_menu_ev_clear		},
	{ ST_PLAY,		EV_STOP,	&st_play_ev_stop		},
	{ ST_HISCORE,	EV_BUTTON,	&st_hiscore_ev_button	},
	{ ST_ABOUT,		EV_BUTTON,	&st_about_ev_button		},
	{ ST_ANY,		EV_ANY,		&st_any_ev_any			}
};

#define TRANS_COUNT (sizeof(trans)/sizeof(*trans))

static xQueueHandle event_queue;
static portBASE_TYPE state;

void fsm_init (void)
{
	portENTER_CRITICAL();
	{
		vSerialPutString(NULL, "fsm init\n", 9);
		state = ST_MENU;
		event_queue = xQueueCreate (FSM_EVENT_QUEUE_LEN, (unsigned portBASE_TYPE) sizeof (fsm_event_t *));
		if (event_queue == 0)
		{
			vSerialPutString (NULL, "FSM queue create failed\n", 24);
		}
		
		menu_event = pvPortMalloc (sizeof (menu_event_t));
	}
	portEXIT_CRITICAL();
}

void fsm_update (void)
{
	fsm_event_t *event;
	
	if (xQueueReceive (event_queue, &event, 0) == pdFALSE)
	{
		/* No new event */
		return;
	}
	
    for (int i = 0; i < TRANS_COUNT; i++)
	{
	    if ((state == trans[i].st) || (ST_ANY == trans[i].st))
		{
		    if ((event->type == trans[i].ev) || (EV_ANY == trans[i].ev))
			{
			    state = (trans[i].fn)(event->ptr);
				vPortFree(event->ptr);
				vPortFree (event);
			    break;
		    }
	    }
    }
	
	vPortFree((event)->ptr);
	vPortFree (event);
}

portBASE_TYPE fsm_event_put (fsm_event_t *event, portTickType block_time)
{
	return xQueueSend (event_queue, (void *) &event, block_time);
}

void fsm_event_put_from_isr (fsm_event_t *event)
{
	signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	xQueueSendFromISR (event_queue, (void *) &event, &xHigherPriorityTaskWoken );

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD ();
	}
}

portBASE_TYPE fsm_get_state (void)
{
	return state;
}