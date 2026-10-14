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


void main(){
    sei();
    EXTRAM_Init();
    UART_Init (BAUD_PRESCALE);
    JOY_Init(&joy);
    ADC_Init();
    SPI_Init();

    DDRD &= ~(1 << DDD4);

    OLED_Init();
    ISR_Init();

    cursor_t cursor;

    
    OLED_Clear(&cursor);
    OLED_Print(OV_logo, &cursor);
    
    _delay_ms(2000);
    
    Interface_Startup(&cursor, &joy, &pad);
    printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n", joy.x_max, joy.x_min, joy.y_max, joy.y_min);

    _delay_ms(2000);

    OLED_Clear(&cursor);
    Interface_Print_Menu(&cursor);

    
    uint8_t menu_option = 0;
    uint8_t btn = 1;
    //printf("Checkpoint\n");
    Buttons btns;
    while(1) {
        ADC_Read(&joy, &pad);
        JOY_Readable_Pos(&joy);
        JOY_Direction(&joy);

        if (joy.dir == DOWN && menu_option > 0) {
            menu_option--;
        } else if (joy.dir == UP && menu_option < 3) {
            menu_option++;
        }

        //OLED_Clear(&cursor);
        //Interface_Print_Menu(&cursor);
        cursor.column_start = 0;
        OLED_Goto_Column(&cursor);
        OLED_Print_Char(font5, ' ', MEDIUM_FONT);
        switch (menu_option) {
            case 0:
                cursor.line_start = 0;
                break;
            case 1:
                cursor.line_start = 8;
                break;
            case 2:
                cursor.line_start = 16;
                break;
            }
            
            
        cursor.column_start = 0;
        OLED_Goto_Pos(&cursor);
        
        btn = PIND & (1 << PIND4);
        if (btn) {
            OLED_Print_Char(font5, 'X', MEDIUM_FONT);
        } else {
            OLED_Print_Char(font5, 'O', MEDIUM_FONT);
        }
        
        btns = BTN_Read();
        if (btns.R1 == 1) {
            LED_Set(1, 1);
        } else {
            LED_Set(1,0);
        }
        
        //printf("1: %2X, 2: %2X, 3: %2X\n", btns.right, btns.left, btns.nav);
        printf("Joy dir: %2X\n", joy.dir);
        printf("x: %2X, y: %2X\n", joy.x_dig, joy.y_dig);
        printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n", joy.x_max, joy.x_min, joy.y_max, joy.y_min);
        printf("JoyBTN: %2X\n", btn);
        
        _delay_ms(100);
        
    }
}

