#include <adc.h>

#define ADC_Prescaler 127

void ADC_Init() {
    
    // ADC mode register, find values for prescaler, think rest is fine
    ADC->ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(ADC_Prescaler);

    // Enable channels, probably only need channel 0
    ADC->ADC_CHER = ADC_CHER_CH0;
    
    // Enable counter / PMC for ADC
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    // Start conversions
    ADC->ADC_CR = ADC_CR_START;

}