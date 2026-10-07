#include "../Drivers/OLED_driver.h"
#include "../Drivers/SPI_driver.h"
#include "../Graphics/OLED_graphics.h"
#include "../Graphics/fonts.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#include <string.h>

#define F_CPU 4915200UL
#include<util/delay.h>
#include "Game_menu.h"

void Interface_Print_Menu(cursor_t* cursor) {
    OLED_Home(cursor);

    OLED_Print_Char(font5, 'X', MEDIUM_FONT);

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