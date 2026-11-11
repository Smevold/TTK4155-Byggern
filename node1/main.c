#define F_CPU 4915200UL

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define UART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)

#include "tests/sram_test.h"
#include "tests/pin_test.h"
#include "Drivers/UART_driver.h"
#include "Drivers/IO_driver.h"
#include "Drivers/SPI_driver.h"
#include "Drivers/OLED_driver.h" 
#include "Drivers/ISR_driver.h"
#include "Drivers/CAN_communication_driver.h"
#include "Drivers/CAN_controller_driver.h"
#include "Graphics/OLED_graphics.h"
#include "Graphics/fonts.h"
#include "Interface/Game_menu.h"
#include "State_machine.h"


void main(){
    // Enable interrupts service
    sei();
    
    // Init functions
    EXTRAM_Init();
    UART_Init (BAUD_PRESCALE);
    JOY_Init(&joy);
    ADC_Init();
    SPI_Init();
    OLED_Init();
    ISR_Init();
    CAN_Init();
    
    // Variables needed
    cursor_t cursor;
    CAN_MSG msg_tx;
    CAN_MSG msg_rx; 
    Buttons btns;
    uint8_t state = 0;
    
    // Begin startup 
    SM_Startup(&cursor, &joy, &pad);
    
    
    while(1) {
        switch(state) {
            case 0:
                
            case 1:
                uint8_t menu_option = 0;
                SM_Running(&joy, &pad, &msg_tx, &msg_rx, &btns);
                break;
        }
        
    }
    
}

