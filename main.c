#define F_CPU 4915200UL		/* Define frequency here its 8MHz */

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)

void pin_init(){
    //MCUCR = (1 << SRE);
    DDRA = (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
    DDRE |= (1 << DDE1);
   
}

void pin_set(){
    PORTA = (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4) | (1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0);
   // PORTE = (1 << PE1);
}

void main(){
    pin_init();
    pin_set();

    while(1){
        PORTE |= (1 << PE1);
        PORTA = 0b11110000;
        PORTE &= (0 << PE1);
        //PORTA = (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4) | (1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0);
        _delay_ms(1000);
        //PORTA = (0 << PA7) | (0 << PA6) | (0 << PA5) | (0 << PA4) | (0 << PA3) | (0 << PA2) | (0 << PA1) | (0 << PA0);
        //_delay_ms(1000);
        PORTE |= (1 << PE1);
        PORTA = 0b00001111;
        PORTE &= (0 << PE1);
        _delay_ms(1000);
    }
}