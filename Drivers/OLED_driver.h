#pragma once

void OLED_Init() ;

void OLED_Home(cursor_t* cursor) ;

void OLED_Goto_Line(cursor_t* cursor, uint8_t line); 

void OLED_Goto_Column(cursor_t* cursor, uint8_t column); 

void OLED_clear(cursor_t* cursor, uint8_t line) ;

void oled_pos(cursor_t* cursor, uint8_t line, uint8_t column ) ;

void oled_print (char* print) ;

char print;

typedef struct {
    uint8_t column;
    uint8_t line;

} cursor_t;
