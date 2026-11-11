#include "io_driver.h"

// This value works as the threshold from the converted and filtered IR signal
// Might need refining
#define IR_Threshold 1024

// Same as node 1
void JOY_Readable_X(io_joy_t* joy) {
    joy->x = (joy->x_dig - joy->x_min) * 100 / (joy->x_max - joy->x_min);
}

// 
void JOY_Readable_Y(io_joy_t* joy) {
    joy->y = (joy->y_dig - joy->y_min) * 100 / (joy->y_max - joy->y_min);
}

void JOY_Readable_Pos(io_joy_t* joy) {
    JOY_Readable_X(joy);
    JOY_Readable_Y(joy);
}

uint8_t IR_result() {
    // Read from ADC
    uint16_t ir_value = ADC_Read();

    // If value from IR exceeds threshold, return 1
    if (ir_value > IR_Threshold) {return 1;}

    // Otherwise return 0
    return 0;
}