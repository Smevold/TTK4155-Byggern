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

    // Clear WGM00 and set WGM01 for CTC
    TCCR0 |= (1 << WGM01);
    TCCR0 &= ~(1 << WGM00);

    // Set COM00 and clear COM01 for "toggle OC0 on Compare Match"
    TCCR0 |= (1 << COM00); 
    TCCR0 &= ~(1 << COM01);

    // Set clk select for clk without prescaler
    TCCR0 |= (1 << CS00);
    TCCR0 &= ~(1 << CS01) & ~(1 << CS02); 

    // Want 5 MHz clk signal according to data sheet

    // Toggles every cycle
    OCR2 = 0;
}

io_pos_t ADC_read() {
    io_pos_t pos;

    *EXT_ADC = 0x00;

    _delay_us(ADC_CONV_TIME_US); // Should change to polling or interrupts

    // Should automatically change address for reading
    pos.joy_x = *EXT_ADC;
    pos.joy_y = *EXT_ADC;
    pos.pad_x = *EXT_ADC;
    pos.pad_y = *EXT_ADC;

    return pos;
}

