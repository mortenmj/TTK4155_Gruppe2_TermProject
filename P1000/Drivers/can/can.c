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
#include <string.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "MCP2515.h"
#include "MCP2515define.h"
#include "can.h"

static xQueueHandle rx_queue;
static xQueueHandle tx_queue;

static can_frame_t *can_frames;
static can_frame_t *first_available;

can_frame_t *can_frame_acquire (void)
{	
	can_frame_t *frame = first_available;
	first_available = frame->next;
	
	return frame;
}

void can_frame_release (can_frame_t *frame)
{
	frame->next = first_available;
	first_available = frame;
}

void can_init (void)
{
	portENTER_CRITICAL();
	{
		unsigned char canstat = 0;

		rx_queue = xQueueCreate (CAN_RX_QUEUE_LEN, (unsigned portBASE_TYPE) sizeof (can_frame_t*));
		tx_queue = xQueueCreate (CAN_TX_QUEUE_LEN, (unsigned portBASE_TYPE) sizeof (can_frame_t*));
		
		/* Initialize frame pool */
		can_frames = pvPortMalloc ((size_t) ((CAN_RX_QUEUE_LEN + CAN_TX_QUEUE_LEN) * sizeof (can_frame_t)));
		first_available = &can_frames[0];
		for (uint8_t i = 0; i < CAN_RX_QUEUE_LEN + CAN_TX_QUEUE_LEN - 1; i++)
		{
			can_frames[i].next = &can_frames[i+1];
		}
		can_frames[CAN_RX_QUEUE_LEN + CAN_TX_QUEUE_LEN - 1].next = NULL;
		
		mcp2515_spi_port_init ();
		mcp2515_reset ();

		do
		{
			mcp2515_read (MCP2515_CANSTAT, &canstat);
		} while ((canstat & 0xE0) != MCP2515_MODE_CONFIG);

		DDRB |= PCINT4;
		PCMSK0 |= (1 << PCINT4);
		PCICR |= (1 << PCIE0);

		/* Enable interrupts on MCP2515 */
		mcp2515_write (MCP2515_CANINTE, /*MCP2515_CANINTF_ERRIF |*/ MCP2515_CANINTF_WAKIF | MCP2515_CANINTF_TX0IF | MCP2515_CANINTF_TX1IF | MCP2515_CANINTF_TX2IF | MCP2515_CANINTF_RX0IF | MCP2515_CANINTF_RX1IF);

		/* Set one-shot mode */
		mcp2515_bit_modify (MCP2515_CANCTRL, MCP2515_MODE_ONESHOT, MCP2515_MODE_ONESHOT);

		/* Set normal mode */
		mcp2515_bit_modify (MCP2515_CANCTRL, MCP2515_MODE_MASK, MCP2515_MODE_NORMAL);
		do
		{
			mcp2515_read (MCP2515_CANSTAT, &canstat);
		} while ((canstat & 0xE0) != MCP2515_MODE_NORMAL);
	}
	portEXIT_CRITICAL();
}

uint8_t can_receive (can_frame_t *frame, portTickType block_time)
{
	return xQueueReceive (rx_queue, frame, block_time);
}

uint8_t can_transmit (can_frame_t *frame, portTickType block_time)
{
	uint8_t status;
	mcp2515_read_rxtx_status (&status);

	uint8_t buffer = 0;

	/* Look for an available transmit buffer */
	if ((status & (1 << 2)) == MCP2515_TX_BUF_0)
	{
		buffer = MCP2515_TX_BUF_0;
	}
	else if ((status & (1 << 4)) == MCP2515_TX_BUF_1)
	{
		buffer = MCP2515_TX_BUF_1;
	}
	else if((status & (1 << 6)) == MCP2515_TX_BUF_2)
	{
		buffer = MCP2515_TX_BUF_2;
	}
	else
	{
		/* No transmit buffer available, queue packet and it will be sent by the ISR */
		return xQueueSend (tx_queue, frame, block_time);
	}

	mcp2515_load_tx_buf (buffer, &frame->packet);
	mcp2515_rts (buffer);
	can_frame_release (frame);
	
	return pdPASS;
}

ISR (PCINT0_vect)
{
	signed portBASE_TYPE xHigherPriorityTaskWoken;
	uint8_t status;
	mcp2515_read (MCP2515_CANINTF, &status);
	
	/* Handle error interrupt */
	if (status & MCP2515_CANINTF_ERRIF)
	{
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_ERRIF, 0);
	}

	/* Handle wake-up interrupt */
	if (status & MCP2515_CANINTF_WAKIF)
	{
		unsigned char canstat = 0;
		
		/* Set normal mode */
		mcp2515_bit_modify (MCP2515_CANCTRL, MCP2515_MODE_MASK, MCP2515_MODE_NORMAL);
		do
		{
			mcp2515_read (MCP2515_CANSTAT, &canstat);
		} while ((canstat & 0xE0) != MCP2515_MODE_NORMAL);
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_WAKIF, 0);
	}

	/* Handle TX0 interrupt */
	if (status & MCP2515_CANINTF_TX0IF)
	{
		can_frame_t *frame = NULL;
		
		if (xQueueReceiveFromISR (tx_queue, frame, &xHigherPriorityTaskWoken) == pdTRUE)
		{
			/* Send frame */
			mcp2515_load_tx_buf (MCP2515_TX_BUF_0, &frame->packet);
			mcp2515_rts (MCP2515_TX_BUF_0);
			can_frame_release (frame);
		}
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_TX0IF, 0);
	}

	/* Handle TX1 interrupt */
	if (status & MCP2515_CANINTF_TX1IF)
	{
		can_frame_t *frame = NULL;
		
		if (xQueueReceiveFromISR (tx_queue, frame, &xHigherPriorityTaskWoken) == pdTRUE)
		{
			/* Send frame */
			mcp2515_load_tx_buf (MCP2515_TX_BUF_1, &frame->packet);
			mcp2515_rts (MCP2515_TX_BUF_1);
			can_frame_release (frame);
		}
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_TX1IF, 0);
	}

	/* Handle TX2 interrupt */
	if (status & MCP2515_CANINTF_TX2IF)
	{
		can_frame_t *frame = NULL;
		
		if (xQueueReceiveFromISR (tx_queue, frame, &xHigherPriorityTaskWoken) == pdTRUE)
		{
			/* Send frame */
			mcp2515_load_tx_buf (MCP2515_TX_BUF_2, &frame->packet);
			mcp2515_rts (MCP2515_TX_BUF_2);
			can_frame_release (frame);
		}
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_TX2IF, 0);
	}

	/* Handle RX0 interrupt */
	if (status & MCP2515_CANINTF_RX0IF)
	{
		can_frame_t *frame = can_frame_acquire ();
		
		mcp2515_read_rx_buf (MCP2515_RX_BUF_0, &frame->packet);
		xQueueSendFromISR (rx_queue, frame, &xHigherPriorityTaskWoken);
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_RX0IF, 0);
	}

	/* Handle RX1 interrupt */
	if (status & MCP2515_CANINTF_RX1IF)
	{
		can_frame_t *frame = can_frame_acquire ();
		
		mcp2515_read_rx_buf (MCP2515_RX_BUF_1, &frame->packet);
		xQueueSendFromISR (rx_queue, frame, &xHigherPriorityTaskWoken);
		
		/* Clear interrupt */
		mcp2515_bit_modify (MCP2515_CANINTF, MCP2515_CANINTF_RX1IF, 0);
	}
	
	if (xHigherPriorityTaskWoken)
	{
		taskYIELD ();
	}
}