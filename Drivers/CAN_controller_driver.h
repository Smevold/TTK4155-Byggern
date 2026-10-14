#pragma once

#include <stdint.h>


void CAN_init();
void CAN_Read_Controller(uint8_t address);
void CAN_Write_Controller(uint8_t address, uint8_t data);
void CAN_Request_To_Send(uint8_t TXREQ_mask);
void CAN_Read_Status();
void CAN_Bit_Modify(uint8_t address, uint8_t mask_byte, uint8_t data);
void CAN_Controller_RST();
