#include<avr/io.h>
#include<util/delay.h>


#define F_CPU 8000000UL		/* Define frequency here its 8MHz */
#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)	

void main (void)
{

    UART_Init (BAUD_PRESCALE);
}

void UART_Init( unsigned int ubrr)
{
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char) ubrr;
    UCSRB = (1<<RXEN)|(1<<TXEN);
    
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}