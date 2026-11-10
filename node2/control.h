#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sam.h"
#include "motor.h"

typedef struct{
    float error_sum;
    float error_prev;
    float output; 
} pid_ctrl_t;

void PID_Controller(pid_ctrl_t* pid, motor_t* motor);