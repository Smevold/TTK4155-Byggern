#pragma once

#include <stdint.h>

//#define EXT_ADC ((volatile uint8_t *) 0x1000)
#define EXT_RAM ((volatile uint8_t *) 0x1400)

// Digital value of IO devices
typedef struct {
    uint8_t joy_x;
    uint8_t joy_y;
    uint8_t pad_x;
    uint8_t pad_y;
} io_pos_t;

// Directions for joystick
typedef enum {
    UP, DOWN, LEFT, RIGHT, NEUTRAL
} direction_t;

// Functional values for joystick
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_max;
    uint8_t y_min;
    direction_t dir;
} io_joy_t;

void ADC_Init();

void JOY_Init(io_joy_t* joy_extreme);

io_pos_t ADC_Read();

// Return percentile of joystick position in x direction
void JOY_Readable_X(io_joy_t* joy, uint8_t* x_digital);

// Return percentile of joystick position in y direction
void JOY_Readable_Y(io_joy_t* joy, uint8_t* y_digital);

// Return struct with percentile in both axis'
void JOY_Readable_Pos(io_joy_t* joy, io_pos_t* joy_digital);

// Return direction of joystick
void JOY_Direction(io_joy_t* joy);

// Change extremes of digital value of joystick position or calibrate joystick
void JOY_Calibrate(io_joy_t* joy, io_pos_t* joy_digital);