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

void OLED_Goto_Page(cursor_t* cursor); 

void OLED_Goto_Column(cursor_t* cursor); 

void OLED_Goto_Pos(cursor_t* cursor);

void OLED_Clear(cursor_t* cursor);

void OLED_Fill(cursor_t* cursor);

void OLED_Clear_Line(cursor_t* cursor);

//void OLED_Print (char* print) ;

//char print;

