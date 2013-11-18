/*
 * spi.h
 *
 * Created: 18.11.2013 17:28:46
 *  Author: mortenmj
 */ 


#ifndef SPI_H_
#define SPI_H_

void spi_init_pins (void);
void spi_init_module (struct spi_device *device);

#endif /* SPI_H_ */