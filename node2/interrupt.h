#pragma once

#include <stdint.h>
#include "sam.h"

void Timer_Init();


extern volatile uint32_t ticks;
extern volatile uint8_t pid_tick;

// Initialize PWM for AVR SAM
void PWM_Init();

// Change duty cycle in microseconds based on input
void PWM_Set_Duty_Cycle_us(uint16_t us, uint8_t channel);
void PWM_Set_Duty_Cycle(uint8_t duty_cycle_percentile, uint16_t* last_us, uint8_t channel);