#include "motor.h"

// Deadspace for motor direction
#define DEADSPACE 5

void Motor(io_joy_t* joy, uint16_t* last_us) {
    // Take in joystick x and y
    JOY_Readable_Pos(joy);

    // Change PWM
    PWM_Joy_Duty_Cycle(joy->y, last_us, 0);

    // Change dir based on input from joystick
    if (joy.x > 50 + DEADSPACE) {
        // SET PIN 23 PORT C HIGH
    } else if (joy.x < 50 - DEADSPACE) {
        // SET PIN 23 PORT C LOW
    }


}