#include "control.h"

#define SAMPLING_PERIOD 0.02 // Matches our timer-interrupt, should use that to check motor-pos

#define PID_KP 1
#define PID_KI 0.1
#define PID_KD 1

#define PID_OUT_MAX 100 // Maximum Motor Duty Cycle, should check

void PID_Controller(pid_ctrl_t* pid, motor_t* motor) {
    // Error
    float error = (float)(motor->p_ref - motor->p_measured);

    // Accumulated error over time
    pid->error_sum += error;

    // Proportional effect
    float p = PID_KP * error;

    // Integrative effect (summation)
    // Consider Integrator Windup
    // If pad is ref, can define that ref cannot go outside motor range
    float i = PID_KI * SAMPLING_PERIOD * pid->error_sum;

    // Derivative effect
    float d = (PID_KD / SAMPLING_PERIOD) * (error - pid->error_prev);

    // Save error
    pid->error_prev = error;

    // Collective output
    float u = p + i + d;
    if (u  > PID_OUT_MAX) {
        u = PID_OUT_MAX;
    } else if (u < -PID_OUT_MAX) {
        u = -PID_OUT_MAX;
    }
    pid->output = u;
}

void PID_Update_Ref(motor_t* motor, io_joy_t* joy){
    // Accumulate joy-stick speed reference (linearized around 0) for positional reference
    motor->p_ref = motor->p_ref + (joy->x - 50);

    
}