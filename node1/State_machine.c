#include "State_machine.h"

SM_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad) {
    OLED_Clear(cursor);
    OLED_Print(OV_logo, cursor);
    
    _delay_ms(2000);
    
    Interface_Startup(cursor, joy, pad);
    printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n", joy->x_max, joy->x_min, joy->y_max, joy->y_min);
    
    _delay_ms(2000);
    
    OLED_Clear(cursor);
    Interface_Print_Menu(cursor);
}


SM_Running(io_joy_t* joy, io_pad_t* pad, CAN_MSG* msg_tx, CAN_MSG* msg_rx, Buttons* btns) {
    while(1) {
        // Read and compute ADC/JOY-values
        // Add function: readADCandUpdateJOY? / OR interrupt
        ADC_Read(joy, pad);
        JOY_Readable_Pos(joy);
        JOY_Direction(joy);
        joy->btn = PIND & (1 << PIND4);
        
        
        // -- BEGIN JOY OVER CAN TEST
        CAN_Send_Joy(joy);
        CAN_Receive_Message(msg_rx);
        
        //printf("--- NEW MESSAGE ---\n");
        //printf("msg_send: ID: 02, DLC: 07, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n", joy->x_max, joy->x_min, joy->y_max, joy->y_min, joy->x_dig, joy->y_dig, joy->btn);
        //printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X\n", msg_rx->ID, msg_rx->DLC, msg_rx->Data[0]);
        // -- END JOY OVER CAN TEST
        
        _delay_ms(100);
    }
}