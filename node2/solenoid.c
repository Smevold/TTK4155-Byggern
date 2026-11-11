#include "solenoid.h"

void Solenoid_Init(){
    // USING PINC28 OR PIN D3
    
    PIOC->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOC->PIO_PER |= PIO_PC28; // Enable register pin 28 port c
    PIOC->PIO_OER |= PIO_PC28; // Output enable register pin 28 port c

    PIOC->PIO_CODR = PIO_PC28; // Clear output on pin
    
}

void Solenoid_Hit(){
    PIOC->PIO_CODR = PIO_PC28;
    uint32_t start_tick = ticks;
    while (ticks < start_tick + 5) {;} // should replace with interrupt
    PIOC->PIO_SODR = PIO_PC28;
    start_tick = ticks;
    while (ticks < start_tick + 5) {;}
}