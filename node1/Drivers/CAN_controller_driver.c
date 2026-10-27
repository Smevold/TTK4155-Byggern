#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"
#include "CAN_controller_driver.h"


//RTS 
#define MCP_RTS      0x80
#define MCP_RTS_TX0  0x01
#define MCP_RTS_TX1  0x02
#define MCP_RTS_TX2  0x04

//CAN_Read
#define MCP_CAN_Read 0b00000011
#define MCP_CAN_Write 0b00000010
#define MCP_CAN_Bit_Modify 0b00000101
#define MCP_CAN_RST 0b11000000



// The MCP2515 expects the first byte after
//lowering CS to be the instruction/
//command byte. This implies that CS must
//be raised and then lowered again to
//invoke another command.

uint8_t CAN_Read_Controller(uint8_t address){
    SPI_SlaveSelect(SS_CAN);

    SPI_Transmit(MCP_CAN_Read);
    SPI_Transmit(address);
    uint8_t data = SPI_Receive();
    
    SPI_SlaveDeselectAll();
    return data;
}

void CAN_Write_Controller(uint8_t address, uint8_t data){
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_CAN_Write);
    SPI_Transmit(address);
    SPI_Transmit(data);

     // CS HIGH    
    SPI_SlaveDeselectAll();
}


// RTS
void CAN_Request_To_Send(uint8_t TXREQ_mask){ 
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_RTS | (TXREQ_mask & 0x07));
    
    // This command will set the TxBnCTRL.TXREQ bit for
    //the respective buffer(s)

    SPI_SlaveDeselectAll();
}



//if (s & (1 << 0)) { /* message waiting in RXB0 */ }
//if (s & (1 << 1)) { /* message waiting in RXB1 */ }
//if (s & (1 << 2)) { /* TXB0 transmission still pending */ }
//if (s & (1 << 3)) { /* TXB0 finished sending */ }

uint8_t CAN_Read_Status(){
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_CAN_Read_Status);
    uint8_t status = SPI_Receive();

    SPI_SlaveDeselectAll(); 
    
    return status;
}

void CAN_Bit_Modify(uint8_t address, uint8_t mask_byte, uint8_t data){
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_CAN_Bit_Modify);
    SPI_Transmit(address);
    SPI_Transmit(mask_byte);
    SPI_Transmit(data);

     SPI_SlaveDeselectAll();
}

void CAN_Controller_RST(){
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_CAN_RST);

    // CS HIGH
    SPI_SlaveDeselectAll();

     _delay_ms(1); 
}