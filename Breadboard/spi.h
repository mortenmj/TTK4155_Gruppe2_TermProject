/*
 * spi.h
 *
 * Created: 30.09.2013 20:08:38
 *  Author: mortenmj
 */ 

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_init (void);
void spi_write (uint8_t data);
uint8_t spi_read (void);

#endif /* SPI_H_ */