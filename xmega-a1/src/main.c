/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <util/delay.h>
#include "nRF24L01.h"
#include "wl_module.h"

struct spi_device spi_device_conf = {
	.id = IOPORT_CREATE_PIN(PORTC, 1)
};

int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	
	wl_module_init(&spi_device_conf);
	wl_module_tx_config (wl_module_TX_NR_0);

	pmic_init();
	pmic_set_scheduling (PMIC_SCH_ROUND_ROBIN);
	
	cpu_irq_enable ();
		
	_delay_ms (50);
	
	uint8_t data_buffer[3] = {0xAA, 0x00, 0xAA};
	uint8_t payload[wl_module_PAYLOAD];
	
	payload[0] = 10;
	payload[1] = 10;
	payload[2] = 20;
	payload[3] = 30;
	payload[4] = 40;
	payload[5] = 50;

	while(1)
	{
		//wl_module_send (payload, wl_module_PAYLOAD);
		//_delay_ms (20);
		//wl_module_config_register(STATUS, (1<<TX_DS));
	}
}