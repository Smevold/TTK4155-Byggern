#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sam.h"
#include "motor.h"

typedef struct motor_t motor_t;

typedef struct pid_ctrl_t{
    float error_sum;
    float error_prev;
    float output; 
} pid_ctrl_t;

// PID controller used for the motor
void PID_Controller(pid_ctrl_t* pid, motor_t* motor);

// update p_ref value wih hyseisis and max min values
void PID_Update_Ref(motor_t* motor, io_joy_t* joy);