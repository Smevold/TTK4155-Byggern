#pragma once

#include <stdint.h>

void SPI_init();

void SPI_Transmit(char cData);

char SPI_Receive();