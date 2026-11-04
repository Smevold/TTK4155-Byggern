 // -- BEGIN BTN / LED TEST - ADD TO FILE
btns = BTN_Read();
if (btns.R1 == 1) {
    LED_Set(1, 1);
} else {
    LED_Set(1,0);
}
// -- END BTN / LED TEST