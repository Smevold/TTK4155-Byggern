#include <adc.h>

#define ADC_Prescaler 3
#define ADC_Tracktime 5

void ADC_Init() {
    // Enable counter / PMC for ADC
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    // USELESS
    //PIOB->PIO_ABSR |= PIO_PB18X1_AD11;
    //PIOB->PIO_PDR |= PIO_PB18X1_AD11;

    // ADC mode register, find values for prescaler, think rest is fine
    ADC->ADC_MR = ADC_MR_FREERUN_ON 
    | ADC_MR_PRESCAL(ADC_Prescaler) 
    | ADC_MR_STARTUP_SUT512 
    | ADC_MR_TRACKTIM(ADC_Tracktime) 
    | ADC_MR_SETTLING_AST5;

    // Enable channels, probably only need channel 11
    ADC->ADC_CHER = ADC_CHER_CH11;

    // Start conversions USELESS
    //ADC->ADC_CR = ADC_CR_START;
}

uint16_t ADC_Read() {
    return ADC->ADC_CDR[11];
}