#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Drivers/UART_driver.h"
#include "pin_test.h"

void pin_test(){
    volatile uint8_t *ram = (volatile uint8_t *)0x1800;

    ram[0x00] = 0x01;   // address 0x1800 
    ram[0x01] = 0x02;   // address 0x1801 A0

    printf("00: %02X A0: %02X\n", ram[0x00], ram[0x01]);

    ram[0x00] = 0x11; // 
    ram[0x02] = 0x22; // A1

    printf("00: %02X A1: %02X\n", ram[0x00], ram[0x02]);

    ram[0x04] = 0x33; // A2
    ram[0x08] = 0x44; // A3

    printf("A2: %02X A3: %02X\n", ram[0x04], ram[0x08]);

    ram[0x10] = 0x55; // A4
    ram[0x20] = 0x66; // A5
    
    printf("A4: %02X A5: %02X\n", ram[0x10], ram[0x20]);
    
    ram[0x40] = 0x77; // A6
    ram[0x80] = 0x88; // A7

    printf("A6: %02X A7: %02X\n", ram[0x40], ram[0x80]);

    ram[0x100] = 0xA1;  // A8
    ram[0x200] = 0xA2;  // A9

    printf("A8: %02X A9: %02X\n", ram[0x100], ram[0x200]);

    ram[0x400] = 0xA3;  // A10
    ram[0x800] = 0xA4;  // A11

    printf("A10: %02X A11: %02X\n", ram[0x400], ram[0x800]);
}