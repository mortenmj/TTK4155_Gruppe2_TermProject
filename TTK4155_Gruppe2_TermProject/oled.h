/*
 * oled.h
 *
 * Created: 15.09.2013 13:45:42
 *  Author: mortenmj
 *  Se https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/SSD1308
 */ 

#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

#define ROWS		64
#define COLUMNS		128
#define PAGES		8
#define PAGE_WIDTH	(ROWS/8)
#define FONT_WIDTH	8
#define CHARS		(COLUMNS / FONT_WIDTH)
#define MAX_PAGE	(PAGES - 1)
#define MAX_COL		(COLUMNS - 1)

#define OLED_RESET					0x08

#define HORIZONTAL_ADDRESSING_MODE	0x00
#define VERTICAL_ADDRESSING_MODE	0x01
#define PAGE_ADDRESSING_MODE		0x02
#define OLED_MEMORY_ADDRESSING_MODE	0x20

#define OLED_COLUMN_ADDRESS			0x21
#define OLED_PAGE_ADDRESS			0x22
#define OLED_CONTRAST				0x81
#define OLED_SEGMENT_REMAP_START	0xA0
#define OLED_SEGMENT_REMAP_END		0xA1
#define OLED_DISPLAY_RAM			0xA4
#define OLED_DISPLAY_ON				0xA5
#define OLED_DISPLAY_NORMAL			0xA6
#define OLED_DISPLAY_INVERSE		0xA7
#define OLED_MUX_RATIO				0xA8

#define IREF_INTERNAL				0x00
#define IREF_EXTERNAL				0x10
#define OLED_SELECT_IREF			0xAD

#define OLED_POWER_OFF				0xAE
#define OLED_POWER_ON				0xAF
#define OLED_CMD_MODE				0x80
#define OLED_DATA_MODE				0x40

#define VCOMH_DESELECT_0_65			0x00
#define VCOMH_DESELECT_0_77			0x20
#define VCOMH_DESELECT_0_83			0x30
#define OLED_VCOMH_DESELECT_LEVEL	0xDB

#define PAGE0 0x00
#define PAGE1 0x01
#define PAGE2 0x02
#define PAGEE 0x03
#define PAGE4 0x04
#define PAGE5 0x05
#define PAGE6 0x06
#define PAGE7 0x07
#define OLED_PAGE_START_ADDRESS 0xB0

#define OLED_DISPLAY_OFFSET		0xD3
#define OLED_DISPLAY_CLOCK		0xD5
#define OLED_PRE_CHARGE_PERIOD	0xD9
#define OLED_COM_PINS			0xDA

void oled_init (void);
void oled_send_command (uint8_t cmd);
void oled_send_commands (uint8_t len, uint8_t *cmds);
void oled_send_data (uint8_t data);

void oled_set_column (uint8_t col);
void oled_set_column_address (uint8_t start, uint8_t end);
void oled_set_contrast (uint8_t contrast);
void oled_set_display_clock (uint8_t clock);
void oled_set_display_on (void);
void oled_set_display_off (void);
void oled_set_iref (uint8_t ref);
void oled_set_memory_addressing_mode (uint8_t mode);
void oled_set_mux_ratio (uint8_t ratio);
void oled_set_page_address (uint8_t start, uint8_t end);
void oled_set_pre_charge_period (uint8_t period);
void oled_set_vcom_deselect_level (uint8_t level);

void oled_clear_display (void);
void oled_fill_display (void);
void oled_clear_line (uint8_t line);
void oled_set_line (uint8_t line);
void oled_putchar (char c);
void oled_print (char *c);

#endif /* OLED_H_ */