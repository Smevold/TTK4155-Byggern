//
#include "interrupt.h"

#define CLKA 656250
#define PWM_DUTY_CYCLE_MIN_us 900
#define PWM_DUTY_CYCLE_MAX_us 2100
#define PWM_DUTY_CYCLE_PERCENTILE_STEP ((PWM_DUTY_CYCLE_MAX_us-PWM_DUTY_CYCLE_MIN_us)/100)

void Timer_Init() {
    // Configure PMC to enable Timer Counter Clock (Power Management Controller)
    // enable Counter clock
    // Waveform mode TC_CMR TIOA Out and TIOB out if ot external triggger
    //Need trigger to reset counter
    //This time base is automatically fed back to TIOA of channel 0 when QDEN and SPEEDEN are set.
    // så PWM etterpå me intterupt så regne t tid?

    // Power Management Controller, enable Timer Counter Clock 0
    PMC->PMC_PCER0 = PMC_PCER0_PID27;
   
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 // Prescaler MCK/128
                            | TC_CMR_WAVE             // WAVE MODE
                            | TC_CMR_WAVSEL_UP_RC;   // UP mode with auto trig on RC compare
    // 20 ms  MCK/128  20 * 656250 Hz
    TC0->TC_CHANNEL[0].TC_RC = 13125;                  // The registers RC value. Used to compare with CV and reset when hit

    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //  Enables the RC Compare Interrupt
    
    NVIC_EnableIRQ(TC0_IRQn); // Enable NVIC interrupts for TC0


    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Counter clock enable and software trigger command
}

volatile uint32_t ticks = 0;

TC0_Handler(void) {

    uint32_t sr = TC0->TC_CHANNEL[0].TC_SR;
    if (sr & TC_SR_CPCS){
        ticks++;
    }
}

// PWM Initialization page: 996

void PWM_Init(){
    // Power Management Controller enable PWM Module
    PIOB->PIO_ABSR |= PIO_PB13B_PWMH1;
    PIOB->PIO_PDR |= PIO_PB13B_PWMH1;

    PMC->PMC_PCER1 = PMC_PCER1_PID36;


    // Page 1001
    PWM->PWM_WPCR = 0; // 0 - Disable write protection of register groups, 1 - enable write protection, 2 - enable write protection but can only be disabled by hardware reset of pwm

    // Page 1006
    // 0111 for clk divider MCK/128
    PWM->PWM_CLK = 1 | (0b0111 << PWM_CLK_PREA_Pos); //  turn on clk a, and set prescaler to 128

    // Page 1044
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;

    // Comment to make output from comparator start low, leave it for it to start high (0 on CPOL bit means start low, 1 means start high)
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL;

    // Enable dead time generator
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_DTE;

    //Page 1048
    // Set period to 20 ms
    PWM->PWM_CH_NUM[1].PWM_CPRD = 13125;

    // Page 1046
    // Set duty cycle to 1.5 ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 984;

    // Page 1014
    PWM->PWM_SCM = 0;

    // Page 1017
    PWM->PWM_SCUP = 0;

    PWM->PWM_ENA = PWM_ENA_CHID1; // enable output for channel 1
}


// Change duty cycle in microseconds based on input
void PWM_Set_Duty_Cycle_us(uint16_t us) {
    if (us >= PWM_DUTY_CYCLE_MAX_us) {us = PWM_DUTY_CYCLE_MAX_us;}
    if (us <= PWM_DUTY_CYCLE_MIN_us) {us = PWM_DUTY_CYCLE_MIN_us;}
    uint32_t CDTY_update_value = us*CLKA/1000000;

    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = CDTY_update_value;
}

void PWM_Joy_Duty_Cycle(uint8_t joy_pos, uint16_t* last_us) {
    uint16_t us;

    us = PWM_DUTY_CYCLE_MIN_us + joy_pos * PWM_DUTY_CYCLE_PERCENTILE_STEP;

    // Hysteresis functionality to avoid shaking the servo
    if ((us > (last_us + 100)) | (us < (last_us - 100))) {
        PWM_Set_Duty_Cycle_us(us);
        last_us = us;
    }
    
}