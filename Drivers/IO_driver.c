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

void ADC_init() {
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

io_pos_t ADC_read() {
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

