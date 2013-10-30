/*
 * spi.c
 *
 * Created: 30.09.2013 20:08:29
 *  Author: mortenmj
 */ 

#include "spi.h"

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

/* SPI port */
#define spiDDR							DDRB
#define spiPORT							PORTB

/* Constants for writing to SPCR. */
#define spiENABLE						( ( unsigned char ) ( 1 << SPE ) )
#define spiMASTER_MODE					( ( unsigned char ) ( 1 << MSTR ) )
#define spiPRESCALE						( ( unsigned char ) ( 1 << SPR1 | 1 << SPR0 ) )

/* SPI pins */
#define spiSS							( ( unsigned char ) ( 1 << PB0 ) )
#define spiMOSI							( ( unsigned char ) ( 1 << PB2 ) )
#define spiMISO							( ( unsigned char ) ( 1 << PB3 ) )
#define spiSLCK							( ( unsigned char ) ( 1 << PB1 ) )
											
/*-----------------------------------------------------------*/

void spi_init (void)
{
	/* SS, SLCK & MOSI as output */

	spiDDR |= ( spiSS | spiMOSI | spiSLCK);
	
	SPCR |= ( spiENABLE | spiMASTER_MODE | spiPRESCALE );
	
	printf ("SPI...OK\n");
}

uint8_t spi_transfer (uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void spi_write_block (uint8_t *data, uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		spi_transfer ( data[i] );
	}
}

void spi_readwrite_block (uint8_t *data, uint8_t *dest, uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		dest[i] = spi_transfer ( data[i] );
	}
}