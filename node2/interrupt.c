//

timer_init() {
    // Configure PMC to enable Timer Counter Clock (Power Management Controller)
    // enable Counter clock
    // Waveform mode TC_CMR TIOA Out and TIOB out if ot external triggger
    //Need trigger to reset counter
    //TIOA TCLK1
    // Speed measurement ?
    //This time base is automatically fed back to TIOA of channel 0 when QDEN and SPEEDEN are set.
    // så PWM etterpå me intterupt så regne t tid?
   
   
    TC_CCR0->CLKEN = 1;
}
