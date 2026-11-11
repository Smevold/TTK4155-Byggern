#pragma once

#include "Drivers/IO_driver.h" // DON*T MOVE THIS FURTHER DOWN, OTHERWISE COMPILER WON'T RECOGNIS STRUCTS!!!!

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>  

#include "Drivers/OLED_driver.h"
#include "Interface/Game_menu.h"
#include "Drivers/CAN_communication_driver.h"
#include "Drivers/CAN_controller_driver.h"
#include "Graphics/OLED_graphics.h"

void SM_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad);
void SM_Menu(io_joy_t* joy, io_joy_t* cursor);
void SM_Running(io_joy_t* joy, io_pad_t* pad, CAN_MSG* msg_tx, CAN_MSG* msg_rx, Buttons* btns);