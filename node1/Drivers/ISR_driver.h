// Interrupt Service Routine

#pragma once

#include <stdint.h>
#include <avr/interrupt.h>

void ISR_Init();

extern volatile int loop;

// Interrupt handler for joystick reading
ISR(TIMER3_OVF_vect);