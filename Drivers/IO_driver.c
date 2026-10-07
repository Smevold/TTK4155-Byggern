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

// Initialise maximum and minimums of the joystick
// Some value has been chosen, approx midway between max and min
void JOY_Init(io_joy_t* joy_extreme) {
    joy_extreme->x_max = 0x80;
    joy_extreme->x_min = 0x80;
    joy_extreme->y_max = 0x80;
    joy_extreme->y_min = 0x80;
}

void ADC_Read(io_joy_t* joy, io_pad_t* pad) {
    volatile char *ext_adc = (char *) 0x1000; // Start address for the SRAM

    ext_adc[0] = 0x01;

    _delay_us(ADC_CONV_TIME_US); // Should change to polling or interrupts

    // Should automatically change address for reading
    joy->y_dig = ext_adc[0];
    joy->x_dig = ext_adc[0];
    pad->pad_y = ext_adc[0];
    pad->pad_x = ext_adc[0];
}

void JOY_Readable_X(io_joy_t* joy) {
    joy->x = (joy->x_dig - joy->x_min) * 100 / (joy->x_max - joy->x_min);
}

void JOY_Readable_Y(io_joy_t* joy) {
    joy->y = (joy->y_dig - joy->y_min) * 100 / (joy->y_max - joy->y_min);
}

void JOY_Readable_Pos(io_joy_t* joy) {
    JOY_Readable_X(joy);
    JOY_Readable_Y(joy);
}

void JOY_Direction(io_joy_t* joy) {
    // Percentile size of square area giving neutral direction
    const uint8_t neutral_size = 10;
    
    // Check if neutral
    if ((joy->x < 50 + neutral_size && joy->x > 50 - neutral_size)
        && (joy->y < 50 + neutral_size && joy->y > 50 - neutral_size)) {
            joy->dir = NEUTRAL;
            return;
    }

    // Determine direction of joystick
    // Logic is: think of position in a coordinate, divide sections with diagonals, setup expressions, voila
    if (joy->y > joy->x) {
        if (joy->y > 100 - joy->x) {
            joy->dir = UP;
        } else {
            joy->dir = LEFT;
        }
    } else {
        if (joy->y > 100 - joy->x) {
            joy->dir = RIGHT;
        } else {
            joy->dir = DOWN;
        }
    }
}

void JOY_Calibrate(io_joy_t* joy) {
    
    // Compare and change in x direction
    if (joy->x_dig < joy->x_min) {
        joy->x_min = joy->x_dig;
    } else if (joy->x_dig > joy->x_max) {
        joy->x_max = joy->x_dig;
    }

    // Compare and change in y direction
    if (joy->y_dig < joy->y_min) {
        joy->y_min = joy->y_dig;
    } else if (joy->y_dig > joy->y_max) {
        joy->y_max = joy->y_dig;
    }
}

void BTN_Read(){

    SPI_SlaveSelect(SS_IO_AVR);
    SPI_Transmit(0x04);
    
    _delay_us(40);

    //Buttons btns = {0};
    //Buttons btns;
    uint8_t btns_in[3];  //(uint8_t *)&btns; //  0x1000 kanskje? 

    btns_in[0] = SPI_Receive();
    _delay_us(2);
    btns_in[1] = SPI_Receive();
    _delay_us(2);
    btns_in[2] = SPI_Receive();

    printf("1: %2X, 2: %2X, 3: %2X\n", btns_in[0], btns_in[1], btns_in[2]);

}

void LED_Set(uint8_t led_n, uint8_t on){

    SPI_SlaveSelect(SS_IO_AVR);
    
    SPI_Transmit(0x05);
    _delay_us(40);

    SPI_Transmit(led_n); 
    SPI_Transmit(on); // off = 0 on = everything else
    _delay_us(2);
}

