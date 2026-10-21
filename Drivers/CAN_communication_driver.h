#pragma once

#include <stdint.h>

typedef struct{
    uint16_t ID;
    uint8_t DLC;
    uint8_t Data;


} CAN_MSG;

void CAN_Init();
void CAN_Send_Message(CAN_MSG* msg);