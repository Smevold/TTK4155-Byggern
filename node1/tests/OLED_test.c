while(1) {
    ADC_Read(&joy, &pad);
    JOY_Readable_Pos(&joy);
    JOY_Direction(&joy);

    if (joy.dir == DOWN && menu_option > 0) {
        menu_option--;
    } else if (joy.dir == UP && menu_option < 3) {
        menu_option++;
    }

    //OLED_Clear(&cursor);
    //Interface_Print_Menu(&cursor);
    cursor.column_start = 0;
    OLED_Goto_Column(&cursor);
    OLED_Print_Char(font5, ' ', MEDIUM_FONT);
    switch (menu_option) {
        case 0:
            cursor.line_start = 0;
            break;
        case 1:
            cursor.line_start = 8;
            break;
        case 2:
            cursor.line_start = 16;
            break;
        }
        
        
    cursor.column_start = 0;
    OLED_Goto_Pos(&cursor);

    btn = PIND & (1 << PIND4);
    if (btn) {
        OLED_Print_Char(font5, 'X', MEDIUM_FONT);
    } else {
        OLED_Print_Char(font5, 'O', MEDIUM_FONT);
    }

    btns = BTN_Read();
    if (btns.R1 == 1) {
        LED_Set(1, 1);
    } else {
        LED_Set(1,0);
    }
    */

    printf("1: %2X, 2: %2X, 3: %2X\n", btns.right, btns.left, btns.nav);
    printf("Joy dir: %2X\n", joy.dir);
    printf("x: %2X, y: %2X\n", joy.x_dig, joy.y_dig);
    printf("xmax: %2X, xmin: %2X, ymax: %2X, ymin: %2X\n", joy.x_max, joy.x_min, joy.y_max, joy.y_min);
    printf("JoyBTN: %2X\n", btn);
}