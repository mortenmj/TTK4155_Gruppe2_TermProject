/*
 * display.c
 *
 * Created: 02.10.2013 23:45:09
 *  Author: mortenmj
 */

#include "FreeRTOS.h"
#include "task.h"

#include "u8glib/u8g.h"
#include "m2tklib/m2.h"
#include "m2tklib/m2ghu8g.h"
#include "display.h"

/* Task frequency */
#define dispTASK_FREQUENCY		( ( const portTickType ) 200 )

/* Delay needed before initializing display */
#define dispINIT_DELAY			1000

/*=========================================================================*/
/* Forward declaration of the toplevel element */
M2_EXTERN_ALIGN(top_el_x2l_menu);


/*=========================================================================*/
/* Define three user menus, just for demonstration. */

M2_ROOT(el_mnu1_sel, "t1", "Menu 1 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu1_sel, "-1|1W64H64", &el_mnu1_sel);

M2_ROOT(el_mnu2_sel, "t1", "Menu 2 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu2_sel, "-1|1W64H64", &el_mnu2_sel);

M2_ROOT(el_mnu3_sel, "t1", "Menu 3 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu3_sel, "-1|1W64H64", &el_mnu3_sel);


/*=========================================================================*/
uint8_t value = 0;
char buf[20];

/* define callback procedure, which returns a menu entry with a value */
const char *xmenu_value(uint8_t idx, uint8_t msg)
{
	if ( msg == M2_STRLIST_MSG_GET_STR ) {
		strcpy(buf, " Value: ");
		itoa((int)value, buf+strlen(buf), 10);
		return buf;
	}
	return "";
}

/* define callback procedures which increment and decrement a value */
const char *xmenu_inc(uint8_t idx, uint8_t msg) {
	if ( msg == M2_STRLIST_MSG_SELECT  ) {
		value++;
	}
	return "";
}

const char *xmenu_dec(uint8_t idx, uint8_t msg) {
	if ( msg == M2_STRLIST_MSG_SELECT  ) {
		value--;
	}
	return "";
}

/*=========================================================================*/
/* this is the overall menu structure for the X2L Menu */

m2_xmenu_entry xmenu_data[] =
{
{ "Menu 1", NULL, NULL },             /* expandable main menu entry */
{ ".", NULL, xmenu_value },           /* The label of this menu line is returned by the callback procedure */
{ ". Inc", NULL, xmenu_inc },         /* This callback increments the value */
{ ". Dec", NULL, xmenu_dec },         /* This callback decrements the value */
{ "Menu 2", NULL, NULL },
{ ". Sub 2-1", &top_el_mnu1_sel, NULL },
{ ". Sub 2-2", &top_el_mnu2_sel, NULL},
{ ". Sub 2-3", &top_el_mnu3_sel, NULL },
{ NULL, NULL, NULL },
};

/*=========================================================================*/
/* This is the main menu dialog box */

/* The first visible line and the total number of visible lines. */
/* Both values are written by M2_X2LMENU and read by M2_VSB */
uint8_t el_x2l_first = 0;
uint8_t el_x2l_cnt = 3;

/* M2_X2LMENU definition */
/* Option l4 = four visible lines */
/* Option e15 = first column has a width of 15 pixel */
/* Option W43 = second column has a width of 43/64 of the display width */
/* Option F3 = select font 3 for the extra column (icons) */

M2_X2LMENU(el_x2l_strlist, "l4e15W43F3", &el_x2l_first, &el_x2l_cnt, xmenu_data, 65,102,'\0');
M2_SPACE(el_x2l_space, "W1h1");
M2_VSB(el_x2l_vsb, "l4W2r1", &el_x2l_first, &el_x2l_cnt);
M2_LIST(list_x2l) = { &el_x2l_strlist, &el_x2l_space, &el_x2l_vsb };
M2_HLIST(el_x2l_hlist, NULL, list_x2l);
M2_ALIGN(top_el_x2l_menu, "-1|1W64H64", &el_x2l_hlist);


void vDisplay ( void *pvParameters )
{
	portTickType xLastWakeTime;
	u8g_t u8g;
	
	xLastWakeTime = xTaskGetTickCount ();
	
	/* Display requires a short wait before initialization */
	vTaskDelay ( dispINIT_DELAY / portTICK_RATE_MS );
	
	u8g_Init8BitFixedPort(&u8g, &u8g_dev_ssd1308_128x64_parallel, 0, 0, 0, 0, 0);
	u8g_SetFont (&u8g, u8g_font_5x8);
	
	m2_Init (&top_el_x2l_menu, m2_es_avr_u8g, m2_eh_6bs, m2_gh_u8g_bfs);
	m2_SetU8g(&u8g, m2_u8g_box_icon);
	m2_SetFont(0, (const void *)u8g_font_5x8);
	m2_SetFont(3, (const void *)u8g_font_m2icon_5);
	
	while (1)
	{
		vTaskDelayUntil ( &xLastWakeTime, dispTASK_FREQUENCY );
		
		u8g_FirstPage(&u8g);
		do
		{
			m2_Draw ();
		} while ( u8g_NextPage( &u8g ) );
	}
}
