#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"
#include "CAN_controller_driver.h"

#define MCP_CAN_CRT         0x0F
#define MCP_CAN_ABAT_En     0x01  
#define MCP_CAN_OSM_Dis     0x00
#define MCP_CAN_CLKOut_Dis  0x00
#define MCP_CAN_CLKPre_1    0x00

#define MODE_Normal         0x00
#define MODE_Loopback       0x40
#define MODE_Mask           0xE0

void CAN_init(){
    // Slave select
    SPI_SlaveSelect(SS_CAN);
     
    //Reset pin HIGH HUSKER IKKE PIN LOL
    SPI_Transmit(MCP_CAN_RST);

    SPI_SlaveDeselectAll();

    // New command
    SPI_SlaveSelect(SS_CAN);
    SPI_Transmit(MCP_CAN_CRT); // CAN Control register
    // clk prescaler CLK/1, clk out disable, osm disable, abat request, loop back mode
    SPI_Transmit((MODE_Loopback << 5) | (MCP_CAN_ABAT_En << 4) | (MCP_CAN_OSM_Dis << 3) | (MCP_CAN_CLKOut_Dis << 2) | MCP_CAN_CLKPre_1);

    SPI_SlaveDeselectAll();
    SPI_SlaveSelect(SS_CAN);

    // Check if CAN is correct
    uint8_t stat = CAN_Read_Controller(MCP_CAN_Read_Status);
    if ((stat & MODE_Mask) != MODE_Loopback) {
        // failed
    }
    SPI_SlaveDeselectAll();
}