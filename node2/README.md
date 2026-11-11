# TTK4155 Node 2

Code for node 2 in TTK4155 Embedded and Industrial Computer Systems Design at NTNU. This repository consists of two parts: starter code given by TTK4155 and code made by group 34, consisting Fredrik S.A., August H.S., Ina M.R., developed in a Linux build environment.

The starter code contains two kinds of files:

## Cross-platform starter code (Linux and Windows)

- `can.c` / `can.h`    CAN driver
- `time.c` / `time.h`  timing helpers
- `uart.c` / `uart.h`  UART driver + printf support

## Linux-only build environment

- `main.c`   example entry point
- `Makefile` arm-none-eabi-gcc build + openocd flashing
- `sam/`     CMSIS headers, startup code, linker script, openocd config

All other files have been created by Group 34, and some others have been modified.

The tests directory consists of examples of some tests used during development, to be stored for later viewing.

