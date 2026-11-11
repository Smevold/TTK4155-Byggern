#pragma once

#include<avr/io.h>
#include <stdio.h>

// Initalize UART
void UART_Init( unsigned int ubrr);

// Transmit data over UART
void UART_Transmitter( unsigned char data);

// Receive data over UART
unsigned char UART_Receiver();

// Transmit a list of data
void UART_Send (char *str);