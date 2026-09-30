#include "OLED_driver.h"
#include "SPI_driver.h"
#include "../Graphics/OLED_graphics.c"

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
    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(0); // Set Column Start Address
    SPI_Transmit(127); // Set Column End Address

    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit(0); // Set Page Start Address
    SPI_Transmit(7); // Set Page End Address
}

void OLED_Goto_Line(cursor_t* cursor) {
    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit(cursor->line_start >> 3); // Set Page Start Address
    SPI_Transmit(cursor->line_end >> 3); // Set Page End Address
}

void OLED_Goto_Column(cursor_t* cursor) {
    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(cursor->column_start); // Set Column Start Address
    SPI_Transmit(cursor->column_end); // Set Column End Address
}

OLED_Goto_Pos(cursor_t* cursor) {
    OLED_Goto_Column(&cursor);
    OLED_Goto_Line(&cursor);
}

void OLED_Clear() {
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    

    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(0); // Set Column Start Address
    SPI_Transmit(127); // Set Column End Address

    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit(0); // Set Page Start Address
    SPI_Transmit(7); // Set Page End Address

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int j = 0; j < 8; j++) { // Clear all Pages
        for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0x00);
        }
    }

}

void OLED_Draw_OV() {
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    

    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(0); // Set Column Start Address
    SPI_Transmit(127); // Set Column End Address

    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit(0); // Set Page Start Address
    SPI_Transmit(7); // Set Page End Address

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    char byte = 0x00;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 128; j++){
            byte = pgm_read_byte(&(OV_logo[i][j]));
            SPI_Transmit(byte);
        }
    }
}

OLED_Clear_Line(cursor_t* cursor, uint8_t line) {

}

void OLED_Print (char* print){

}