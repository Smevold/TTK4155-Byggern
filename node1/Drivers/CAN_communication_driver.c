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


// CAN Controll REGISTER
#define MCP_CAN_CRT         0x0F
#define MCP_CAN_ABAT_En     0x01  
#define MCP_CAN_OSM_Dis     0x00
#define MCP_CAN_CLKOut_Dis  0x00
#define MCP_CAN_CLKPre_1    0x00

// CAN Controller modes
#define MODE_Normal         0x00
#define MODE_Loopback       0x40
#define MODE_Mask           0xE0

// transmitt buffer
#define EXIDE               0x03
// Bit timing config reg
// config 1 
//Sync jump width length bits
#define CNF1                0x2A
//Bit time length bit, Sample point config bit, PS1 length bit, Propagation segment lenght bits
#define CNF2                0x29
// sets start of frame signal bit, wake-up filter bit PS2 length bit
#define CNF3                0x28

#define CANINTF             0x2C

void CAN_Init(){
    //Reset 
    CAN_Controller_RST();

    // Bit timing
    // writes to config 1, length 4 x Tq
    CAN_Write_Controller(CNF1, 0x03); // 0b00000011
    // writes to config 2, 
    CAN_Write_Controller(CNF2, 0xF0); // 0b11110000
    // writes to config 3, CLKOUT pin enable for SOF sig PS2 length bit
    CAN_Write_Controller(CNF3, 0x86); // 0b10000110
    
    // Enable interrupt TX buffer
    CAN_Write_Controller(0x2b, 0b00011100);
    DDRD &= ~(1 << DDD2); // Set interrupt pin as input

    // Ignore all mask/filters in RX buffer
    CAN_Bit_Modify(0x60, 0b01100000, 0b01100000);

    // New command
    CAN_Bit_Modify(0x0F, 0xE0, MODE_Normal);


    // Check if CAN is correct
    uint8_t stat = CAN_Read_Controller(0x0E);
    if ((stat & MODE_Mask) != MODE_Normal) {
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
    // Message ID 
    CAN_Write_Controller(0x31, (msg->ID >> 3));
    CAN_Write_Controller(0x32, ((msg->ID << 5) | (0 <<  EXIDE)));
    // (0 << 6) does nothing, but RTR is cleared: Transmitted Message will be a Data Frame
    // (msg->DLC & 0x0F) mask out the 4 most significant bits
    CAN_Write_Controller(0x35, ((0 << 6) | (msg->DLC & 0x0F)));

    // Writes bytes to transmit buffer(s). bytes where DLC is how many bytes to write
    for (uint8_t i = 0; i < msg->DLC; i++) {
        CAN_Write_Controller(0x36+i, msg->Data[i]);
    }
    // Only writes to relevant buffers
    CAN_Request_To_Send(0x01);
    CAN_Bit_Modify(0x30, 0x07, 0x07);

}

void CAN_Receive_Message(CAN_MSG* msg){
    uint8_t timeout = 10000000000;
    //Timeout when no message recived
    while (!(CAN_Read_Controller(CANINTF) & 0x01)) {
        if(--timeout == 0) {
            printf("Timed out CAN_Receive\n\r");
            return;
        };
    } 
    // message adress register is read and DLC
    msg->ID = (CAN_Read_Controller(0x61) << 3);
    msg->ID = (msg->ID | (CAN_Read_Controller(0x62) >> 5));
    msg->DLC = (CAN_Read_Controller(0x65) & 0x0F);

    // loop for reading of multiple bytes
    for (uint8_t i = 0; i < msg->DLC; i++) {
        //reads revice buffer(s)
        msg->Data[i] = CAN_Read_Controller(0x66+i);
    }

    CAN_Bit_Modify(CANINTF, 0x01, 0x00); // Clear RX0IF
}

void CAN_Send_Joy(io_joy_t* joy) {
    CAN_MSG msg;
    msg.ID = 0x02; // Random ID
    msg.DLC = 0x07; // Send 7 bytes

    // Fill tx buffers
    msg.Data[0] = joy->x_max;
    msg.Data[1] = joy->x_min;
    msg.Data[2] = joy->y_max;
    msg.Data[3] = joy->y_min;
    msg.Data[4] = joy->x_dig;
    msg.Data[5] = joy->y_dig;
    msg.Data[6] = joy->btn;

    // Sends tx buffer
    CAN_Send_Message(&msg);
}