#pragma once

#include <stdint.h>
#include "sam.h"

void Timer_Init();

extern volatile uint32_t ticks;

void PWM_Init();