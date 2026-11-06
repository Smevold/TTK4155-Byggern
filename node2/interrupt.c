//
#include "interrupt.h"

void Timer_Init() {
    // Configure PMC to enable Timer Counter Clock (Power Management Controller)
    // enable Counter clock
    // Waveform mode TC_CMR TIOA Out and TIOB out if ot external triggger
    //Need trigger to reset counter
    //This time base is automatically fed back to TIOA of channel 0 when QDEN and SPEEDEN are set.
    // så PWM etterpå me intterupt så regne t tid?
   
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

    Pwm* pwm;
    PwmCh_num* pwm_channel_num;
    uint32_t status;

pwm->PWM_ENA = PWM_ENA_CHID1; // enable output for cchannel 1

// Page 1001
pwm->PWM_WPCR = 0; // 0 - Disable write protection of register groups, 1 - enable write protection, 2 - enable write protection but can only be disabled by hardware reset of pwm
//temp->PWM_WPSR = 0; // Read protected, becomes 0 on reset, see page 1004 in the datasheet


// Page 1006
// 0111 for clk divider MCK/128
pwm->PWM_CLK = 1 | (0b0111 << PWM_CLK_PREA_Pos); //  turn on clk a, and set prescaler to 128

// Page 1044
// To write to the register, WPSR have to be cleared
// To clear WPSR, we have to read it, see page 1039, specially the note at the bottom of the page
status = pwm->PWM_WPSR; // <------- Think this is correct, remove if you are sure it is correct
pwm_channel_num->PWM_CMR = PWM_CMR_CPRE_CLKA; // <-------------- Good chance this is wrong

// Uncomment to choose center aligned, leave it for left aligned (0 on the CALG bit means left, 1 means center)
//pwm_channel_num->PWM_CMR |= PWM_CMR_CALG; 

// Comment to make output from comparator start low, leave it for it to start high (0 on CPOL bit means start low, 1 means start high)
pwm_channel_num->PWM_CMR |= PWM_CMR_CPOL;

// Enable dead time generator
pwm_channel_num->PWM_CMR |= PWM_CMR_DTE;

//Page 1048
status = pwm->PWM_WPSR;
// Might be really wrong, don't know VVVVVVVVVVVVVVVVV
pwm_channel_num->PWM_CPRD = 1680000; // Assuming it is left aligned, using the formula: CPRD * DIVA / MCK = Period of output signal. For Period = 20 ms, CPRD must be equal to 1680000 in decimal

// Page 1046
// See page 978
// Assume fchannel_x_clock is CLKA, or MCK / 128
// This might be very wrong, espacially if anything above is wrong
pwm_channel_num->PWM_CDTY = 12141;

// Page 1014
status = pwm->PWM_WPSR;
pwm->PWM_SCM = 0; // The channel(s) do not need to be synchronous

// Page 1017
pwm->PWM_SCUP = 0;

// Page 1051
// No clue here, might be 4??
pwm_channel_num->PWM_DT = (4 << PWM_DT_DTH_Pos) | (4 << PWM_DT_DTL_Pos);


pwm->PWM_ENA = 1;

}
