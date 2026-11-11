#include "motor.h"

// Deadspace for motor direction
#define DEADSPACE 5

void Motor_Init() {
    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOC->PIO_PER |= PIO_PC23; // Enable register pin 23
    PIOC->PIO_OER |= PIO_PC23; // Output enable register pin 23
}

// This function cannot take in joy, must take in throttle_signal
void Motor(pid_ctrl_t* pid , uint16_t* last_us) {
    // Change PWM
    PWM_Joy_Duty_Cycle((abs(pid->output)), last_us, 0);
    
    // Change dir based on input from joystick
    if (pid->output > 0 + DEADSPACE) {
        // SET PIN 23 PORT C HIGH
        PIOC->PIO_SODR |= PIO_PC23;
    } else if (pid->output < 0 - DEADSPACE) {
        // SET PIN 23 PORT C LOW
        PIOC->PIO_CODR |= PIO_PC23;
    }


}

void Motor_Get_Position(motor_t* motor){

    motor->p_measured = TC2->TC_CHANNEL[0].TC_CV;
    
}