#pragma once

#include <stdint.h>
#include <avr/io.h>

// Slave select for different units
#define SS_IO_AVR 0
#define SS_OLED 1
#define SS_CAN 2

// Init SPI 
void SPI_Init();

// Transmit 1 byte
char SPI_Transmit(char data);

// reads SPI line until finished
char SPI_Receive();

// uses SPI_transmit to transmit n bytes
void SPI_Transmit_n_Bytes(char* data, uint8_t n);

// Write "char* data[n];" to create input for this function
void SPI_Receive_n_Bytes(char* data, uint8_t n);
// select what slave to communicate with
void SPI_SlaveSelect(uint8_t slave);
// after SPI communication is done deselection is needed
void SPI_SlaveDeselectAll();