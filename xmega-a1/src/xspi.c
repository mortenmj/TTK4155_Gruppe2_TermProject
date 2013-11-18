/*
 * nrf_spi.c
 *
 * Created: 18.11.2013 17:26:27
 *  Author: mortenmj
 */ 

#include <asf.h>
#include "xspi.h"

#define MOSI	PIN5_bm
#define MISO	PIN6_bm
#define SCK		PIN7_bm

void spi_init_pins (void)
{
	ioport_configure_port_pin(&PORTC, MOSI, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_port_pin(&PORTC, MISO, IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, SCK, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
}

void spi_init_module (struct spi_device *device)
{
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, device, SPI_MODE_0, 1000000, 0);
	spi_enable(&SPIC);
}