#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

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
//#include "control.h"

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

    printf("Test 1\n");

    uint32_t last_tick = 0;
    uint16_t last_us = 0;
    //pid_ctrl_t pid = {0}; // Sets all members to zero
    //motor_t motor = {0}; // Sets all members to zero

    uint16_t adc = 0;

    // ADC CHIP TEMP CHECK --- REMOVE LATER
    ADC->ADC_ACR |= ADC_ACR_TSON;     // enable the sensor (check macro name in component_adc.h)


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
            
            PWM_Joy_Duty_Cycle(joy.x, &last_us, 0);

        }
        
        adc = ADC_Read();

        // 1/4 delay as node1
        if(ticks - last_tick >= 25) {
            printf("%2X\n", adc);

            uint32_t l = ADC->ADC_LCDR;
            printf("ch:%lu val:%lu\n", (l >> 12) & 0xF, l & 0xFFF);
            printf("CHSR:%lX MR:%lX\n", ADC->ADC_CHSR, ADC->ADC_MR);
            printf("COR:%lX CGR:%lX EMR:%lX\n", ADC->ADC_COR, ADC->ADC_CGR, ADC->ADC_EMR);

            // ADC CHIP TEMP CHECK --- REMOVE LATER
            ADC->ADC_CHER = ADC_CHER_CH15;
            // after a moment:
            printf("%u\n", ADC->ADC_CDR[15]);

            //printf("Before last tick\n");
            last_tick = ticks;
            //printf("Before send\n");
            can_tx(msg_tx);
            //printf("After send\n");

        }







        // --- CAN JOY TO PWM TEST END ---

        /*
        // --- PID Controller Call BEGIN ---

        if (pid_tick == 1) {
            pid_tick = 0;
            motor.p_measured = 0; // READ MOTOR POS
            PID_Controller(&pid, &motor);
            // SET MOTOR SPEED
        }

        // --- PID Controller Call END ---
        */
    }    
}