#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"

// The MCP2515 expects the first byte after
//lowering CS to be the instruction/
//command byte. This implies that CS must
//be raised and then lowered again to
//invoke another command.

void CAN_init(){
    // Slave select
    SPI_SlaveSelect(SS_CAN);
    
     // CS low
    PORTB &= ~(1 << PB2); 
    //Reset pin low elns
    PORTB &= ~(1 << PB2); 
}

void CAN_Read_Controller(){
    CAN_init();
    SPI_Transmit(0b00000011);
    SPI_Receive();
    
}

void CAN_Write_Controller(){
    CAN_init();
    SPI_Transmit(0b00000010);

    
}


// RTS
void CAN_Request_To_Send(char TXREQ_bit){ 
    CAN_init();
    SPI_Transmit(0b100);
    SPI_Receive();
    // This command will set the TxBnCTRL.TXREQ bit for
    //the respective buffer(s)


}

void CAN_Read_Status(){
    CAN_init();
    SPI_Transmit(0b10100000);
    SPI_Receive();

    
}

void CAN_Bit_Modify(){
    CAN_init();
    SPI_Transmit(0b00000101)

}

void CAN_Controller_RST(){
    CAN_init();
    SPI_Transmit(0b11000000);
    // CS HIGH
    PORTB &= ~(1 << PB2);

}