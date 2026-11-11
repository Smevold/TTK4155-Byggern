#pragma once

#include "sam.h"
#include <stdint.h>
#include "IO_driver.h"
#include "interrupt.h"
#include "can.h"
#include "motor.h"
#include "control.h"

void SM_game_running(CanMsg* msg_rx, CanMsg* msg_tx, io_joy_t* joy, motor_t* motor, pid_ctrl_t* pid, uint32_t* can_last_tick, uint32_t* last_us_motor);