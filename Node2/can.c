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

#include "config.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <util/delay.h>

#include "MCP2515.h"
#include "MCP2515define.h"
#include "can.h"

void can_init ( void )
{
	mcp2515_spi_port_init ();
		
	mcp2515_set_baudrate(250000, 1);
	
	/* Enable pin change interrupt on PB4 */
/*
	DDRB |= PCINT4;
	PCMSK0 |= (1 << PCINT4);
	PCICR |= (1 << PCIE0);
*/
	/* Enable interrupts on MCP2515 */
//	mcp2515_write ( MCP2515_CANINTE, 0xFF );

	/* Set loopback mode */
	mcp2515_bit_modify ( MCP2515_CANCTRL, MCP2515_MODE_MASK, MCP2515_MODE_LOOPBACK );
	
	printf ("CAN...OK\n");
}


void can_read ( unsigned char addr, unsigned char *val )
{
	mcp2515_read ( (uint8_t) addr, (uint8_t *) val );
}

uint8_t can_receive ( can_frame_t *frame )
{
	printf("receive\n");
	uint8_t status;
	

	while(1)
	{
		mcp2515_read_rxtx_status (&status);
		if (status & 3)
		{
			break;
		}
		printf("waiting\n");
		_delay_ms(100);
	}

	uint8_t buffer = 0;
	if (status & (1 << 0))
	{
		buffer = MCP2515_RX_BUF_0;
	}
	else if (status & (1 << 1))
	{
		buffer = MCP2515_RX_BUF_1;
	}
	else
	{
		return 0;
	}

	mcp2515_read_rx_buf (buffer, (mcp2515_can_frame_t *) frame);

	// Clear interrupt
	if(buffer == MCP2515_RXB0CTRL) {
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_RX0IF, 0);
		return 1;
	}
	else if(buffer == MCP2515_RXB1CTRL) {
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_RX1IF, 0);
		return 1;
	}
	
	return 0;
}

uint8_t can_transmit ( can_frame_t *frame )
{
	uint8_t status;
	mcp2515_read_rxtx_status (&status);
	
	uint8_t buffer = 0;
	
	if ((status & (1 << 2)) == 0)
	{
		buffer = MCP2515_TX_BUF_0;
	}
	else if ((status & (1 << 4)) == 0)
	{
		buffer = MCP2515_TX_BUF_1;
	}
	else if((status & (1 << 6)) == 0)
	{
		buffer = MCP2515_TX_BUF_2;
	}
	else
	{
		return 0;
	}

	mcp2515_load_tx_buf(buffer, (mcp2515_can_frame_t *) frame);
	mcp2515_rts (buffer);
	
	return 1;
}

void can_write ( unsigned char addr, unsigned char val )
{
	mcp2515_write ( (uint8_t) addr, (uint8_t) val );
}