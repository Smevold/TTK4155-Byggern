#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"
#include "CAN_controller_driver.h"
#include "CAN_communication_driver.h"

#define MCP_CAN_CRT         0x0F
#define MCP_CAN_ABAT_En     0x01  
#define MCP_CAN_OSM_Dis     0x00
#define MCP_CAN_CLKOut_Dis  0x00
#define MCP_CAN_CLKPre_1    0x00

#define MODE_Normal         0x00
#define MODE_Loopback       0x40
#define MODE_Mask           0xE0

#define EXIDE               0x03
#define CNF1                0x2A
#define CNF2                0x29
#define CNF3                0x28

#define CANINTF             0x2C

void CAN_Init(){
    //Reset 
    CAN_Controller_RST();

    // Bit timing
    CAN_Write_Controller(CNF1, 0x03); 
    CAN_Write_Controller(CNF2, 0xF0); 
    CAN_Write_Controller(CNF3, 0x86); 
    
    // Enable interrupt transmit buffer
    CAN_Write_Controller(0x2b, 0b00011100);
    DDRD &= ~(1 << DDD2); // Set interrupt pin as input

    // Ignore all mask/filters in RX buffer
    CAN_Bit_Modify(0x60, 0b01100000, 0b01100000);

    // New command
    CAN_Write_Controller(MCP_CAN_CRT, MODE_Loopback | (MCP_CAN_ABAT_En << 4) | (MCP_CAN_OSM_Dis << 3) | (MCP_CAN_CLKOut_Dis << 2) | MCP_CAN_CLKPre_1); // CAN Control register


    // Check if CAN is correct
    uint8_t stat = CAN_Read_Controller(0x0E);
    if ((stat & MODE_Mask) != MODE_Loopback) {
        // failed
    }




}

/*
char CAN_RX_Buffer(uint8_t adr_mask){
    SPI_SlaveSelect(SS_CAN);


    SPI_Transmit(0b10010000 | (adr_mask << 1));
    char data = SPI_Receive();
    SPI_SlaveDeselectAll();

    return data;
}

void CAN_TX_Buffer(uint8_t data, uint8_t adr_mask){
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(0b01000000 | (adr_mask));
    SPI_Transmit(data);
    SPI_SlaveDeselectAll();

}
*/


void CAN_Send_Message(CAN_MSG* msg){
    
    CAN_Write_Controller(0x31, (msg->ID >> 3));
    CAN_Write_Controller(0x32, ((msg->ID << 5) | (0 <<  EXIDE)));
    CAN_Write_Controller(0x35, ((0 << 6) | (msg->DLC & 0x0F)));

    CAN_Write_Controller(0x36, msg->Data);
    CAN_Request_To_Send(0x01);
    CAN_Bit_Modify(0x30, 0x07, 0x07);

}

void CAN_Receive_Message(CAN_MSG* msg){
    while (!(CAN_Read_Controller(CANINTF) & 0x01)) {;} // Consider adding a timeout
    msg->ID = (CAN_Read_Controller(0x61) << 3);
    msg->ID = (msg->ID | (CAN_Read_Controller(0x62) >> 5));
    msg->DLC = (CAN_Read_Controller(0x65) & 0x0F);
    msg->Data = CAN_Read_Controller(0x66);


    CAN_Bit_Modify(0x2c, 0x03, 0x00);
}