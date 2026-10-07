#pragma once

#include <stdint.h>

//#define EXT_ADC ((volatile uint8_t *) 0x1000)
#define EXT_RAM ((volatile uint8_t *) 0x1400)

// Digital value of IO devices
typedef struct {
    uint8_t pad_x;
    uint8_t pad_y;
} io_pad_t;

// Directions for joystick
typedef enum {
    UP, DOWN, LEFT, RIGHT, NEUTRAL
} direction_t;

// Functional values for joystick
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
} io_joy_t;

// button
typedef struct __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
} Buttons;

void ADC_Init();

void JOY_Init(io_joy_t* joy_extreme);

void ADC_Read(io_joy_t* joy, io_pad_t* pad);

// Return percentile of joystick position in x direction
void JOY_Readable_X(io_joy_t* joy);

// Return percentile of joystick position in y direction
void JOY_Readable_Y(io_joy_t* joy);

// Return struct with percentile in both axis'
void JOY_Readable_Pos(io_joy_t* joy);

// Return direction of joystick
void JOY_Direction(io_joy_t* joy);

// Change extremes of digital value of joystick position or calibrate joystick
void JOY_Calibrate(io_joy_t* joy);

void BTN_Read();

void LED_Set(uint8_t led_n, uint8_t on);