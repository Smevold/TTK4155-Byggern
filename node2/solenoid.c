#include "solenoid.h"

void Solenoid_Init(){
    // Change Pin to something else later and uncomment
    /*
    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOB->PIO_PER |= PIO_PB23; // Enable register pin ??
    PIOB->PIO_OER |= PIO_PB23; // Output enable register pin ??
    */
}

void Solenoid_Hit(){
//    PIOB->PIO_SODR |= PIO_PB23;
    uint16_t start_tick = ticks;
    while (ticks < start_tick + 5) {} // should replace with interrupt
//    PIOB->PIO_SODR &= ~PIO_PB23;
}