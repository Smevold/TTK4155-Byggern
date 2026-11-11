#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>
#include "../Drivers/CAN_communication_driver.h"

void Interface_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad);

void Interface_Print_Menu(cursor_t* cursor);

void Interface_Print_Game_Running(cursor_t* cursor);

void Interface_Print_Score(cursor_t* cursor, CAN_MSG* msg_rx);
