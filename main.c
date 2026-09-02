#define F_CPU 4915200UL		/* Define frequency here its 8MHz */

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)	


void UART_Init( unsigned int ubrr)
{
    UBRR0H = (uns>>>>>>> ebcd8f178575c7adf84f2afc26bd021f91601408igned char)(ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    //Transmit enable
    // receiver enable
    UCSR0B = (1 << RXEN0)|(1 << TXEN0);

    // frame = 8 data, 2 stop
    
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void UART_Transmitter( unsigned char data)
{
    // Data register empty
    while ( !(UCSR0A &(1 << UDRE0)) );

    // UDR to transfer to transmit data buffer reg
    UDR0 = data; 
}

unsigned char UART_Receiver()
{
    while (( UCSR0A &(1 << RXC0)) == 0); // wait for data
    return (UDR0);
}

void UART_Send (char *str)
{
    unsigned char j = 0;

    while (str[j] != 0)
    {
        UART_Transmitter(str[j]);
        j++;
    }
}

void main (void)
{
    char c;
    
    UART_Init (BAUD_PRESCALE);

    
    while (1)
    {
        UART_Send("\n\t Echo Test ");
        //c = UART_Receiver();
        //UART_Transmitter(c); 
        _delay_ms(100);
    }

}
