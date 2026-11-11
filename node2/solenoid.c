#include "solenoid.h"

// Init for Solenoid, to enable relevant pin
void Solenoid_Init(){    
    PIOC->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOC->PIO_PER |= PIO_PC28; // Enable register pin C28
    PIOC->PIO_OER |= PIO_PC28; // Output enable register pin C28

    PIOC->PIO_CODR = PIO_PC28; //  Set output low
    
}

// Pulses Solenoid to expand for 100 ms
void Solenoid_Hit(){
    PIOC->PIO_CODR = PIO_PC28; // Set output high, solenoid expand
    uint32_t start_tick = ticks;
    while (ticks < start_tick + 5) {;}
    PIOC->PIO_SODR = PIO_PC28; // Set output low, solenoid contraxt
    start_tick = ticks;
    while (ticks < start_tick + 5) {;}
}