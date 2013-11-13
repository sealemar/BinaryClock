//
// developed by Sergey Markelov (11/13/2013)
//

#include <ncurses.h>
#include <errno.h>

#include <logger.h>
#include <arch/clock_bits.h>
#include "clock_bits_extra.h"

void setPixelRaw(int x, int y, Bool turnOn)
{
    const int pixel = turnOn ? PIXEL_ON | A_BOLD : PIXEL_OFF | A_DIM;
    x <<= 1;

    if(has_colors())
    {
        const int attr = turnOn
                       ? COLOR_PAIR(COLOR_ON)
                       : COLOR_PAIR(COLOR_OFF);

        mvaddch(y, x, pixel | attr);
    }
    else
    {
        mvaddch(y, x, pixel);
    }
}

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

    setPixelRaw(x, y, turnOn);

    refresh();

    return 0;
}
