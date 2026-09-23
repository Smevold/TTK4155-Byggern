#include "SPI_driver.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"

void SPI_Init() {
    // Set MOSI and SCK as output
    DDRB |= (1 << DDB5) | (1 << DDB7);

    // Set SS_AVR, SS_OLED, SS_EMPTY as output
    DDRB |= (1 << DDB4) | (1 << DDB3) | (1 << DDB2);

    // Deselect all SS
    PORTB |= (1 << PB4) | (1 << PB3) | (1 << PB2);

    // Enable SPI and Master, set clk rate to fck/16
    // MSB is transmitted first
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_Transmit(char data) {
    // Start transmission
    SPDR = data;

    // Wait for transmission completion
    // SPIF is set when transmission is complete
    while (!(SPSR & (1 << SPIF))){;}
}

char SPI_Receive() {
    // Wait for receive completion
    while(!(SPSR & (1 << SPIF))){;}

    return SPDR;
}

void SPI_Transmit_n_Bytes(char* data, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        SPI_Transmit(data[i]);
    }
}

// Write "char* data[n];" to create input for this function
void SPI_Receive_n_Bytes(char* data, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        data[i] = SPI_Receive;
    }
}

void SPI_SlaveSelect(uint8_t slave) {
    switch (slave) {
        case SS_IO_AVR: 
            PORTB |= (1 << PB3) | (1 << PB2);
            PORTB &= ~(1 << PB4); 
            break;
        case SS_OLED: 
            PORTB |= (1 << PB4) | (1 << PB2);
            PORTB &= ~(1 << PB3); 
            break;
        case SS_EMPTY: 
            PORTB |= (1 << PB4) | (1 << PB3);
            PORTB &= ~(1 << PB2); 
            break;
    }
}