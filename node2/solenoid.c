#include "solenoid.h"

void Solenoid_Init(){
    // Change Pin to something else later and uncomment
    
    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOC->PIO_PER |= PIO_PC28; // Enable register pin ??
    PIOC->PIO_OER |= PIO_PC28; // Output enable register pin ??
    
}

void Solenoid_Hit(){
    printf("Sett pin high");
    PIOC->PIO_SODR = PIO_PC28;
    printf("Ticks");
    uint16_t start_tick = ticks;
    printf("Wait");
    while (ticks < start_tick + 100) {;} // should replace with interrupt
    printf("Set pin low");
    PIOB->PIO_CODR = PIO_PC28;
}