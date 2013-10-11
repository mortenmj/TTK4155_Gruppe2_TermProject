/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

int main (void)
{
	board_init();
	sysclk_init();
/*
	
	ioport_init();

	static usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	usart_serial_init(USART_SERIAL, &usart_options);
*/
	gfx_mono_init();
	gfx_mono_draw_line(10, 10, 20, 20, GFX_PIXEL_SET);

	// Insert application code here, after the board has been initialized.
	
	while (1)
	{
		//usart_serial_putchar(USART_SERIAL, 'c');
	}
}
