#pragma once

#include <stdint.h>

// Commando to CAN controller
#define MCP_CAN_Read_Status 0b10100000
#define MCP_CAN_RST 0b11000000

// Reads register from CAN controller address
uint8_t CAN_Read_Controller(uint8_t address);
// Writes to register CAN controller address
void CAN_Write_Controller(uint8_t address, uint8_t data);
// Request to send to specific registers
void CAN_Request_To_Send(uint8_t TXREQ_mask);
// Reads status for message reception and transmission for CAN controller
uint8_t CAN_Read_Status();
// Modify bits on sprecific adress for CAN controller
void CAN_Bit_Modify(uint8_t address, uint8_t mask_byte, uint8_t data);
// Reset CAN controller
void CAN_Controller_RST();
