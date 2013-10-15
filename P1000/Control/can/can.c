/**
 * @file
 *
 * @ingroup CAN
 *
 * @brief CAN API implementation
 */

/*
 * can.c
 *
 * Created: 01.10.2013 20:11:09
 *  Author: mortenmj
 */

#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "MCP2515.h"
#include "MCP2515define.h"
#include "can.h"

can_t can;

void can_init ( void )
{
	mcp2515_spi_port_init ();
		
	mcp2515_set_baudrate(250000, 1);
	
	/* Enable pin change interrupt on PB4 */
	DDRB |= PCINT4;
	PCMSK0 |= (1 << PCINT4);
	PCICR |= (1 << PCIE0);

	/* Enable interrupts on MCP2515 */
	mcp2515_write ( MCP2515_CANINTE, 0xFF );

	/* Set loopback mode */
	mcp2515_bit_modify ( MCP2515_CANCTRL, MCP2515_MODE_MASK, MCP2515_MODE_LOOPBACK );
}


void can_read ( unsigned char addr, unsigned char *c )
{
	mcp2515_read ( (uint8_t) addr, (uint8_t *) c );
}

void can_receive ( can_frame_t *in_frame )
{
	mcp2515_read_rx_buf(MCP2515_RX_BUF_0, (mcp2515_can_frame_t *) in_frame);
}

void can_transmit ( can_frame_t *out_frame )
{
	mcp2515_load_tx_buf(MCP2515_TX_BUF_0, (mcp2515_can_frame_t *) out_frame);
	mcp2515_rts (MCP2515_TX_BUF_0);
}

void can_write ( unsigned char addr, unsigned char c )
{
	mcp2515_write ( (uint8_t) addr, (uint8_t) c );
}