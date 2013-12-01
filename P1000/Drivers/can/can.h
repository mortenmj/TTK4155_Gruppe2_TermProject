/**
 * @file
 *
 * @ingroup CAN
 *
 * @brief High-level CAN driver
 */

/*
 * can.h
 *
 * Created: 01.10.2013 19:26:47
 * Author: mortenmj
 */

#ifndef CAN_H_
#define CAN_H_

#include "MCP2515.h"

#define CAN_RX_QUEUE_LEN 10
#define CAN_TX_QUEUE_LEN 10

/**
 * @defgroup CAN CAN driver
 *
 * @ingroup Drivers
 *
 * @brief High-level CAN driver
 *
 * @{
 */

/**
 *  @brief CAN frame
 *
 * Struct that describes a frame to be sent over CAN-bus.
 */
typedef struct can_frame {
	/**
	* Rather than allocating a new frame when we need it we use an object pool to store frames not currently in use
	* Packets not in use form a singly linked list so that we can retrieve a packet from the pool in O(1) time. 
	* We use a union to implement the linked list in the same memory as we store data.
	*/
	union
	{
		mcp2515_can_frame_t packet;
		struct can_frame *next;
	};
} can_frame_t ;

/**
 * @brief Acquire a frame from the object pool
 *
 * @returns A frame pointer
 */
can_frame_t *can_frame_acquire (void);

/**
 * @brief Release a frame back to the object pool
 *
 * @param[in] frame Frame to release.
 */
void can_frame_release (can_frame_t *frame);

/**
 * @brief Initializes CAN device
 */
void can_init (void);

/**
 * @brief Receive a packet from the CAN bus
 *
 * @param[out] frame Packet to write to
 * @param block_time Time to block on read
 *
 * @returns 0 on success
 */
uint8_t can_receive (can_frame_t *frame, portTickType block_time);

/**
 * @brief Transmit a packet on the CAN bus
 *
 * @param[in] frame Packet to read from
 * @param block_time Time to block on read
 *
 * @returns 0 on success
 */
uint8_t can_transmit (can_frame_t *frame, portTickType block_time);

/** @} */

#endif /* CAN_H_ */