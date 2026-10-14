// Interrupt Service Routine

#pragma once

#include <stdint.h>

void ISR_Init();

// Interrupt handler for joystick reading
ISR(TIMER1_OVF_vect);