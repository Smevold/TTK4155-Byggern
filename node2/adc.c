#include <adc.h>

void ADC_Init() {
    // Enable counter / PMC for ADC
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    // CR -Reset ADC ?
    // MR - ADC mode register, find values
    // CHER - Enable channels, probably only need channel 0
    // SEQRx - Doubt we'll need, but be certain
    // IER - Enable interrupts, probably nice
    // EMR - Look into CMPMODE, rest seems uneccesary
    // CWR - Is this needed?
    // CR - Start adc again
    // Enable adc for pin 37?
}