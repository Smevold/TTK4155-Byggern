#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"
#include "CAN_controller_driver.h"

void CAN_init(){
    // Slave select
    SPI_SlaveSelect(SS_CAN);
    
    // CS low
    PORTB &= ~(1 << PB2); 
    //Reset pin HIGH HUSKER IKKE PIN LOL
    SPI_Transmit(0xC0);
    // clk out 1, clk out disable, osm disable, abat request, loop back mode
    SPI_Transmit(0b00001010);

    PORTB |= (1 << PB2); 

    uint8_t stat = CAN_Read_Controller(0x0E);
    if ((stat & 0xE0) != 0x40) {
        // failed
    }
}