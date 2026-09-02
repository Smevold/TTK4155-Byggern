#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define F_CPU 8000000UL		/* Define frequency here its 8MHz */
#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)	



void UART_Init( unsigned int ubrr)
{
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char) ubrr;
    //Transmit enable
    // receiver eable
    UCSRB = (1 << RXEN)|(1 << TXEN);

    // fram = 8 data, 2 stop
    
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}
void UART_Transicver ( unsigned char data)
{
    // Data register empty
    while ( !(UCSRA &(1 << UDRE)) )

    // UDR to transfer to transmit data buffer reg
    UDR = data; 
}

void UART_Receiver()
{
    while (( UCSRA &(1 << RXC)) == 0); // wait for data
    return (UDR);
}

void send (char *str)
{
    unsigned char j = 0;

    while (str[j] != 0)
    {
        UART_Transicver(str[j]);
        j++;
    }
}

void main (void)
{
    char c;
    
    UART_Init (BAUD_PRESCALE);

    UART_SendString("\n\t Echo Test ");
    while (1)
    {
        c = UART_Receiver();
        UART_Transicver(c); 
    }
}