#include "solenoid.h"

void Solenoid_Init(){
    // Change Pin to something else later and uncomment
    
    PIOC->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOC->PIO_PER |= PIO_PC28; // Enable register pin ??
    PIOC->PIO_OER |= PIO_PC28; // Output enable register pin ??
    
}

void Solenoid_Hit(){
    printf("Sett pin high");
    PIOC->PIO_SODR = PIO_PC28;
    printf("Ticks");
    uint32_t start_tick = ticks;
    printf("Wait");
    while (ticks < start_tick + 5) {;} // should replace with interrupt
    printf("Set pin low");
    PIOC->PIO_CODR = PIO_PC28;
    start_tick = ticks;
    while (ticks < start_tick + 5) {;}
}