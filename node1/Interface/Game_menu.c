#include "../Drivers/OLED_driver.h"
#include "../Drivers/SPI_driver.h"
#include "../Drivers/IO_driver.h"
#include "../Drivers/ISR_driver.h"
#include "../Graphics/OLED_graphics.h"
#include "../Graphics/fonts.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#include <string.h>
#include "time.h" // SHOULD SWITCH TO TIMER INTERRUPT

#define F_CPU 4915200UL
#include<util/delay.h>
#include "Game_menu.h"

void Interface_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad) {
    OLED_Clear(cursor);
    OLED_Home(cursor);
    char str_calibrating[] = "Calibrating...";
    OLED_Print_Str(font5, str_calibrating, MEDIUM_FONT);

    cursor->line_start = 8;
    OLED_Goto_Pos(cursor);
    char str_holdright[] = "Spin joystick now!";
    OLED_Print_Str(font5, str_holdright, MEDIUM_FONT);

    // Reset timer
    TCNT3 = 0x0000;
    loop = 1;
    while(loop) {
        ADC_Read(joy, pad);
        JOY_Calibrate(joy);
    }

    // Reset clock
    // TCNT0 = 0x00;

    OLED_Clear_Line(cursor);
    char str_goodwork[] = "Good Work, August!";
    OLED_Print_Str(font5, str_goodwork, MEDIUM_FONT);
    
}

void Interface_Print_Menu(cursor_t* cursor) {
    OLED_Home(cursor);

    cursor->column_start = 8;
    OLED_Goto_Pos(cursor);
    char line1[] = "Option 1";
    OLED_Print_Str(font5, line1, MEDIUM_FONT);

    cursor->line_start = 8;
    OLED_Goto_Pos(cursor);

    char line2[] = "Option 2";
    OLED_Print_Str(font5, line2, MEDIUM_FONT);

    cursor->line_start = 16;
    OLED_Goto_Pos(cursor);

    char line3[] = "Option C";
    OLED_Print_Str(font5, line3, MEDIUM_FONT);
} 

void Interface_Menu_Nav(io_joy_t* joy, cursor_t* cursor, uint8_t* menu_option) {
        if (joy->dir == DOWN && *menu_option > 0) {
            *menu_option--;
        } else if (joy->dir == UP && *menu_option < 3) {
            *menu_option++;
        }

        cursor->column_start = 0;
        OLED_Goto_Column(&cursor);
        OLED_Print_Char(font5, ' ', MEDIUM_FONT);
        switch (*menu_option) {
            case 0:
                cursor->line_start = 0;
                break;
            case 1:
                cursor->line_start = 8;
                break;
            case 2:
                cursor->line_start = 16;
                break;
            }
            
            
        cursor->column_start = 0;
        OLED_Goto_Pos(&cursor);
        
        // Joy btn is connected to pin D 4
        joy->btn = PIND & (1 << PIND4);
        if (joy->btn) {
            OLED_Print_Char(font5, 'X', MEDIUM_FONT);
        } else {
            OLED_Print_Char(font5, 'O', MEDIUM_FONT);
        }
}