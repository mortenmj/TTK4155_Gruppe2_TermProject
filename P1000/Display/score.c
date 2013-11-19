/*
 * score.c
 *
 * Created: 19.11.2013 13:03:03
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "event.h"
#include "u8glib/u8g.h"

portLONG score_val;
xSemaphoreHandle score_sem;
char score_string[10];
xTimerHandle score_timer;


event_t evnt = {EVENT_SCORE, 0};

static void score_update (void)
{
	score_val += 100;
	
	xSemaphoreTake (score_sem, 0);
	ltoa (score_val, score_string, 10);
	event_put (&evnt, 0);
	xSemaphoreGive (score_sem);
}

void score_init (void)
{
	vSemaphoreCreateBinary(score_sem);
	score_timer = xTimerCreate ("Score timer", 1000 / portTICK_RATE_MS, pdTRUE, 0, score_update);
}

void score_display (u8g_t *u8g)
{
	u8g_SetFont (u8g, u8g_font_9x18);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);
	
	u8g_DrawStr(u8g, 20, 20, score_string);
}

void score_lock (void)
{
	xSemaphoreTake (score_sem, portMAX_DELAY );
}

void score_reset (void)
{
	/* TODO: Check if score is new highscore */
	score_val = 0;
	xTimerStop (score_timer, 0);
}

void score_start (void)
{
	xTimerReset ( score_timer, 0);
	xTimerStart ( score_timer, 0);
}

void score_unlock (void)
{
	xSemaphoreGive (score_sem);
}