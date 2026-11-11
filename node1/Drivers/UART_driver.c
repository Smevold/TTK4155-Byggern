#include "UART_driver.h"


void UART_Init( unsigned int ubrr)
{
    // Set baud rate for USART
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0)|(1 << TXEN0);

    // Enable writing to UCSR0C, select 2 stop bits, select 8 bits for character size / data size
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    fdevopen(UART_Transmitter, UART_Receiver);
}

// Transmit data over UART
void UART_Transmitter( unsigned char data)
{
    // Data register empty
    while ( !(UCSR0A &(1 << UDRE0)) );

    // UDR to transfer to transmit data buffer reg
    UDR0 = data; 
}

// Receive data over UART
unsigned char UART_Receiver()
{
    while (( UCSR0A &(1 << RXC0)) == 0); // wait for data
    return (UDR0);
}

// Transmit a list of data
void UART_Send (char *str)
{
    // Initalize index to iterate over list of input data in str
    unsigned char j = 0;

    // Iterate over str and transmit untill it reaches the end
    while (str[j] != 0)
    {
        UART_Transmitter(str[j]);
        j++;
    }
}