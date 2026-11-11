#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"
#include "CAN_controller_driver.h"


// RTS 
#define MCP_RTS      0x80
#define MCP_RTS_TX0  0x01
#define MCP_RTS_TX1  0x02
#define MCP_RTS_TX2  0x04

// Instructions bytes for CAN Controller
#define MCP_CAN_Read 0b00000011
#define MCP_CAN_Write 0b00000010
#define MCP_CAN_Bit_Modify 0b00000101
#define MCP_CAN_RST 0b11000000







uint8_t CAN_Read_Controller(uint8_t address){
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);

    
    // Send read command
    SPI_Transmit(MCP_CAN_Read);
    // Address to register to read from
    SPI_Transmit(address);
    // Sends the data out on the SO pin
    uint8_t data = SPI_Receive();
    
    // Deselect
    SPI_SlaveDeselectAll();
    return data;
}

void CAN_Write_Controller(uint8_t address, uint8_t data){
    
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);
    // Send write command
    SPI_Transmit(MCP_CAN_Write);
    // Adress to register to send to
    SPI_Transmit(address);
    // Sends the data
    SPI_Transmit(data);

    // Deselect  
    SPI_SlaveDeselectAll();
}


// RTS
void CAN_Request_To_Send(uint8_t TXREQ_mask){ 
    
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);
    //Sends RTS command, last 3 bits (TXREQ_mask)  
    //indicate which transmit buffer enabled
    SPI_Transmit(MCP_RTS | (TXREQ_mask & 0x07));

    // Deselect
    SPI_SlaveDeselectAll();
}



//if (s & (1 << 0)) { /* message waiting in RXB0 */ }
//if (s & (1 << 1)) { /* message waiting in RXB1 */ }
//if (s & (1 << 2)) { /* TXB0 transmission still pending */ }
//if (s & (1 << 3)) { /* TXB0 finished sending */ }

uint8_t CAN_Read_Status(){
    
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);
    // Read status command sendt
    SPI_Transmit(MCP_CAN_Read_Status);
    // status recive
    uint8_t status = SPI_Receive();

    // Deselect
    SPI_SlaveDeselectAll(); 
    
    return status;
}

void CAN_Bit_Modify(uint8_t address, uint8_t mask_byte, uint8_t data){
    
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);
    // Bit modify command byte send
    SPI_Transmit(MCP_CAN_Bit_Modify);
    // Sends Addres to edit, which bits to and 
    // data what it changes to
    SPI_Transmit(address);
    SPI_Transmit(mask_byte);
    SPI_Transmit(data);

    // Deselect
     SPI_SlaveDeselectAll();
}

void CAN_Controller_RST(){
    
    // Slave select CAN
    SPI_SlaveSelect(SS_CAN);

    // reset  command
    SPI_Transmit(MCP_CAN_RST);

    // Deselect
    SPI_SlaveDeselectAll();
    // normal delat after reset
     _delay_ms(1); 
}