#pragma once

#include <stdint.h>

//#define EXT_ADC ((volatile uint8_t *) 0x1000)
#define EXT_RAM ((volatile uint8_t *) 0x1400)

typedef struct {
    uint8_t joy_x;
    uint8_t joy_y;
    uint8_t pad_x;
    uint8_t pad_y;
} io_pos_t;

void ADC_init();

io_pos_t ADC_read();