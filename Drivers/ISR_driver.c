#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "ISR_driver.h"
#include "IO_driver.h"

// To find the interrupt vectors, look up page Table 24 and 25 on page 57 and 58 in ATmega162 datasheet
// Also look at lab manual to see how interrupts 


// Interrupt handler to update Joystick position and direction
ISR(TIMER1_OVF_vect){
    // Read from the ADC to update values
    ADC_Read(joy, pad);

    // Update percentile values
    JOY_Readable_Pos(joy);

    // Update direction of joystick
    JOY_Direction(joy);
}

// Interrupt handler to calibrate joystick
ISR(TIMER0_OVF_vect){
    // Update positional data
    ADC_Read(joy, pad);

    // Calibrate extremes of position
    JOY_Calibrate(joy);
}