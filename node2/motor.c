#include "motor.h"

// Deadspace for motor direction
#define DEADSPACE 5

void Motor_Init() {
    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOB->PIO_PER |= PIO_PB23; // Enable register pin 13
    PIOB->PIO_OER |= PIO_PB23; // Output enable register pin 13
}

// This function cannot take in joy, must take in throttle_signal
void Motor(io_joy_t* joy, uint16_t* last_us) {
    // Take in joystick x and y
    JOY_Readable_Pos(joy);

    // Change PWM
    PWM_Joy_Duty_Cycle(joy->x, last_us, 0);
    
    // Change dir based on input from joystick
    if (joy->x > 50 + DEADSPACE) {
        // SET PIN 23 PORT C HIGH
        PIOB->PIO_SODR |= PIO_PB23;
    } else if (joy->x < 50 - DEADSPACE) {
        // SET PIN 23 PORT C LOW
        PIOB->PIO_SODR &= ~PIO_PB23;
    }


}

u_int32_t position_motor(){

    u_int32_t  position_motor = TC2->TC_CHANNEL[0].TC_CV;
    return position_motor;
}