#pragma once

#include <stdint.h>

#define MCP_CAN_Read_Status 0b10100000
#define MCP_CAN_RST 0b11000000

uint8_t CAN_Read_Controller(uint8_t address);
void CAN_Write_Controller(uint8_t address, uint8_t data);
void CAN_Request_To_Send(uint8_t TXREQ_mask);
uint8_t CAN_Read_Status();
void CAN_Bit_Modify(uint8_t address, uint8_t mask_byte, uint8_t data);
void CAN_Controller_RST();
