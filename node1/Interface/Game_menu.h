#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

void Interface_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad);

void Interface_Print_Menu(cursor_t* cursor);
