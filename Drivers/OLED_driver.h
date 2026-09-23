#pragma once

#include <stdint.h>

typedef struct {
    uint8_t column;
    uint8_t line;
} cursor_t;

void OLED_Init() ;

void OLED_Home(cursor_t* cursor) ;

void OLED_Goto_Line(cursor_t* cursor, uint8_t line); 

void OLED_Goto_Column(cursor_t* cursor, uint8_t column); 

void OLED_Clear(cursor_t* cursor, uint8_t line) ;

void OLED_Pos(cursor_t* cursor, uint8_t line, uint8_t column ) ;

//void OLED_Print (char* print) ;

//char print;

