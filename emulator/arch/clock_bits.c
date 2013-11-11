//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <arch/clock_bits.h>
#include "clock_bits_extra.h"

//
// @brief switches one pixel on or off
// @param x x coordinate (0 < x < SCREEN_WIDTH)
// @param y y coordinate (0 < y < SCREEN_HEIGHT)
// @param turnOn if not FALSE, the pixel will be turned on
// @returns 0 on ok
//
int emulator_setPixel(int x, int y, Bool turnOn)
{
    if(x < 0 || x >= CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(EINVAL, "%d", "Invalid _x_ value [%d], should be 0 < x < %d", x, CLOCK_SCREEN_WIDTH);
    if(y < 0 || y >= CLOCK_SCREEN_HEIGHT)
        OriginateErrorEx(EINVAL, "%d", "Invalid _y_ value [%d], should be 0 < y < %d", y, CLOCK_SCREEN_HEIGHT);

    setPixel(x, y, turnOn);

    refresh();

    return 0;
}
