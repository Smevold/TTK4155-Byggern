#pragma once

#include <stdint.h>

#define SS_IO_AVR 0
#define SS_OLED 1
#define SS_EMPTY 2

void SPI_Init();

void SPI_Transmit(char cData);

char SPI_Receive();

void SPI_Transmit_n_Bytes(char* data, uint8_t n);

// Write "char* data[n];" to create input for this function
void SPI_Receive_n_Bytes(char* data, uint8_t n);

void SPI_SlaveSelect(uint8_t slave);