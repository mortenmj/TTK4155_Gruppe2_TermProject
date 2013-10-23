/*
 * mcp2515.c
 *
 * Created: 16.10.2013 10:46:12
 *  Author: mortenmj
 */ 

#include <avr/interrupt.h>

#include "mcp2515.h"
#include "spi.h"

/* MCP port */
#define mcpDDR							DDRB
#define mcpPORT							PORTB

/* MCP pins */
#define mcpSS							( ( unsigned char ) ( 1 << PB0 ) )

#define MCP_SELECT() (mcpPORT &= ~(1 << MCP_SS_PIN))
#define MCP_DESELECT() (mcpPORT |= (1 << MCP_SS_PIN))

void mcp2515_init (uint16_t addr)
{
	spi_init ();
		
	// Configure device
	mcp2515_reset ();	
	
	// Enable interrupts
	mcp2515_bit_modify (MCP_REG_CANINTE, 0x03, 0x03);
	
	// Write address
	mcp2515_write (MCP_REG_RXF0SIDL, (((uint8_t)(addr & 0x07)) << 5));
	mcp2515_write (MCP_REG_RXF0SIDH, (uint8_t)((addr >> 3) & 0xFF));
}

void mcp2515_reset (void)
{
	MCP_SELECT();
	spi_write (MCP_CMD_RESET);
	MCP_DESELECT();
}

uint8_t mcp2515_read (uint8_t addr)
{
	MCP_SELECT();
	spi_write (MCP_CMD_READ);
	spi_write (addr);
	uint8_t val = spi_read ();
	MCP_DESELECT();
	
	return val;	
}

void mcp2515_write (uint8_t addr, uint8_t val)
{
	MCP_SELECT();
	spi_write (MCP_CMD_WRITE);
	spi_write (addr);
	spi_write (val);
	MCP_DESELECT();
}

void mcp2515_bit_modify (uint8_t addr, uint8_t mask, uint8_t data) 
{
	MCP_SELECT();
	// Read command
	spi_write (MCP_CMD_BIT_MODIFY);
	spi_write (addr);
	spi_write (mask);
	spi_write (data);
	MCP_DESELECT();
}

uint8_t mcp2515_read_status (void)
{
	MCP_SELECT();
	spi_write (MCP_CMD_READ_STATUS);
	uint8_t val = spi_read ();
	MCP_DESELECT();
	
	return val;
}

void mcp2515_rts (uint8_t buffer)
{
	MCP_SELECT();
	spi_write (MCP_CMD_RTS | (1<<buffer));
	MCP_DESELECT();
}