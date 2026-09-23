#define F_CPU 4915200UL

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)

#include "tests/sram_test.h"
#include "tests/pin_test.h"
#include "Drivers/UART_driver.h"
#include "Drivers/IO_driver.h"
#include "Drivers/SPI_driver.h"

void pin_init(){ // Is probably only ext ram init
    MCUCR |= (1 << SRE);
    //DDRA = 0b11111111;
    //DDRA |= (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
    //DDRE |= (1 << DDE1);
    SFIOR |= (1 << XMM2);
    SFIOR &= ~((1 << XMM1) | (1 << XMM0));
}

/*
void pin_set(){
    PORTA = (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4) | (1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0);
   // PORTE = (1 << PE1);
}
   */



void main(){
    pin_init();
    UART_Init (BAUD_PRESCALE);
    ADC_init();

    while(1) {
        io_pos_t pos = ADC_read();
        printf("Joy_x: %2X, Joy_y: %2X, Pad_x: %2X, Pad_y: %2X\n", pos.joy_x, pos.joy_y, pos.pad_x, pos.pad_y);
    
        _delay_ms(20);
    }
}