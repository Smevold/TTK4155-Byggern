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
#include "Graphics/OLED_graphics.h"
#include "Graphics/fonts.h"
#include "Interface/Game_menu.h"

void PIN_Init(){ // Is probably only ext ram init
    MCUCR |= (1 << SRE);
    //DDRA = 0b11111111;
    //DDRA |= (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
    //DDRE |= (1 << DDE1);
    SFIOR |= (1 << XMM2);
    SFIOR &= ~((1 << XMM1) | (1 << XMM0));
}

/*
void pin_set(){
    PORTA = (1 << PA7) | (1 << PA6) | (1 << PA5) | (1 << PA4) | (1 << PA3) | (1 << PA2) | (1 << PA1) | (1 << PA0);
   // PORTE = (1 << PE1);
}
   */



void main(){
    PIN_Init();
    UART_Init (BAUD_PRESCALE);
    ADC_Init();
    SPI_Init();

    OLED_Init();

    cursor_t cursor;

    OLED_Clear(&cursor);

    /*
    io_joy_t* joy;
    io_pos_t* pos;

    *pos = ADC_Read();

    JOY_Init(&joy);
    
    printf("Test please work");
    

    
    printf("Calibrating...");
    volatile int i = 0;
    while(i < 5) {
        JOY_Calibrate(&joy, &pos);
        _delay_ms(1000);
        i++;
        printf("%2X seconds passed", i);
    }
    printf("Done calibrating !!!!!!!");

    printf("x_max: %2X, x_min: %2X, y_max: %2X, y_min: %2X", joy->x_max, joy->x_min, joy->y_max, joy->y_min);
    */

    OLED_Home(&cursor);
    //OLED_Print(OV_logo, &cursor);

    _delay_ms(3000);
    //char aye = 'a';
    //OLED_Print_Char((const uint8_t*)font8, aye, LARGE_FONT);
    char str[] = "Hello, World!";
    OLED_Print_Str(font8, str, LARGE_FONT);

    //cursor.line_start = 5;
    //OLED_Clear_Line(&cursor);

    while(1) {
        _delay_ms(20);
    }
}