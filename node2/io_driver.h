#pragma once

#include <stdint.h>
#include "sam.h"

typedef enum {
    UP, DOWN, LEFT, RIGHT, NEUTRAL
} direction_t;

typedef struct {
    // Percentile of x and y position from 0 - 100
    uint8_t x;
    uint8_t y;

    // Digital value of voltage in x and y direction
    uint8_t x_dig;
    uint8_t y_dig;

    // Maximums and minimums of x and y digital voltage value
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_max;
    uint8_t y_min;

    // Enum of direction 
    direction_t dir;

    // Button
    uint8_t btn;
} io_joy_t;

// Return percentile of joystick position in x direction
void JOY_Readable_X(io_joy_t* joy);

// Return percentile of joystick position in y direction
void JOY_Readable_Y(io_joy_t* joy);

// Return struct with percentile in both axis'
void JOY_Readable_Pos(io_joy_t* joy);