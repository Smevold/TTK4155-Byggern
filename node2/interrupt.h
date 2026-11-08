#pragma once

#include <stdint.h>
#include "sam.h"

void Timer_Init();

extern volatile uint32_t ticks;

void PWM_Init();
void PWM_Set_Duty_Cycle_us(uint16_t us);
void PWM_Joy_Duty_Cycle(uint8_t joy_pos, uint16_t* last_us);