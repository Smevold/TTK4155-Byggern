#include <stdio.h>
#include <stdlib.h>
#include "sam.h"
#include "motor.h"

typdedef struct{
    uint8_t Kp;
    uint8_t Ki;
    uint8_t Kd; // Possibly not needed
    uint8_t output; // Just guessed type-size, should check
}