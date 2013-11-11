//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_EMULATOR_ARCH_CLOCK_BITS_EXTRA_H
#define BINARY_CLOCK_EMULATOR_ARCH_CLOCK_BITS_EXTRA_H

#include <ncurses.h>
#include <arch/clock_bits.h>

inline static void setPixel(int x, int y, Bool turnOn)
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

#endif
