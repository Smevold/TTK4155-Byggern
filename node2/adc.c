#include <adc.h>

#define ADC_Prescaler (84000000/840000/2-1)
#define ADC_Tracktime 2

void ADC_Init() {
    // Enable counter / PMC for ADC
    PMC->PMC_PCER1 = PMC_PCER1_PID37;

    // USELESS
    //PIOB->PIO_ABSR |= PIO_PB18X1_AD11;
    //PIOB->PIO_PDR |= PIO_PB18X1_AD11;


    //ADC->ADC_WPMR = ~ADC_WPMR_WPEN;

    ADC->ADC_MR = ADC_MR_FREERUN_ON
    | ADC_MR_PRESCAL(ADC_Prescaler) 
    | ADC_MR_TRACKTIM(ADC_Tracktime)
    | ADC_MR_TRANSFER(0);

    // ADC mode register, find values for prescaler, think rest is fine
    /*ADC->ADC_MR = ADC_MR_FREERUN_ON
    | ADC_MR_LOWRES_BITS_12
    | ADC_MR_PRESCAL(ADC_Prescaler) 
    | ADC_MR_STARTUP_SUT512 
    | ADC_MR_TRACKTIM(ADC_Tracktime) 
    | ADC_MR_SETTLING_AST5;*/

    //USELESS?
    //ADC->ADC_EMR = ADC_EMR_TAG;

    // Enable channels, probably only need channel (7)
    ADC->ADC_CHER = ADC_CHER_CH7;

    // Start conversions USELESS
    ADC->ADC_CR = ADC_CR_START;

    //ADC->ADC_WPMR |= ADC_WPMR_WPEN;
}

uint16_t ADC_Read() {
    return ADC->ADC_CDR[7];
}