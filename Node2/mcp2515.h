/*
 * mcp2515.h
 *
 * Created: 16.10.2013 10:47:51
 *  Author: mortenmj
 */ 


#ifndef MCP2515_H_
#define MCP2515_H_

#include <avr/io.h>

#define MCP_SS_PIN 4
#define MCP_SS_DDR DDRB
#define MCP_SS_PORT PORTB

#define MCP_CMD_RESET			0xC0
#define MCP_CMD_READ			0x03
#define MCP_CMD_WRITE			0x02
#define MCP_CMD_READ_STATUS		0xA0
#define MCP_CMD_RTS				0x80
#define MCP_CMD_BIT_MODIFY		0x05

#define MCP_REG_RXF0SIDH 0x00
#define MCP_REG_RXF0SIDL 0x01
#define MCP_REG_RXM0SIDH 0x20
#define MCP_REG_RXM0SIDL 0x21

#define MCP_REG_CANINTE 0x2b
#define MCP_REG_CANINTF 0x2c
#define MCP_REG_CANCTRL 0x0f
#define MCP_REG_CTXB0CTRL 0x30
#define MCP_REG_CTXB1CTRL 0x40
#define MCP_REG_CTXB2CTRL 0x50
#define MCP_REG_RXB0CTRL 0x60
#define MCP_REG_RXB1CTRL 0x70

#define MCP_STATUS_RX0IF 0
#define MCP_STATUS_RX1IF 1
#define MCP_STATUS_TXREQ0 2
#define MCP_STATUS_TX0IF 3
#define MCP_STATUS_TXREQ1 4
#define MCP_STATUS_TX1IF 5
#define MCP_STATUS_TXREQ2 6
#define MCP_STATUS_TX2IF 7

void mcp2515_init (uint16_t listenAddress);
void mcp2515_reset (void);
uint8_t mcp2515_read (uint8_t adr);
void mcp2515_write (uint8_t adr, uint8_t val);
void mcp2515_rts (uint8_t buffer);
uint8_t mcp2515_read_status (void);
void mcp2515_bit_modify (uint8_t addr, uint8_t mask, uint8_t data);

#endif /* MCP2515_H_ */