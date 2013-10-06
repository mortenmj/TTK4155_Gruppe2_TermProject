/*
 *
 * \file
 *
 * \brief Empty user application template
 *
 *
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */

#include <asf.h>
#include <util/delay.h>
#include "nRF24L01/radio.h"

/*
radiopacket_t packet;
uint8_t remote_addr[5] = {0x15, 0x15, 0x15, 0x15, 0x15};
*/

int main (void)
{
	sysclk_init ();
	board_init ();
	ioport_init();
	
	static usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT,
	};

	usart_serial_init (USART_SERIAL, &usart_options);
		
	/* LED */
	ioport_set_pin_dir (LED0, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_set_pin_dir (LED1, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_set_pin_dir (LED2, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	/* Radio */
/*
	Radio_Init ();
	packet.type = MESSAGE;
	packet.payload.message.address[0] = remote_addr[0];
	packet.payload.message.address[1] = remote_addr[1];
	packet.payload.message.address[2] = remote_addr[2];
	packet.payload.message.address[3] = remote_addr[3];
	packet.payload.message.address[4] = remote_addr[4];
*/

	/* Enable interrupts */
	sei();

	while(1)
	{/*
		Radio_Set_Tx_Addr(remote_addr);
		Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);*/
		usart_serial_putchar (USART_SERIAL, 'c');
		_delay_ms(100);
	}
}