#pragma once

#include<avr/io.h>
#include <stdio.h>

void UART_Init( unsigned int ubrr);

void UART_Transmitter( unsigned char data);

unsigned char UART_Receiver();

void UART_Send (char *str);