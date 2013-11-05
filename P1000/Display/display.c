/*
 * display.c
 *
 * Created: 02.10.2013 23:45:09
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "task.h"

#include "u8glib/u8g.h"
#include "display.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 20 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

u8g_t u8g;

void draw(void) {
	// graphic commands to redraw the complete screen should be placed here
	u8g_SetFont (&u8g, u8g_font_5x8);
	
	//u8g_DrawStr(&u8g, 20, 20, "Hello World!");
}

void vDisplay ( void *pvParameters )
{
	portTickType xLastWakeTime;
	vSerialPutString( NULL, "disp\n", 5);
	xLastWakeTime = xTaskGetTickCount ();
	
	menu_init ();
	
	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );
	
	
	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);

	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, dispTASK_FREQUENCY );
		
		u8g_FirstPage(&u8g);
		do
		{
			menu_display (&u8g);
		} while ( u8g_NextPage( &u8g ) );
	}
}
