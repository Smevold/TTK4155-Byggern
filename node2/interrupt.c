//

void Timer_Init() {
    // Configure PMC to enable Timer Counter Clock (Power Management Controller)
    // enable Counter clock
    // Waveform mode TC_CMR TIOA Out and TIOB out if ot external triggger
    //Need trigger to reset counter
    //This time base is automatically fed back to TIOA of channel 0 when QDEN and SPEEDEN are set.
    // så PWM etterpå me intterupt så regne t tid?
   
   TC0->CHANNEL[0].TC_CMR = TC_CM_TCCLKS_TIMER_CLOCK4 // Prescaler MCK/128
                            | TC_CMR_WAVE             // WAVE MODE
                            | TC_CMR_WAVESEL_UP_RC;   // UP mode with auto trig on RC compare
    // 20 ms  MCK/128  20 * 656250 Hz
    TC0->TC_CHANNEL[0].TC_RC = 13125;                  // The registers RC value. Used to compare with CV and reset when hit

    TC0->TC_CHANNEL[0].TC_IER = TC_OER_CPCS; //  Enables the RC Compare Interrupt
    
    NVIC_EnableIRQ(TC0_IRQn);


    
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN 1 TC_CCR_SWIRG;
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

Pwm->PWM_ENA = PWM_ENA_CHID1; // enable output for cchannel 1

Pwm->Pwm_WPCR = 0; // protection
Pwm->Pwm_WPSR = 0; // protection


Pwm->Pwm_CLK = 1;

// 0111 for clk divider MCP/128
Pwm->PWM_CLK = 1 | (7 << PWM_CLK_PREA_Pos); //  turn on clk a and b, choose prescaler



}
