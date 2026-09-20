#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "UART_driver.h"
#include "IO_driver"

void adc_init() {
    // Set PD4 as timer output for the ADC clc
    DDRD0 |= (1 << DDD0);
}