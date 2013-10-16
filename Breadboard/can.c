/*
 * can.c
 *
 * Created: 16.10.2013 10:50:26
 *  Author: mortenmj
 */ 

#include "can.h"
#include "mcp2515.h"

void can_init(uint16_t listenAddress) {
	mcp2515_init(listenAddress);
	
	// Set mode of operation to LOOPBACK
	mcp2515_bit_modify(MCP_REG_CANCTRL, 0b11100000, 0b01000000);
	
	// Set mode of operation to NORMAL
	//mcp2515_bit_modify(MCP_REG_CANCTRL, 0b11100000, 0b00000000);
}

uint8_t can_send_frame(can_frame_t* frame) {
	// Figure out which buffer we are working on
	uint8_t status = mcp2515_read_status();
	
	uint8_t buffer = 0;
	if((status&(1<<MCP_STATUS_TXREQ0)) == 0) {
		buffer = MCP_REG_CTXB0CTRL;
	} else if((status&(1<<MCP_STATUS_TXREQ1)) == 0) {
		buffer = MCP_REG_CTXB1CTRL;
	} else if((status&(1<<MCP_STATUS_TXREQ2)) == 0) {
		buffer = MCP_REG_CTXB2CTRL;
	}
	else {
		return 0;
	}
	
	// Fill buffer with data.
	for(uint8_t i = 0; i < frame->size; i++) {
		mcp2515_write(buffer+0x6+i, frame->data[i]);
	}
	// Set length of data
	mcp2515_write(buffer+0x05, frame->size & 0xF);
	
	// Set low bits of identifier
	mcp2515_write(buffer+0x02, (((uint8_t)(frame->identifier&0x7))<<5));
	// Set high bits of identifier
	mcp2515_write(buffer+0x01, (uint8_t)((frame->identifier>>3) & 0xFF));
	// Request to tranfer frame
	mcp2515_write(buffer, (1<<3));
	//mcp2515_request_to_send(0);
	
	return 1;
}

uint8_t can_recieve_frame(can_frame_t* frame) {
	while(1) {
		uint8_t status = mcp2515_read_status();
		if(status & ((1<<MCP_STATUS_RX0IF) | (1<<MCP_STATUS_RX1IF))) 
			break;
	}
	
	uint8_t buffer = 0;
	uint8_t status = mcp2515_read_status();
	if((status & (1<<MCP_STATUS_RX0IF))){
		buffer = MCP_REG_RXB0CTRL;
		//printf("Reading from 0\r");
	}
	else if ((status & (1<<MCP_STATUS_RX1IF))){
		buffer = MCP_REG_RXB1CTRL;
		//printf("Reading from 1\r");
	}
	else {
		// WTF ?!?!
		return 0;
	}
	
	frame->size = mcp2515_read(buffer+0x5) & 0xF;
	for(uint8_t i = 0; i < frame->size; i++) {
		frame->data[i] = mcp2515_read(buffer+0x6+i);
	}
	
	// Clear interrupt
	if(buffer == MCP_REG_RXB0CTRL) {
		mcp2515_bit_modify(MCP_REG_CANINTF, (1<<0), 0x00);
		//printf("Clearing from 0\r");
		return 1;
	}
	else if(buffer == MCP_REG_RXB1CTRL) {
		mcp2515_bit_modify(MCP_REG_CANINTF, (1<<1), 0x00);
		//printf("Clearing from 1\r");
		return 1;
	}
	return 0;
}