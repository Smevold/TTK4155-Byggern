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
    uint8_t menu_option = 0;
    Buttons btns;

    // Begin startup - add function? Tilstandsmodell?
    OLED_Clear(&cursor);
    OLED_Print(OV_logo, &cursor);
    
    _delay_ms(2000);
    
    Interface_Startup(&cursor, &joy, &pad);
    printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n", joy.x_max, joy.x_min, joy.y_max, joy.y_min);
    
    _delay_ms(2000);
    
    OLED_Clear(&cursor);
    Interface_Print_Menu(&cursor);
    
    
    while(1) {
        // Read and compute ADC/JOY-values
        // Add function: readADCandUpdateJOY? / OR interrupt
        ADC_Read(&joy, &pad);
        JOY_Readable_Pos(&joy);
        JOY_Direction(&joy);
        joy.btn = PIND & (1 << PIND4);

        // -- BEGIN JOY OVER CAN TEST
        CAN_Send_Joy(&joy);
        CAN_Receive_Message(&msg_rx);
        
        printf("--- NEW MESSAGE ---\n");
        printf("msg_send: ID: 02, DLC: 07, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", joy.x_max, joy.x_min, joy.y_max, joy.y_min, joy.x_dig, joy.y_dig, joy.btn);
        printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", msg_rx.ID, msg_rx.DLC, msg_tx.Data[0], msg_tx.Data[1], msg_tx.Data[2], msg_tx.Data[3], msg_tx.Data[4], msg_tx.Data[5], msg_tx.Data[6]);
        // -- END JOY OVER CAN TEST
        
        _delay_ms(2000);
        
    }
    
}

