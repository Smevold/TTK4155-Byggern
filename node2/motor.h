#pragma once

#include "sam.h"
#include <stdint.h>

#include "io_driver.h"
#include "adc.h"
#include "interrupt.h"
#include "motor.h"
#include "control.h"

// If not more is needed for the position regulator, consider not using a struct
typedef struct {
    // Encoder should be real-time encoder-value
    uint16_t encoder; // Just guessed type-size
    // Position should be the calculated position based on encoder
    uint16_t p_measured; // Just guessed type-size, should check what is needed
    // Ref should be the reference-value
    // Calculated by adding together the joystick values?
    // We should consider changing to the pad, for position based control
    uint16_t p_ref; 
} motor_t;

void Motor_Init();
void Motor(pid_ctrl_t* pid , uint16_t* last_us);

void Motor_Get_Position(motor_t* motor);