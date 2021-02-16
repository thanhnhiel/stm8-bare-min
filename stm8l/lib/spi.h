#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

/*
 * Initialize SPI in MODE1.
 *
 * Pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> user defined
 */
void SPI1_init();

void SPI1_write(uint8_t data);

uint8_t SPI1_read();

#endif /* SPI1_H */

