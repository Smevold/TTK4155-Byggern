#include "../Drivers/OLED_driver.h"
#include "../Drivers/SPI_driver.h"
#include "../Graphics/OLED_graphics.h"
#include "../Graphics/fonts.h"

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#include <string.h>

#define F_CPU 4915200UL
#include<util/delay.h>
#include "Game_menu.h"