//
// developed by Sergey Markelov (11/13/2013)
//

#ifndef BINARY_CLOCK_EMULATOR_EMULATOR_H
#define BINARY_CLOCK_EMULATOR_EMULATOR_H

#include <clock_state.h>

#define PIXEL_ON   'o'
#define PIXEL_OFF  '-'
#define COLOR_ON     1
#define COLOR_OFF    2

//
// @brief sets pixel, but doesn't refresh the screen
//
int setPixelRaw(int x, int y, Bool turnOn);

int emulator_setPixel(int x, int y, Bool turnOn);

int emulator_uptimeMillis(unsigned long *millis);

int emulator_initDateTime(DateTime *dt);

int emulator_init();

//
// @brief call this function from the main loop
//
int emulator_update(const ClockState *cs);

//
// @brief If clock_init() was called, this function must be called
//        to deinitialize and free resources.
//
void clock_deinit();

#endif
