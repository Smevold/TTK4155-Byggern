#include "motor.h"

// Deadspace for motor direction
#define DEADSPACE 5

void Motor_Init() {
    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOB->PIO_PER |= PIO_PC23; // Enable register pin 13
    PIOB->PIO_OER |= PIO_PC23; // Output enable register pin 13
}

// This function cannot take in joy, must take in throttle_signal
void Motor(io_joy_t* joy, uint16_t* last_us) {
    // Take in joystick x and y
    JOY_Readable_Pos(joy);

    // Change PWM
    PWM_Joy_Duty_Cycle(joy->y, last_us, 0);
    
    // Change dir based on input from joystick
    if (joy.x > 50 + DEADSPACE) {
        // SET PIN 23 PORT C HIGH
        PIOB->PIO_SODR |= PIO_PC23;
    } else if (joy.x < 50 - DEADSPACE) {
        // SET PIN 23 PORT C LOW
        PIOB->PIO_SODR &= ~PIO_PC23;
    }


}