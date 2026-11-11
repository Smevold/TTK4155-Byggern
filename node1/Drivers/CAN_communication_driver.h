#pragma once

#include <stdint.h>

// Struct for CAN message 
typedef struct{
    uint16_t ID;
    // Data length Character
    uint8_t DLC;
    uint8_t Data[8];
} CAN_MSG;

// Initilize CAN controller
void CAN_Init();
// Send message to trancive buffer from CAN controller
void CAN_Send_Message(CAN_MSG* msg);
// Reads recive buffer from CAN controller
void CAN_Receive_Message(CAN_MSG* msg);
// Sends joystick data from CAN controller
void CAN_Send_Joy(io_joy_t* joy);