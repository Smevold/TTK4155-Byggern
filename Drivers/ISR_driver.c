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
    TCCR3A &= ~(1 << COM01) & ~(1 << COM00);
    TCCR3B &= ~(1 << COM01) & ~(1 << COM00);

    // Set clk_IO/1024
    TCCR3B |= (1 << CS32) | (1 << CS30);
    TCCR3B &= ~(1 << CS31);

    // Set Output Compare for 3 seconds
    // Need to set both high and low
    // Can use OCR3BH/L for a second compare?
    OCR3A = 0x1C1F;

    // Enable timer overflow interrupt on Timer/Counter3
    ETIMSK |= (1 << TOIE3);
}


// To find the interrupt vectors, look up page Table 24 and 25 on page 57 and 58 in ATmega162 datasheet
// Also look at lab manual to see how interrupts 


// Interrupt handler to update Joystick position and direction
ISR(TIMER1_OVF_vect){
    // Read from the ADC to update values
    ADC_Read(&joy, &pad);

    // Update percentile values
    JOY_Readable_Pos(&joy);

    // Update direction of joystick
    JOY_Direction(&joy);
}

// Interrupt handler to calibrate joystick
ISR(TIMER0_OVF_vect){
    // Update positional data
    ADC_Read(&joy, &pad);

    // Calibrate extremes of position
    JOY_Calibrate(&joy);
}
