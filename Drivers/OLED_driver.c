#include "OLED_driver.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

void OLED_Init() {

    // CS active low PB3 
    // reset low init of chip high normal to button 
    // HIGH interpreted data, LOW CMD REG PB1
    DDRB |= ( 1 << DDB1)

   // PORTB &= ~(1 << PORTB1)

}

void OLED_Home(cursor_t* cursor) {
    *cursor.line = 0;
    *cursor.column = 0;

}

void OLED_Goto_Line(cursor_t* cursor, uint8_t line) {
    *cursor.line = line;
}

void OLED_Goto_Column(cursor_t* cursor, uint8_t column) {
    *cursor.column = column;
}

void OLED_clear(cursor_t* cursor, uint8_t line) {

}

void oled_pos(cursor_t* cursor, uint8_t line, uint8_t column ) {
    *cursor.line = line;
    *cursor.column = column;
}

void oled_print (char* print){

}