/*
 * can.h
 *
 * Created: 01.10.2013 19:26:47
 *  Author: mortenmj
 */ 


#ifndef CAN_H_
#define CAN_H_

/*
* CAN status
*/
typedef struct CanStatus {
	/* Interrupt register */
	uint8_t interrupt;
} can_status_t;

/*
*  CAN frame
*/
typedef struct CanFrame {
	/* Message ID */
	uint32_t id;
	
	/* Message data, up to 8 bytes */
	uint8_t data[8];
	
	/* Length of data */
	uint8_t dlc;
} can_frame_t ;

void can_init ( void );
void can_read ( unsigned char addr, unsigned char *c );
void can_receive ( can_frame_t *in_frame );
void can_transmit ( can_frame_t *out_frame );
void can_write ( unsigned char addr, unsigned char c );

#endif /* CAN_H_ */