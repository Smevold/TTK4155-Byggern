#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct {
    uint8_t column_start;
    uint8_t column_end;
    uint8_t line_start;
    uint8_t line_end;
} cursor_t;

void OLED_Init() ;

void OLED_Home(cursor_t* cursor) ;

void OLED_Goto_Line(cursor_t* cursor, uint8_t line); 

void OLED_Goto_Column(cursor_t* cursor, uint8_t column); 

void OLED_Goto_Pos(cursor_t* cursor);

void OLED_Clear();

void OLED_Clear_Line(cursor_t* cursor, uint8_t line);

void OLED_Draw_OV();

const uint8_t logo_128x64_h[8][128] PROGMEM;

//void OLED_Print (char* print) ;

//char print;

