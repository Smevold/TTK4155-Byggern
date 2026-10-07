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

void Read_Buttons();

void set_led(uint8_t led_n, uint8_t on);