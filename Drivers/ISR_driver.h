// Interrupt Service Routine

#pragma once

#include <stdint.h>


// Interrupt handler for joystick reading
ISR(TIMER1_OVF_vect);