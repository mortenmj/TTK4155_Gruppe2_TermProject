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

#include <stdlib.h>
#include <avr/eeprom.h>

#include "u8glib/u8g.h"

#define NUM_HISCORES 3

portLONG score_val;
xSemaphoreHandle score_sem;
char score_string[10];
xTimerHandle score_timer;

portLONG EEMEM eeprom_hiscores[NUM_HISCORES];
portLONG hiscores[NUM_HISCORES];
portCHAR chiscores[3][10];

static void score_update (xTimerHandle xTimer)
{
	score_val += 10;

	xSemaphoreTake (score_sem, 0);
	ltoa (score_val, score_string, 10);
	xSemaphoreGive (score_sem);
}

void score_init (void)
{
	vSemaphoreCreateBinary(score_sem);
	score_timer = xTimerCreate ((const signed char * const) "Score timer", 2500 / portTICK_RATE_MS, pdTRUE, 0, score_update);

	eeprom_read_block ((void *) &hiscores, (const void *) &eeprom_hiscores, NUM_HISCORES * sizeof (portLONG));
	ltoa (hiscores[0], chiscores[0], 10);
	ltoa (hiscores[1], chiscores[1], 10);
	ltoa (hiscores[2], chiscores[2], 10);	
}

void score_hiscore_display (u8g_t *u8g)
{
	u8g_SetFont (u8g, u8g_font_6x10);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);

	u8g_DrawStr(u8g, 5, 5, "Highscores:");
	u8g_DrawStr(u8g, 15, 20, "1.");
	u8g_DrawStr(u8g, 35, 20, chiscores[0]);
	u8g_DrawStr(u8g, 15, 35, "2.");
	u8g_DrawStr(u8g, 35, 35, chiscores[1]);
	u8g_DrawStr(u8g, 15, 50, "3.");
	u8g_DrawStr(u8g, 35, 50, chiscores[2]);
}

void score_display (u8g_t *u8g)
{
	u8g_SetFont (u8g, u8g_font_9x18);
	u8g_SetFontRefHeightText(u8g);
	u8g_SetFontPosTop(u8g);

	u8g_DrawStr(u8g, 5, 5, "Score:");
	u8g_DrawStr(u8g, 15, 20, score_string);
}

void score_start (void)
{
	xTimerReset ( score_timer, 0);
	xTimerStart ( score_timer, 0);
}

void score_stop (void)
{
	xTimerStop ( score_timer, 0);
}

void score_reset (void)
{
	/* See if our value is higher than one in the highscore list */
	int i = 0;
	for (i = 0; i < NUM_HISCORES; i++)
	{
		if (score_val > hiscores[i])
		{
			break;
		}			
	}
	
	for (int j = NUM_HISCORES; j > i; j--)
	{
		hiscores[j] = hiscores[j-1];
	}
	
	hiscores[i] = score_val;
	
	ltoa (hiscores[0], chiscores[0], 10);
	ltoa (hiscores[1], chiscores[1], 10);
	ltoa (hiscores[2], chiscores[2], 10);
	
	score_val = 0;
	xTimerStop (score_timer, 0);
}

void score_save (void)
{
	eeprom_write_block ((const void *) &hiscores, (void *) &eeprom_hiscores, NUM_HISCORES * sizeof (portLONG));
}

void score_clear (void)
{
	hiscores[0] = 0;
	hiscores[1] = 0;
	hiscores[2] = 0;
	ltoa (hiscores[0], chiscores[0], 10);
	ltoa (hiscores[1], chiscores[1], 10);
	ltoa (hiscores[2], chiscores[2], 10);
	
	score_save ();
}