#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "UART_driver.h"
#include "IO_driver"

void adc_init() {
    // Set PD5 as timer output for the ADC clc
    DDRD |= (1 << DDD5);

    // Clear WGM00 and set WGM01 for CTC
    TCCR0 |= (1 << WGM01);
    TCCR0 &= ~(1 << WGM01);

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