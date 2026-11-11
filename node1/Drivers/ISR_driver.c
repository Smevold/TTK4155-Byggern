#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "ISR_driver.h"
#include "IO_driver.h"



void ISR_Init(){
    // Enable CTC on Timer/Counter0
    TCCR3A &= ~(1 << WGM31) & ~(1 << WGM30) ; // WGM30 og WGM31
    TCCR3B |= (1 << WGM32); // WGM32
    TCCR3B &= ~(1 << WGM33); // WGM33

    // Normal Pin operation on OC3A and OC3B
    TCCR3A &= ~((1 << COM3A1) | (1 << COM3A0) | (1 << COM3B1) | (1 << COM3B0));

    // Set clk_IO/1024
    TCCR3B |= (1 << CS32) | (1 << CS30);
    TCCR3B &= ~(1 << CS31);

    // Set Output Compare for 3 seconds
    // Need to set both high and low
    // Can use OCR3BH/L for a second compare?
    OCR3A = 0x383E;

    // Enable timer compare interrupt on Timer/Counter3
    ETIMSK |= (1 << OCIE3A);
}

// External variable 
volatile int loop = 1;

// Interrupt handler to calibrate joystick
// Stops loop that calibrates joystick
ISR(TIMER3_COMPA_vect){
    loop = 0;
}
