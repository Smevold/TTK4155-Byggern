#pragma once

void UART_Init( unsigned int ubrr);

void UART_Transmitter( unsigned char data);

unsigned char UART_Receiver();

void UART_Send (char *str);