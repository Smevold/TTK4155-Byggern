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

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PIOB->PIO_WPMR = 0; // Clear WPEN bit to disable write protect

    PIOB->PIO_PER |= PIO_PB13; // Enable register pin 13
    PIOB->PIO_OER |= PIO_PB13; // Output enable register pin 13
    PIOB->PIO_SODR |= PIO_PB13; // Set output data register pin 13

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    printf("Hello World\n\r");

    CanInit init;
    init.phase2 = 4;
    init.propag = 4;
    init.phase1 = 4;
    init.sjw = 4;
    init.brp = 8;
    init.smp = 8;
    printf("CanInit pahse2: %2X, propag: %2X, phase1: %2X, sjw: %2X, brp: %2X, smp: %2X", init.phase2, init.propag, init.phase1, init.sjw, init.brp, init.smp);

    can_init(init, 0);

    CanMsg m_tx;
    m_tx.id = 2;
    m_tx.length = 1;
    m_tx.byte[8] = 0x04;

    CanMsg m_rx; 

    while (1)
    {
        can_tx(m_tx);
        can_rx(&m_rx);

        printf("TX id: %2X, length: %2X, byte: %2X\n RX: id: %2X, length: %2X, byte: %2X\n", m_tx.id, m_tx.length, m_tx.byte, m_rx.id, m_rx.length, m_rx.byte);
    }
    
}