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
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK);

    // Enable SPI and Master, set clk rate to fck/16
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_Transmit(char cData) {
    // Start transmission
    SPDR = cData;

    // Wait for transmission completion
    while (!(SPSR & (1 << SPIF))){;}
}

char SPI_Receive() {
    // Wait for receive completion
    while(!(SPSR & (1<<SPIF))){;}

    return SPDR;
}