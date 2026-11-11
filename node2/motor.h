#pragma once

#include "sam.h"
#include <stdint.h>
#include <stdlib.h>

#include "io_driver.h"
#include "adc.h"
#include "interrupt.h"
#include "control.h"

// If not more is needed for the position regulator, consider not using a struct
typedef struct motor_t{
    // Position should be the calculated position based on encoder
    uint16_t p_measured; // Just guessed type-size, should check what is needed
    // Ref should be the reference-value
    // Calculated by adding together the joystick values?
    // We should consider changing to the pad, for position based control
    uint16_t p_ref; 
} motor_t;

typedef struct pid_ctrl_t pid_ctrl_t;

void Motor_Init();

// Motor direction with regulator
void Motor(pid_ctrl_t* pid , uint16_t* last_us);

// Get motor position measued with quadric encoder
void Motor_Get_Position(motor_t* motor);