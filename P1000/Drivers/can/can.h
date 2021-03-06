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
typedef struct CanFrame {
	/**
	 * @brief Message ID
	 */
	uint32_t id;

	/**
	 * @brief Message data, up to 8 bytes
	 */
	uint8_t data[8];

	/**
	 * @brief Length of data
	 */
	uint8_t dlc;
} can_frame_t ;

/**
 * @brief Initializes CAN device
 */
void can_init ( void );

/**
 * @brief Reads a register on the CAN controller
 *
 * @param addr The register address to read
 * @param[out] c Char to write register value to
 *
 */
void can_read ( unsigned char addr, unsigned char *val );

/**
 * @brief Receive a packet from the CAN bus
 *
 * @param[out] in_frame Packet to write to
 * @param block Block on receive or not
 *
 */
void can_receive ( can_frame_t *frame, uint8_t block );

/**
 * @brief Transmit a packet on the CAN bus
 *
 * @param[in] out_frame Packet to read from
 *
 */
void can_transmit ( can_frame_t *frame );

/**
 * @brief Writes a register on the CAN controller
 *
 * @param addr The register address to write
 * @param c Char to write to the controller register
 *
 */
void can_write ( unsigned char addr, unsigned char val );

/** @} */

#endif /* CAN_H_ */