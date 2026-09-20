#pragma once

typedef struct {
    uint8_t joy_x;
    uint8_t joy_y;
    uint8_t pad_x;
    uint8_t pad_y;
} adc_reading_t;

void adc_init();