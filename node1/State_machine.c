#include "State_machine.h"

void SM_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad, CAN_MSG* msg_tx, CAN_MSG* msg_rx) {
    OLED_Clear(cursor);
    OLED_Print(OV_logo, cursor);
    
    _delay_ms(2000);
    
    Interface_Startup(cursor, joy, pad);
    printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n\r", joy->x_max, joy->x_min, joy->y_max, joy->y_min);
    
    _delay_ms(2000);
}


void SM_Menu(io_joy_t* joy, cursor_t* cursor, io_pad_t* pad, uint8_t* state) {
    OLED_Clear(cursor);
    Interface_Print_Menu(cursor);

    uint8_t menu_option = 0;
    while(1) {
        // Read and compute ADC/JOY-values
        ADC_Read(joy, pad);
        JOY_Readable_Pos(joy);
        JOY_Direction(joy);
        joy->btn = PIND & (1 << PIND4);

        //Interface_Menu_Nav(joy, cursor, &menu_option);
        // Logic to keep menu_options within the bounds
        if (joy->dir == DOWN && menu_option > 0) {
            menu_option--;
        } else if (joy->dir == UP && menu_option < 2) {
            menu_option++;
        }

        cursor->column_start = 0;
        OLED_Goto_Column(cursor);
        OLED_Print_Char(font5, ' ', MEDIUM_FONT);
        switch (menu_option) {
            case 0:
                cursor->line_start = 0;
                break;
            case 1:
                cursor->line_start = 8;
                break;
        }
            
            
        cursor->column_start = 0;
        OLED_Goto_Pos(cursor);
        
        // Joy btn is connected to pin D 4
        joy->btn = PIND & (1 << PIND4);
        if (joy->btn) {
            // Button is not pressed, do nothing
            OLED_Print_Char(font5, 'X', MEDIUM_FONT);
        } else {
            // Button is pressed, choose current option
            OLED_Print_Char(font5, 'O', MEDIUM_FONT);
            switch (menu_option) {
                // Start game
                case 0:
                    msg_tx->ID = 0x17;
                    msg_tx->DLC = 0x01;
                    msg_tx->Data[0] = 0x00;
                    CAN_Send_Message(msg_tx);
                    CAN_Receive_Message(msg_rx);
                    *state = 1;
                    return;
                // Nothing happens
                case 1:
                    break; 

            }
        }
    }
}


void SM_Running(io_joy_t* joy, io_pad_t* pad, cursor_t* cursor, CAN_MSG* msg_tx, CAN_MSG* msg_rx, Buttons* btns, uint8_t* state) {
    Interface_Print_Game_Running(cursor);
    uint8_t print_count = 0;

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
        
        if (print_count == 10) {
            printf("--- NEW MESSAGE ---\n\r");
            printf("msg_send: ID: 02, DLC: 07, Data: %2X, %2X, %2X, %2X, %2X, %2X, %2X\n\r", joy->x_max, joy->x_min, joy->y_max, joy->y_min, joy->x_dig, joy->y_dig, joy->btn);
            printf("msg_receive: ID: %2X, DLC: %2X, Data: %2X, %2X, %2X\n\r", msg_rx->ID, msg_rx->DLC, msg_rx->Data[0]);
            print_count = 0;
        }
        print_count++;
        // -- END JOY OVER CAN TEST
        
        _delay_ms(100);
    }
}

