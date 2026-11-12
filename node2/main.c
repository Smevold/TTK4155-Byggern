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
#include "game_loop.h"

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


    printf("Hello!\n\r");

    uint32_t can_last_tick = 0;
    uint32_t last_print_tick = 0;
    uint16_t last_us_servo = 0;
    uint16_t last_us_motor = 0;
    uint16_t solenoid_ticks = 0;
    uint32_t game_start_ticks = 0;
    uint32_t game_end_ticks = 0;
    pid_ctrl_t pid = {0}; // Sets all members to zero
    motor_t motor = {0}; // Sets all members to zero
    motor.p_ref = 0x40;

    uint16_t adc = 0;

    uint8_t startup_wait = 1;
    uint8_t state = 0;
    uint8_t score = 0;

    
    
    while (1) {
        switch(state) {
            case 0:
                // Startup wait
                if(can_rx(&msg_rx)) {
                    printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X\n\r", msg_rx.id, msg_rx.length, msg_rx.byte[0]);   
                    // Change to game state running when given signal from node 1
                    state = 1;
                }
            
            case 1:
                game_start_ticks = ticks;
                SM_Game_Running(&msg_rx, &msg_tx, &joy, &motor, &pid, &can_last_tick, &last_us_motor, &state, &last_us_servo);
                game_end_ticks = ticks;
                score = (game_end_ticks - game_start_ticks) / 25;
                can_rx(&msg_rx);
                msg_tx.length = 1;
                msg_tx.byte[0] = score;
                can_tx(msg_tx); 
        }
    }
}  
