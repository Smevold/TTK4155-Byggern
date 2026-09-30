#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"

#define ADC_CHANNEL_NUM 4
#define ADC_CLK_HZ 4000000UL // May be subject to change
#define ADC_CONV_TIME_US ((9UL * ADC_CHANNEL_NUM * 2UL * 1000000UL) / ADC_CLK_HZ + 5) // Should change to polling or interrupts

io_joy_extreme_t* joy_extreme;

void ADC_Init() {
    // Set PD5 as timer output for the ADC clc
    DDRD |= (1 << DDD5);

    // Clear WGM13, WGM11 and WGM10, and set WGM12 for CTC
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
    TCCR1A &= ~(1 << WGM11) & ~(1 << WGM10);

    // Set COM00 and clear COM01 for "toggle OC0 on Compare Match"
    TCCR1A |= (1 << COM1A0); 
    TCCR1A &= ~(1 << COM1A1);

    // Set clk select for clk without prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B &= ~(1 << CS12) & ~(1 << CS11); 

    // Want 5 MHz clk signal according to data sheet

    // Toggles every cycle
    OCR1AL = 0;
    OCR1AH = 0;
}

void Joy_Init() {
    joy_extreme->x_max = 0x80;
    joy_extreme->x_min = 0x80;
    joy_extreme->y_max = 0x80;
    joy_extreme->y_min = 0x80;
}


io_pos_t ADC_Read() {
    io_pos_t pos;
    volatile char *ext_adc = (char *) 0x1000; // Start address for the SRAM

    ext_adc[0] = 0x01;

    _delay_us(ADC_CONV_TIME_US); // Should change to polling or interrupts

    // Should automatically change address for reading
    pos.joy_y = ext_adc[0];
    pos.joy_x = ext_adc[0];
    pos.pad_y = ext_adc[0];
    pos.pad_x = ext_adc[0];

    return pos;
}

uint8_t Joy_Readable_X(uint8_t joy_x) {
    uint8_t joy_readable_x = (joy_x - joy_extreme->x_min) * 100 / (joy_extreme->x_max - joy_extreme->x_min);

    return joy_readable_x;
}

uint8_t Joy_Readable_Y(uint8_t joy_y) {
    uint8_t joy_readable_y = (joy_y - joy_extreme->y_min) * 100 / (joy_extreme->y_max - joy_extreme->y_min);

    return joy_readable_y;
}

joy_readable Joy_Readable_Pos() {
    joy_readable joy_pos;
    io_pos_t pos = ADC_Read();

    joy_pos.x = Joy_Readable_X(pos.joy_x);
    joy_pos.y = Joy_Readable_Y(pos.joy_y); 

    return joy_pos;
}

direction Joy_Direction() {
    joy_readable joy_pos = Joy_Readable_Pos();

    // Percentile size of square area giving neutral direction
    uint8_t neutral_size = 10;

    // Check if neutral
    if ((joy_pos.x < 50 + neutral_size || joy_pos.x < 50 - neutral_size)
        && (joy_pos.x < 50 + neutral_size || joy_pos.x < 50 - neutral_size)) {
            return NEUTRAL;
    }

    // Check which quadrant the joystick is in
    // After given quadrant is determined, logic to determine direction
    if (joy_pos.x > 50) {
        if (joy_pos.y > 50) {
            if (joy_pos.x > joy_pos.y) {
                return RIGHT;
            } else {
                return UP;
            }
        } else {
            if (joy_pos.y < 100 - joy_pos.x) {
                return DOWN;
            } else {
                return RIGHT;
            }
        }
    } else {
        if (joy_pos.y < 50) {
            if (joy_pos.x < joy_pos.y) {
                return LEFT;
            } else {
                return DOWN;
            }
        } else {
            if (joy_pos.y > 100 - joy_pos.x) {
                return UP;
            } else {
                return LEFT;
            }
        }
    }
}

void Calibrate_Joy() {
    io_pos_t pos = ADC_Read();

    if (pos.joy_x < joy_extreme->x_min) {
        joy_extreme->x_min = pos.joy_x;
    } else if (pos.joy_x > joy_extreme->x_max) {
        joy_extreme->x_max = pos.joy_x;
    }

    if (pos.joy_y < joy_extreme->y_min) {
        joy_extreme->y_min = pos.joy_y;
    } else if (pos.joy_y > joy_extreme->y_max) {
        joy_extreme->y_max = pos.joy_y;
    }
}