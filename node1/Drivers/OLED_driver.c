#include "OLED_driver.h"

#define F_CPU 4915200UL
#include<util/delay.h>

// Initialize OLED
void OLED_Init() {

    // CS active low PB3 
    // reset low init of chip high normal to button 
    // HIGH interpreted data, LOW CMD REG PB1
    DDRB |= ( 1 << DDB1) | ( 1 << DDB0);

    PORTB &= ~(1 << PORTB0);
    _delay_us(5);
    PORTB |= (1 << PORTB0);
    _delay_us(3);
    // select OLED
    SPI_SlaveSelect(SS_OLED);
    SPI_Transmit(0xAF); // Display ON

    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x20); // Set Memory Addressing Mode
    SPI_Transmit(0b00); // Set Horizontal Addressing Mode
}

// Move cursor on OLED to home / upper left corner
void OLED_Home(cursor_t* cursor) {
    cursor->line_start = 0;
    cursor->line_end = 63;
    cursor->column_start = 0;
    cursor->column_end = 127;
    OLED_Goto_Pos(cursor);
}

// Movo cursor on OLED to page given in cursor
void OLED_Goto_Page(cursor_t* cursor) {
    SPI_SlaveSelect(SS_OLED);
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x22); // Set Page Address
    SPI_Transmit((cursor->line_start >> 3)); // Set Page Start Address
    SPI_Transmit((cursor->line_end >> 3)); // Set Page End Address
}

// Movo cursor on OLED to column given in cursor
void OLED_Goto_Column(cursor_t* cursor) {
    SPI_SlaveSelect(SS_OLED);
    // Set command-mode
    PORTB &= ~(1 << PORTB1);
    SPI_Transmit(0x21); // Set Column Address
    SPI_Transmit(cursor->column_start); // Set Column Start Address
    SPI_Transmit(cursor->column_end); // Set Column End Address
}

// Move cursor on OLED to position given in cursor
void OLED_Goto_Pos(cursor_t* cursor) {
    OLED_Goto_Column(cursor);
    OLED_Goto_Page(cursor);
}

// Clear entire OLED
void OLED_Clear(cursor_t* cursor) {
    OLED_Home(cursor);
    SPI_SlaveSelect(SS_OLED);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int j = 0; j < 8; j++) { // Clear all Pages
        for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0x00);
        }
    }
}

// Fill entire OLED
void OLED_Fill(cursor_t* cursor) {
    OLED_Home(cursor);
    SPI_SlaveSelect(SS_OLED);

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int j = 0; j < 8; j++) { // Fill all Pages
        for (int i = 0; i < 128; i++) { // Fill all COL
            SPI_Transmit(0xFF);
        }
    }
}

//  Clear line given by cursor on OLED
// Will always clear line_start
void OLED_Clear_Line(cursor_t* cursor) {
    OLED_Goto_Pos(cursor); // Sets SPI_Slave

    // Change to data-mode
    PORTB |= (1 << PORTB1); 
    for (int i = 0; i < 128; i++) { // Clear all COL
            SPI_Transmit(0x00 + (1 << ((cursor->line_start % 8) - 1)));
    }


}

// Print figure given by bitmap at position given by cursor on OLED
void OLED_Print(const uint8_t (*bitmap)[128], cursor_t* cursor){
    OLED_Goto_Pos(cursor); // Sets SPI_Slave

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

// Print character given by character, in font given by font, at position given by cursor, on OLED
void OLED_Print_Char(const uint8_t* font, char character, uint8_t size){
    SPI_SlaveSelect(SS_OLED);
    // Change to data-mode
    PORTB |= (1 << PORTB1); 

    // Find index of character in font table
    // Table is a ASCII table with a 32 bit offset
    uint8_t index = (uint8_t)character - 32;

    // Find adress of character in font, by taking in the adress of the beginning of the font
    // and offsetting the adress with the index multiplied by the size of the characters
    const uint8_t *glyph = font + (uint16_t)index * size;

    // Print character
    char byte = 0x00;
    for (int j = 0; j < size; j++){
            byte = pgm_read_byte(&glyph[j]);
            SPI_Transmit(byte);
    }
}

// Print string given by list of characters str, in font given by font, at position given by cursor, on OLED
void OLED_Print_Str(const uint8_t* font, char str[], uint8_t size) {
    // Finds number of characters in str
    uint8_t str_length = strlen(str);
    char letter;

    // Iterate through all characters, and call OLED_Print_Char to print them
    for (int i = 0; i < str_length; i++) {
        char letter = str[i];
        printf("%s", letter);
        OLED_Print_Char(font, letter, size);
    }
}