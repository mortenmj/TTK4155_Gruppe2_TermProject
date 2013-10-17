/*
 * spi.c
 *
 * Created: 30.09.2013 20:08:29
 *  Author: mortenmj
 */ 

#include "spi.h"

#include <stdint.h>
#include <avr/io.h>

/* SPI port */
#define spiDDR							DDRB
#define spiPORT							PORTB

/* Constants for writing to SPCR. */
#define spiENABLE						( ( unsigned char ) ( 1 << SPE ) )
#define spiMASTER_MODE					( ( unsigned char ) ( 1 << MSTR ) )
#define spiPRESCALE						( ( unsigned char ) ( 1 << SPR1 | 1 << SPR0 ) )

/* SPI pins */
#define spiSS							( ( unsigned char ) ( 1 << PB4 ) )
#define spiMOSI							( ( unsigned char ) ( 1 << PB5 ) )
#define spiMISO							( ( unsigned char ) ( 1 << PB6 ) )
#define spiSLCK							( ( unsigned char ) ( 1 << PB7 ) )
											
/*-----------------------------------------------------------*/

void spi_init (void)
{
	/* SS, SLCK & MOSI as output */

	spiDDR |= ( spiSS | spiMOSI | spiSLCK);
	
	SPCR |= ( spiENABLE | spiMASTER_MODE | spiPRESCALE );
}

uint8_t spi_read(void)
{
	SPDR = 0x00;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void spi_write( uint8_t data )
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}