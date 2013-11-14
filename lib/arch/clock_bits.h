//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_LIB_ARCH_CLOCK_BITS_H
#define BINARY_CLOCK_LIB_ARCH_CLOCK_BITS_H

//
// @description Architecture dependant clock bits
// Third-party should implement these functions
//

#define ON  1
#define OFF 0

#define CLOCK_SCREEN_WIDTH     8
#define CLOCK_SCREEN_HEIGHT    8

#define CLOCK_PATTERN_SIZE      (CLOCK_SCREEN_HEIGHT)

//
// @brief switches one pixel on or off
// @param x x coordinate (0 < x < SCREEN_WIDTH)
// @param y y coordinate (0 < y < SCREEN_HEIGHT)
// @param turnOn if not 0, the pixel will be turned on
// @returns 0 on ok
// EINVAL - if _x_ or _y_ is out of range
//
extern int (* clock_setPixel)(int x, int y, Bool turnOn);

#endif
