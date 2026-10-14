#pragma once

#include <stdint.h>


void CAN_init();
void CAN_Read_Controller(char address);
void CAN_Write_Controller(char address, char data);
void CAN_Request_To_Send(char TXREQ_bit);
void CAN_Read_Status();
void CAN_Bit_Modify(char address, char mask_byte, char data);
void CAN_Controller_RTS();