#include "OLED_driver.h"
#include "SPI_driver.h"
#include "../Graphics/OLED_graphics.h"
#include "../Graphics/fonts.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

void OLED_Init() {

    // CS active low PB3 
    // reset low init of chip high normal to button 
    // HIGH interpreted data, LOW CMD REG PB1
    DDRB |= ( 1 << DDB1) | ( 1 << DDB0);

    PORTB &= ~(1 << PORTB0);
    _delay_us(5);
    PORTB |= (1 << PORTB0);
    _delay_us(3);
    SPI_SlaveSelect(SS_OLED);
    SPI_Transmit(0xAF); // Display ON

    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x20); // Set Memory Addressing Mode
    SPI_Transmit(0b00); // Set Horizontal Addressing Mode

}

void OLED_Home(cursor_t* cursor) {
    cursor->line_start = 0;
    cursor->line_end = 63;
    cursor->column_start = 0;
    cursor->column_end = 127;
    OLED_Goto_Pos(cursor);
}

void OLED_Goto_Page(cursor_t* cursor) {
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit((cursor->line_start >> 3)); // Set Page Start Address
    SPI_Transmit((cursor->line_end >> 3)); // Set Page End Address
}

void OLED_Goto_Column(cursor_t* cursor) {
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(cursor->column_start); // Set Column Start Address
    SPI_Transmit(cursor->column_end); // Set Column End Address
}

void OLED_Goto_Pos(cursor_t* cursor) {
    OLED_Goto_Column(cursor);
    OLED_Goto_Page(cursor);
}

void OLED_Clear(cursor_t* cursor) {
    OLED_Home(cursor);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int j = 0; j < 8; j++) { // Clear all Pages
        for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0x00);
        }
    }
}

void OLED_Fill(cursor_t* cursor) {
    OLED_Home(cursor);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int j = 0; j < 8; j++) { // Clear all Pages
        for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0xFF);
        }
    }
}

// Will always clear line_start
void OLED_Clear_Line(cursor_t* cursor) {
    OLED_Goto_Pos(cursor);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0x00 + (1 << ((cursor->line_start % 8) - 1)));
    }


}


void OLED_Print (const uint8_t (*bitmap)[128], cursor_t* cursor){
    OLED_Goto_Pos(cursor);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    char byte = 0x00;
    for (int i = 0; i <= (cursor->line_end >> 3); i++) {
        for (int j = 0; j <= cursor->column_end; j++){
            byte = pgm_read_byte(&(bitmap[i][j]));
            SPI_Transmit(byte);
        }
    }
}

void OLED_Print_Char (uint8_t** font, char* character, uint8_t size){
    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    uint8_t index_character = (int)*character - 32;

    char byte = 0x00;
    for (int j = 0; j < size; j++){
            byte = pgm_read_byte(&(font[index_character][j]));
            SPI_Transmit(byte);
    }
}

