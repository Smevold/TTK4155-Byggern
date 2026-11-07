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
    
    io_joy_t joy;
    
    Timer_Init();

    PWM_Init();

    while (1)
    {
        // Only update tx and send, if received
        if(can_rx(&msg_rx)) {

        // Send same msg back
        msg_tx.id = msg_rx.id; 
        msg_tx.length = msg_rx.length; 
        msg_tx.byte[0] = msg_rx.byte[0]; // Set value first byte
        msg_tx.byte[1] = msg_rx.byte[1];
        msg_tx.byte[2] = msg_rx.byte[2];
        msg_tx.byte[3] = msg_rx.byte[3];
        msg_tx.byte[4] = msg_rx.byte[4];
        msg_tx.byte[5] = msg_rx.byte[5];
        msg_tx.byte[6] = msg_rx.byte[6];
        can_tx(msg_tx);

        printf("--- NEW MESSAGE ---\n");
        printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_rx.id, msg_rx.length, msg_tx.byte[0], msg_tx.byte[1], msg_tx.byte[2], msg_tx.byte[3], msg_tx.byte[4], msg_tx.byte[5], msg_tx.byte[6]);
        printf("msg_send: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_tx.id, msg_tx.length, msg_tx.byte[0], msg_tx.byte[1], msg_tx.byte[2], msg_tx.byte[3], msg_tx.byte[4], msg_tx.byte[5], msg_tx.byte[6]);
        }

    }
    
}