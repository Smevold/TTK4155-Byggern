#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>

#define F_CPU 4915200UL
#include<util/delay.h>

#include "UART_driver.h"
#include "IO_driver.h"
#include "SPI_driver.h"

#define ADC_CHANNEL_NUM 4
#define ADC_CLK_HZ 4000000UL // May be subject to change
#define ADC_CONV_TIME_US ((9UL * ADC_CHANNEL_NUM * 2UL * 1000000UL) / ADC_CLK_HZ + 5) // Should change to polling or interrupts



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

void JOY_Init(io_joy_t* joy_extreme) {
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

void JOY_Readable_X(io_joy_t* joy, uint8_t* x_digital) {
    joy->x = (*x_digital - joy->x_min) * 100 / (joy->x_max - joy->x_min);
}

void JOY_Readable_Y(io_joy_t* joy, uint8_t* y_digital) {
    joy->y = (*y_digital - joy->y_min) * 100 / (joy->y_max - joy->y_min);
}

void JOY_Readable_Pos(io_joy_t* joy, io_pos_t* joy_digital) {
    JOY_Readable_X(joy, &(joy_digital->joy_x));
    JOY_Readable_Y(joy, &(joy_digital->joy_y));
}

void JOY_Direction(io_joy_t* joy) {
    // Percentile size of square area giving neutral direction
    const uint8_t neutral_size = 10;
    
    // Check if neutral
    if ((joy->x < 50 + neutral_size && joy->x > 50 - neutral_size)
        && (joy->y < 50 + neutral_size && joy->y > 50 - neutral_size)) {
            joy->dir = NEUTRAL;
    }

    // Check which quadrant the joystick is in
    // After given quadrant is determined, logic to determine direction
    if (joy->x > 50) {
        if (joy->y > 50) {
            if (joy->x > joy->y) {
                joy->dir = RIGHT;
            } else {
                joy->dir =  UP;
            }
        } else {
            if (joy->y < 100 - joy->x) {
                joy->dir =  DOWN;
            } else {
                joy->dir =  RIGHT;
            }
        }
    } else {
        if (joy->y < 50) {
            if (joy->x < joy->y) {
                joy->dir =  LEFT;
            } else {
                joy->dir =  DOWN;
            }
        } else {
            if (joy->y > 100 - joy->x) {
                joy->dir =  UP;
            } else {
                joy->dir =  LEFT;
            }
        }
    }
}

void JOY_Calibrate(io_joy_t* joy, io_pos_t* joy_digital) {
    
    // Compare and change in x direction
    if (joy_digital->joy_x < joy->x_min) {
        joy->x_min = joy_digital->joy_x;
    } else if (joy_digital->joy_x > joy->x_max) {
        joy->x_max = joy_digital->joy_x;
    }

    // Compare and change in y direction
    if (joy_digital->joy_y < joy->y_min) {
        joy->y_min = joy_digital->joy_y;
    } else if (joy_digital->joy_y > joy->y_max) {
        joy->y_max = joy_digital->joy_y;
    }
}

void Read_Buttons(){

    SPI_SlaveSelect(SS_IO_AVR);
    SPI_Transmit(0x04);
    
    _delay_us(40);

    Buttons btns = {0};
    //Buttons btns;
    uint8_t *btns_in = (uint8_t *)&btns; //  0x1000 kanskje? 

    btns_in[0] = SPI_Receive(0x00);
    _delay_us(2);
    btns_in[1] = SPI_Receive(0x00);
    _delay_us(2);
    btns_in[2] = SPI_Receive(0x00);

}

void set_led(uint8_t led_n, uint8_t on){

    SPI_SlaveSelect(SS_IO_AVR);
    
    SPI_Transmit(0x05);
    _delay_us(40);

    SPI_Transmit(led_n); 
    SPI_Transmit(on); // off = 0 on = everything else
    _delay_us(2);
}

