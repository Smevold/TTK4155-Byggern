#pragma once

#include <stdint.h>

//#define EXT_ADC ((volatile uint8_t *) 0x1000)
#define EXT_RAM ((volatile uint8_t *) 0x1400)

// Digital value of joystick position
typedef struct {
    uint8_t joy_x;
    uint8_t joy_y;
    uint8_t pad_x;
    uint8_t pad_y;
} io_pos_t;

// Extremes of digital joystick position
typedef struct {
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_max;
    uint8_t y_min;
} io_joy_extreme_t;

// Percentile of joystick position, from 0-100
typedef struct {
    uint8_t x;
    uint8_t y;
} joy_readable;

// Direction of joystick
typedef enum {
    UP, DOWN, LEFT, RIGHT, NEUTRAL
} direction;

void ADC_Init();

void Joy_Init(io_joy_extreme_t* joy_extreme);

io_pos_t ADC_Read();

// Return percentile of joystick position in x direction
uint8_t Joy_Readable_X(io_joy_extreme_t* joy_extreme, uint8_t joy_x);

// Return percentile of joystick position in y direction
uint8_t Joy_Readable_Y(io_joy_extreme_t* joy_extreme, uint8_t joy_y);

// Return struct with percentile in both axis'
joy_readable Joy_Readable_Pos(io_joy_extreme_t* joy_extreme);

// Return direction of joystick
direction Joy_Direction(io_joy_extreme_t* joy_extreme);

// Change extremes of digital value of joystick position or calibrate joystick
void Joy_Calibrate(io_joy_extreme_t* joy_extreme);