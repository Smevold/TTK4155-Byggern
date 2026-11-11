#pragma once
SM_Startup(cursor_t* cursor, io_joy_t* joy, io_pad_t* pad)
SM_Running(io_joy_t joy, io_pad_t* pad, CAN_MSG* msg_tx, CAN_MSG* msg_rx, Buttons btns);