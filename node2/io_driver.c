#include "io_driver.h"

void JOY_Readable_X(io_joy_t* joy) {
    joy->x = (joy->x_dig - joy->x_min) * 100 / (joy->x_max - joy->x_min);
}

void JOY_Readable_Y(io_joy_t* joy) {
    joy->y = (joy->y_dig - joy->y_min) * 100 / (joy->y_max - joy->y_min);
}

void JOY_Readable_Pos(io_joy_t* joy) {
    JOY_Readable_X(joy);
    JOY_Readable_Y(joy);
}