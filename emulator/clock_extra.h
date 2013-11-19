//
// developed by Sergey Markelov (11/13/2013)
//

#ifndef BINARY_CLOCK_EMULATOR_CLOCK_EXTRA_H
#define BINARY_CLOCK_EMULATOR_CLOCK_EXTRA_H

#define PIXEL_ON   'o'
#define PIXEL_OFF  '-'
#define COLOR_ON     1
#define COLOR_OFF    2

//
// @brief sets pixel, but doesn't refresh the screen
//
void setPixelRaw(int x, int y, Bool turnOn);

int emulator_setPixel(int x, int y, Bool turnOn);

int clock_init();
void clock_clearScreen();

#endif
