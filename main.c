#define F_CPU 4915200UL

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)

#include "tests/sram_test.h"
#include "tests/pin_test.h"
#include "Drivers/UART_driver.h"
#include "Drivers/IO_driver.h"
#include "Drivers/SPI_driver.h"
#include "Drivers/OLED_driver.h" 
#include "Drivers/ISR_driver.h"
#include "Graphics/OLED_graphics.h"
#include "Graphics/fonts.h"
#include "Interface/Game_menu.h"


int loop = 1;
io_joy_t joy;

void main(){
    sei();
    EXTRAM_Init();
    UART_Init (BAUD_PRESCALE);
    JOY_Init(&joy);
    ADC_Init();
    SPI_Init();

    OLED_Init();
    ISR_Init();

    cursor_t cursor;

    OLED_Clear(&cursor);
    OLED_Print(OV_logo, &cursor);

    //_delay_ms(2000);

    //OLED_Clear(&cursor);
    //Interface_Startup(&cursor, &joy, &joy_digital);

    
    uint8_t menu_option = 0;
    //printf("Checkpoint\n");
    Buttons btns;
    while(1) {
        btns = BTN_Read();
        if (btns.R1 == 1) {
            LED_Set(1, 1);
        }

        printf("1: %2X, 2: %2X, 3: %2X\n", btns.right, btns.left, btns.nav);
        _delay_ms(1000);
    }
}