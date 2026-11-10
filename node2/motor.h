#pragma once

#include "sam.h"
#include <stdint.h>

#include "io_driver.h"
#include "adc.h"
#include "interrupt.h"
#include "motor.h"

// If not more is needed for the position regulator, consider not using a struct
typedef struct {
    uint16_t position; // Just guessed type-size, should check what is needed
    // MORE???
} motor_t;

void Motor_Init();
void Motor(io_joy_t* joy, uint16_t* last_us);
