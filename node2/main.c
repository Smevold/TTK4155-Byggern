#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
------------- TODO --------------
- Cleanup
- Add switch state in main

- Startup-state
    - Send startup finished over CAN

- Menu-state
    - Change menu-options
    - Make menu-options change to correct state

- Running-state
    - Update servo, motor and servo continuously
    - Send solenoid signal from USER IO BOARD
    - Send signal to node1, when game ends
    - ON GAME END:
        - SAVE SCORE (GAME_START_TICK - GAME_END_TICK)

- Game-over-state
    - Display score
    - Add menu-options
    - (Optional) tune controller better, game should be playable

- Exit
    - Break out of game loop?

*/



/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "uart.h"
#include "can.h"
#include "interrupt.h"
#include "io_driver.h"
#include "control.h"
#include "motor.h"
#include "solenoid.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(84000000, 9600);

    // Can initlialize struct
    CanInit init;
    init.phase2 = 6;
    init.propag = 0;
    init.phase1 = 6;
    init.sjw = 0;
    init.brp = 41; // brp provided by Claude, because we misunderstood
    // BRP+1 = MCK / (bitrate * 16) = 84e6 / (125e3 * 16) = 42
    init.smp = 1;

    can_init(init, 0);
    
    CanMsg msg_tx; 
    CanMsg msg_rx;

    // Send a msg back
    msg_tx.id = 0x04; 
    msg_tx.length = 1;
    msg_tx.byte[0] = 0x14;

    
    io_joy_t joy;
    
    Timer_Init();

    PWM_Init();
    ADC_Init();

    Motor_Init();
    Solenoid_Init();


    printf("Hello!\n");

    uint32_t last_tick = 0;
    uint32_t last_print_tick = 0;
    uint16_t last_us_servo = 0;
    uint16_t last_us_motor = 0;
    uint16_t solenoid_ticks = 0;
    pid_ctrl_t pid = {0}; // Sets all members to zero
    motor_t motor = {0}; // Sets all members to zero
    motor.p_ref = 0x40;

    uint16_t adc = 0;

    // Startup wait
    while(startupnotfinished) {;}

    while (1)
    {


        // --- CAN JOY TO PWM TEST BEGIN ---
        

        // Only update tx and send, if received
        if(can_rx(&msg_rx)) {

            joy.x_max = msg_rx.byte[0];
            joy.x_min = msg_rx.byte[1];
            joy.y_max = msg_rx.byte[2];
            joy.y_min = msg_rx.byte[3];
            joy.x_dig = msg_rx.byte[4];
            joy.y_dig = msg_rx.byte[5];
            joy.btn = msg_rx.byte[6];
 
            //printf("--- NEW MESSAGE ---\n");
            //printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_rx.id, msg_rx.length, joy.x_max, joy.x_min, joy.y_max, joy.y_min, joy.x_dig, joy.y_dig, joy.btn);   
            //printf("msg_send: ID: %2X, DLC: %2X, Data: %2X\n", msg_tx.id, msg_tx.length, msg_tx.byte[0]);
            
            JOY_Readable_Pos(&joy);

            PID_Update_Ref(&motor, &joy);
        }
        
        adc = ADC_Read();

        // 1/4 delay as node1
        if(ticks - last_tick >= 2) {
            can_tx(msg_tx);
            last_tick = ticks;
            //printf("%2X\n", adc);

            //uint32_t l = ADC->ADC_LCDR;
            //printf("ch:%lu val:%lu\n", (l >> 12) & 0xF, l & 0xFFF);
            //printf("CHSR:%lX MR:%lX\n", ADC->ADC_CHSR, ADC->ADC_MR);
            //printf("COR:%lX CGR:%lX EMR:%lX\n", ADC->ADC_COR, ADC->ADC_CGR, ADC->ADC_EMR);

            // ADC CHIP TEMP CHECK --- REMOVE LATER
            // after a moment:
            //printf("%x\n", ADC->ADC_CDR[7]);

            //printf("Before last tick\n");
            //printf("Before send\n");

            //Solenoid_Hit(); 

            //printf("After send\n");

        }
        if (ticks - solenoid_ticks >= 500) {
            solenoid_ticks = ticks;
            Solenoid_Hit();
        }


        // --- CAN JOY TO PWM TEST END ---

        
        // --- PID Controller Call BEGIN ---
        
        if (pid_tick == 1) {
            pid_tick = 0;
            // READ MOTOR POS
            Motor_Get_Position(&motor);
            PID_Controller(&pid, &motor);
            Motor(&pid, &last_us_motor);// SET MOTOR SPEED
        }
        
        if(ticks - last_print_tick >= 100) {
            printf("adc: %d, ref: %d, meas: %d, out: %.4f\n", adc, motor.p_ref, motor.p_measured, pid.output);
            last_print_tick = ticks;
        }

        // --- PID Controller Call END ---
        
       
    }    
}