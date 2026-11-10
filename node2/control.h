#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sam.h"
#include "motor.h"

typedef struct{
    uint16_t error_sum;
    uint16_t error_prev;
    uint8_t output; // Just guessed type-size, should check
} pid_ctrl_t;