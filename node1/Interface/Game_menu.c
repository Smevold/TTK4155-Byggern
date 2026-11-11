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
    char line1[] = "Start game";
    OLED_Print_Str(font5, line1, MEDIUM_FONT);

    cursor->line_start = 8;
    OLED_Goto_Pos(cursor);

    char line2[] = "Exit game";
    OLED_Print_Str(font5, line2, MEDIUM_FONT);
} 

void Interface_Print_Game_Running(cursor_t* cursor) {
    OLED_Clear(cursor);
    OLED_Home(cursor);

    char line[] = "GAME RUNNING";
    OLED_Print_Str(font8, line, LARGE_FONT);
}

void Interface_Print_Score(cursor_t* cursor, CAN_MSG* msg_rx) {
    OLED_Clear(cursor);
    OLED_Home(cursor);

    char line1[] = "Score: 0x%2X", msg_rx->Data[0];
    OLED_Print_Str(font5, line1, MEDIUM_FONT);

    cursor->column_start = 8;
    cursor->line_start = 8;
    OLED_Goto_Pos(cursor);

    char line2[] = "Restart game";
    OLED_Print_Str(font5, line2, MEDIUM_FONT);

    cursor->line_start = 16;
    OLED_Goto_Pos(cursor);

    char line2[] = "Exit game";
    OLED_Print_Str(font5, line2, MEDIUM_FONT);
}