/**
 * @file
 *
 * @ingroup SPI
 *
 * @brief SPI API
 */

/*
 * spi.h
 *
 * Created: 30.09.2013 20:08:38
 *  Author: mortenmj
 */ 


#ifndef SPI_H_
#define SPI_H_

/**
 * @defgroup SPI SPI API
 *
 * @ingroup Devices
 *
 * @brief Provides functions for communicating over the SPI interface
 *
 * @{
 */

/**
 * @brief Initializes SPI interface
 */
void spi_init (void);

/**
 * @brief Writes a byte on the SPI interface
 *
 * @param data_out The data to write
 *
 */
uint8_t spi_transfer (uint8_t data_out);

/**
 * @brief Writes multiple bytes on the SPI interface
 *
 * @param[in] data_out The data to write
 * @param len Number of bytes to write
 *
 */
void spi_write_block (uint8_t *data_out, uint8_t len);

/**
 * @brief Reads and writes multiple bytes on the SPI interface
 *
 * @param[in] data_out The data to write
 * @param[out] data_in The received data
 * @param len Number of bytes to read/write
 *
 */
void spi_readwrite_block (uint8_t *data_out, uint8_t *data_in, uint8_t len);

/** @} */

#endif /* SPI_H_ */