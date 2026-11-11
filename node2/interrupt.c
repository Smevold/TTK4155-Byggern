//
#include "interrupt.h"

#define CLKA 656250 // Frequancy of clock A (84 MHz / 128)

// Maximum and minimum values for servo duty cycle
#define PWM_SERVO_DUTY_CYCLE_MIN_us 900
#define PWM_SERVO_DUTY_CYCLE_MAX_us 2100

// Maximum and minimum values for motor duty cycle
#define PWM_MOTOR_DUTY_CYCLE_MIN_us 0
#define PWM_MOTOR_DUTY_CYCLE_MAX_us 2100

// Percentile steps of duty cycle, 
#define PWM_SERVO_DUTY_CYCLE_PERCENTILE_STEP ((PWM_SERVO_DUTY_CYCLE_MAX_us-PWM_SERVO_DUTY_CYCLE_MIN_us)/100)
#define PWM_MOTOR_DUTY_CYCLE_PERCENTILE_STEP ((PWM_MOTOR_DUTY_CYCLE_MAX_us-PWM_MOTOR_DUTY_CYCLE_MIN_us)/100)

// Hysterisis value for servo duty cycle
#define PWM_DUTY_CYCLE_HYSTERESIS 100

// Total of PWM channels enabled
#define PWM_TOTAL_CHANNELS_EN 2

void Timer_Init() {
    // Power Management Controller, enable Timer Counter Clock 0 and 2
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
    PMC->PMC_PCER1 |= PMC_PCER1_PID33;

    // TC0
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 // Prescaler MCK/128
                            | TC_CMR_WAVE             // WAVE MODE
                            | TC_CMR_WAVSEL_UP_RC;   // UP mode with auto trig on RC compare
    

    // 20 ms  MCK/128  20 * 656250 Hz
    TC0->TC_CHANNEL[0].TC_RC = 13125; // The registers RC value. Used to compare with CV and reset when hit

    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; //  Enables the RC Compare Interrupt
    
    NVIC_EnableIRQ(TC0_IRQn); // Enable NVIC interrupts for TC0

    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Counter clock enable and software trigger command

    // TC2, used for quadratic encoder
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 // Select clock XC0
                                | TC_CMR_ETRGEDG_RISING // External trigger edge on rising edge
                                | TC_CMR_ABETRG; // Select clock XC0
    

    TC2->TC_BMR = TC_BMR_POSEN // Enable position measurement
                | TC_BMR_QDEN // Enable Quadratic Encoder
                | TC_BMR_EDGPHA; // Enable edge detection on both PHA and PHB

    NVIC_EnableIRQ(TC2_IRQn); // Enable NVIC interrupts for TC2


    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enables clock, and restart counter
}

volatile uint32_t ticks = 0; // Ticks used by interrupts
volatile uint8_t pid_tick = 0; // Ticks used for pid to trigger


// Updates ticks and pid_tick every 20 ms
TC0_Handler(void) {

    uint32_t sr = TC0->TC_CHANNEL[0].TC_SR;
    if (sr & TC_SR_CPCS){
        ticks++;
        pid_tick = 1;
    }
}

// PWM Initialization page: 996

void PWM_Init(){
    // Turn on PWM peripheral for pins B13 and B12    
    PIOB->PIO_ABSR |= PIO_PB13B_PWMH1;
    PIOB->PIO_PDR |= PIO_PB13B_PWMH1;

    PIOB->PIO_ABSR |= PIO_PB12B_PWMH0;
    PIOB->PIO_PDR |= PIO_PB12B_PWMH0;

    // Power Management Controller enable PWM Module
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;


    // Page 1001
    PWM->PWM_WPCR = 0; // 0 - Disable write protection of register groups, 1 - enable write protection, 2 - enable write protection but can only be disabled by hardware reset of pwm

    // Page 1006
    // 0111 for clk divider MCK/128
    PWM->PWM_CLK = 1 | (0b0111 << PWM_CLK_PREA_Pos); //  turn on clk a, and set prescaler to 128

    // Page 1044
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA; // Select clock a for channel 1
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA; // Select clock a for channel 0


    // Signal starts high, and enable dead time generator
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL | PWM_CMR_DTE;
    PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPOL | PWM_CMR_DTE;

    //Page 1048
    // Set period to 20 ms
    PWM->PWM_CH_NUM[1].PWM_CPRD = 13125;
    PWM->PWM_CH_NUM[0].PWM_CPRD = 13125;

    // Page 1046
    // Set duty cycle to 1.5 ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 984;
    PWM->PWM_CH_NUM[0].PWM_CDTY = 1; // This value is changed almost immedietaly on startup, and does not really matter,

    // Page 1014
    PWM->PWM_SCM = 0; // Sets channels to by asynchronous

    // Page 1017
    PWM->PWM_SCUP = 0; // Sets synchronous update register, but with 0 synchronous channels, value does not matter

    PWM->PWM_ENA = PWM_ENA_CHID1 | PWM_ENA_CHID0; // enable output for channel 1 and 0
}


// Change duty cycle in microseconds based on input
void PWM_Set_Duty_Cycle_us(uint16_t us, uint8_t channel) {
    // Check if given channel is valid / enabled
    uint32_t CDTY_update_value;
    if (channel < PWM_TOTAL_CHANNELS_EN) {
        // Logic given channel is 1, or servo PWM signal
        if (channel == 1) {
            if (us >= PWM_SERVO_DUTY_CYCLE_MAX_us) {us = PWM_SERVO_DUTY_CYCLE_MAX_us;}
            if (us <= PWM_SERVO_DUTY_CYCLE_MIN_us) {us = PWM_SERVO_DUTY_CYCLE_MIN_us;}
            CDTY_update_value = us*CLKA/1000000;
        }
        // Logic given channel is 0, or motor PWM signal
        if (channel == 0) {
            if (us >= PWM_MOTOR_DUTY_CYCLE_MAX_us) {us = PWM_MOTOR_DUTY_CYCLE_MAX_us;}
            if (us <= PWM_MOTOR_DUTY_CYCLE_MIN_us) {us = PWM_MOTOR_DUTY_CYCLE_MIN_us;}
            CDTY_update_value = us*CLKA/1000000;
        }

        // Update channel's duty cycle register
        PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = CDTY_update_value;
    }
}

void PWM_Joy_Duty_Cycle(uint8_t duty_cycle_percentile, uint16_t* last_us, uint8_t channel) {
    uint16_t us;
    if (channel == 0) {
        // Value to motor PWM is given from an absolute from a range from [-50, 50], so multiply by 2 to have the right width
        us = PWM_MOTOR_DUTY_CYCLE_MIN_us + duty_cycle_percentile * 2 * PWM_MOTOR_DUTY_CYCLE_PERCENTILE_STEP;
        PWM_Set_Duty_Cycle_us(us, channel);
    }

    if (channel == 1) {
        us = PWM_SERVO_DUTY_CYCLE_MIN_us + duty_cycle_percentile * PWM_SERVO_DUTY_CYCLE_PERCENTILE_STEP;
        // Hysteresis functionality to avoid shaking the servo
        if ((us > (*last_us + PWM_DUTY_CYCLE_HYSTERESIS)) | (us < (*last_us - PWM_DUTY_CYCLE_HYSTERESIS))) {
            PWM_Set_Duty_Cycle_us(us, channel);
            *last_us = us;
        }
    }

    
    
    
    
}