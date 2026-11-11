#include <adc.h>

#define ADC_Prescaler (84000000/840000/2-1)
#define ADC_Tracktime 2

// Initialization function for ADC
void ADC_Init() {
    // Enable counter / PMC for ADC
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    // Configure ADC
    ADC->ADC_MR = ADC_MR_FREERUN_ON // Convert continuosly
    | ADC_MR_PRESCAL(ADC_Prescaler)  // Set prescaler
    | ADC_MR_TRACKTIM(ADC_Tracktime) // Set tracktime, according to datasheet, should be a minimum
    | ADC_MR_TRANSFER(0); // Set transfer

    // Enable channel
    ADC->ADC_CHER = ADC_CHER_CH7;

    // Start conversions
    ADC->ADC_CR = ADC_CR_START;
}

// Read ADC function
// Returns digital value of signal given to the ADC
uint16_t ADC_Read() {
    return ADC->ADC_CDR[7]; // The converted data is stored in the corresponding register to channel enabler in ADC_CHER
}