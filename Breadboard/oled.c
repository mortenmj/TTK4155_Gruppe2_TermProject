/*
 * oled.c
 *
 * Created: 15.09.2013 13:45:31
 *  Author: mortenmj
 */ 

#include "config.h"

#include <stdint.h>
#include <avr/pgmspace.h>

#include "oled.h"
#include "font_8x8.h"

volatile uint8_t *oled_cmd = (uint8_t *) 0x1000;
volatile uint8_t *oled_data = (uint8_t *) 0x1200;

void oled_init (void)
{
	oled_set_display_off ();
	oled_send_command (0xa1); //segment remap
	oled_send_command (0xda); //common pads hardware: alternative
	oled_send_command (0x12);
	
	oled_send_command (0xc8); //common output scan direction:com63~com0
	
	oled_set_mux_ratio (0x3F);	
	oled_set_display_clock (0x80);	
	oled_set_contrast (0x50);
	oled_set_pre_charge_period (0x21);
	oled_set_memory_addressing_mode (PAGE_ADDRESSING_MODE);
	oled_set_vcom_deselect_level (VCOMH_DESELECT_0_83);
	oled_set_iref (IREF_INTERNAL);
	
	oled_send_command (OLED_DISPLAY_RAM);
	oled_send_command (OLED_DISPLAY_NORMAL);
	oled_clear_display ();
	oled_set_display_on ();
}

void oled_send_command (uint8_t cmd)
{
	
	*oled_cmd = cmd;
}

void oled_send_commands (uint8_t len, uint8_t *cmds)
{
	for (int i = 0; i < len; i++)
	{
		*oled_cmd = cmds[i];
	}
}

void oled_send_data (uint8_t data)
{
	*oled_data = data;
}

void oled_set_column (uint8_t col)
{
	oled_send_command (0x00 + (col & 0x0F));
	oled_send_command (0x10 + (col >> 4));
}

void oled_set_column_address (uint8_t start, uint8_t end)
{
uint8_t cmds[3] = {OLED_COLUMN_ADDRESS, start, end};
oled_send_commands (3, cmds);
}

void oled_set_contrast (uint8_t contrast)
{
	uint8_t cmds[2] = { OLED_CONTRAST, contrast };
	oled_send_commands (2, cmds);
}

void oled_set_display_clock (uint8_t clock)
{
	uint8_t cmds[2] = { OLED_DISPLAY_CLOCK, clock };
	oled_send_commands (2, cmds);
}

void oled_set_display_on (void)
{
	oled_send_command (OLED_POWER_ON);
}

void oled_set_display_off (void)
{
	oled_send_command (OLED_POWER_OFF);
}

void oled_set_iref (uint8_t ref)
{
	uint8_t cmds[2] = { OLED_SELECT_IREF, ref };
	oled_send_commands (2, cmds);
}

void oled_set_line (uint8_t line)
{
	oled_send_command (OLED_PAGE_START_ADDRESS + line);
	oled_set_column (0);
}

void oled_set_memory_addressing_mode (uint8_t mode)
{
	uint8_t cmds[2] = { OLED_MEMORY_ADDRESSING_MODE, mode };
	oled_send_commands (2, cmds);
}

void oled_set_mux_ratio (uint8_t ratio)
{	
	uint8_t cmds[2] = { OLED_MUX_RATIO, ratio };
	oled_send_commands (2, cmds);
}	

void oled_set_page_address (uint8_t start, uint8_t end)
{
	uint8_t cmds[3] = {OLED_PAGE_ADDRESS, start, end};
	oled_send_commands (3, cmds);
}

void oled_set_position (uint8_t line, uint8_t col)
{
	oled_set_line (line);
	oled_set_column (col * FONT_WIDTH);
}

void oled_set_pre_charge_period (uint8_t period)
{
	uint8_t cmds[2] = { OLED_PRE_CHARGE_PERIOD, period };
	oled_send_commands (2, cmds);
}

void oled_set_vcom_deselect_level (uint8_t level)
{
	uint8_t cmds[2] = { OLED_VCOMH_DESELECT_LEVEL, level };
	oled_send_commands (2, cmds);
}

void oled_clear_line (uint8_t line)
{
	oled_set_line (line);

	for (uint8_t col = 0; col < COLUMNS; col++)
	{
		oled_send_data (0);
	}
}

void oled_clear_display (void)
{
	for (uint8_t line = 0; line < PAGES; line++)
	{
		oled_clear_line (line);
	}
}

void oled_putchar (char c, uint8_t inv)
{
	for (uint8_t i = 0; i < FONT_WIDTH; i++)
	{
		inv ? oled_send_data (~pgm_read_byte (&font[c - ' '][i])) : oled_send_data (pgm_read_byte (&font[c - ' '][i]));
	}
}

void oled_print (char *c, uint8_t inv)
{
	while (*c != '\0')
	{
		oled_putchar(*c++, inv);
	}		
}