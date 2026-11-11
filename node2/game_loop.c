#include "game_loop.h"

void SM_Game_Running(CanMsg* msg_rx, CanMsg* msg_tx, io_joy_t* joy, motor_t* motor, pid_ctrl_t* pid, uint32_t* can_last_tick, uint32_t* last_us_motor, uint8_t* state){
    while (1) {
        if (!IR_result()){
            *state = 0;
            can_rx(msg_rx);
            msg_tx->length = 1;
            msg_tx->byte[0] = 0x0A;
            can_tx(*msg_tx);
            break;
        }
        if(can_rx(msg_rx)) {
            joy->x_max = msg_rx->byte[0];
            joy->x_min = msg_rx->byte[1];
            joy->y_max = msg_rx->byte[2];
            joy->y_min = msg_rx->byte[3];
            joy->x_dig = msg_rx->byte[4];
            joy->y_dig = msg_rx->byte[5];
            joy->btn = msg_rx->byte[6];

            //printf("--- NEW MESSAGE ---\n");
            //printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n\r", msg_rx.id, msg_rx.length, joy.x_max, joy.x_min, joy.y_max, joy.y_min, joy.x_dig, joy.y_dig, joy.btn);   
            //printf("msg_send: ID: %2X, DLC: %2X, Data: %2X\n\r", msg_tx.id, msg_tx.length, msg_tx.byte[0]);
            
            JOY_Readable_Pos(joy);
            PID_Update_Ref(motor, joy);
        }

        // 1/4 delay as node1
        if(ticks - *can_last_tick >= 2) {
            msg_tx->length = 1;
            msg_tx->byte[0] = 0x01;
            can_tx(*msg_tx);
            *can_last_tick = ticks;
        }

        if (!joy->btn) {
            Solenoid_Hit();
        }
        
        if (pid_tick == 1) {
            pid_tick = 0;
            // READ MOTOR POS
            Motor_Get_Position(motor);
            PID_Controller(pid, motor);
            Motor(pid, last_us_motor);// SET MOTOR SPEED
        }
        /*
        if(ticks - last_print_tick >= 100) {
            printf("adc: %d, ref: %d, meas: %d, out: %.4f\n\r", adc, motor.p_ref, motor.p_measured, pid.output);
            last_print_tick = ticks;
        }
        */
    }
}