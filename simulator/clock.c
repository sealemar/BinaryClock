#include <ncurses.h>
#include <clock.h>
#include "logger.h"

#define PIXEL_ON   '*'
#define PIXEL_OFF  '+'
#define COLOR_ON     1
#define COLOR_OFF    2

const char PATTERN_SMILEY[] = { 0x3c, 0x42, 0xa5, 0x81, 0xa5, 0x99, 0x42, 0x3c };

inline static void _setPixel(int x, int y, int turnOn)
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
// @brief initializes clock
//
int clock_init()
{
    initscr();              // Start curses mode
    raw();                  // Line buffering disabled
    keypad(stdscr, TRUE);   // We get F1, F2 etc..
    noecho();               // Don't echo() while we do getch
    curs_set(0);            // make the cursor invisible

    if(has_colors())
    {
        start_color();
        init_pair(COLOR_ON,  COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_OFF, COLOR_BLUE, COLOR_BLACK);
    }

    return 0;
}

//
// @brief clears screen turning all pixels off
//
void clock_clearScreen()
{
    for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
    {
        for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
        {
            _setPixel(x, y, false);
        }
    }
    refresh();
}

//
// @brief switches one pixel on or off
// @param x x coordinate (0 < x < SCREEN_WIDTH)
// @param y y coordinate (0 < y < SCREEN_HEIGHT)
// @param turnOn if not 0, the pixel will be turned on
// @returns 0 on ok
//
int clock_setPixel(int x, int y, int turnOn)
{
    if(x < 0 || x >= CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(-1, "%d", "Invalid _x_ value [%d], should be 0 < x < %d", x, CLOCK_SCREEN_WIDTH);
    if(y < 0 || y >= CLOCK_SCREEN_HEIGHT)
        OriginateErrorEx(-1, "%d", "Invalid _y_ value [%d], should be 0 < y < %d", y, CLOCK_SCREEN_HEIGHT);

    _setPixel(x, y, turnOn);

    refresh();

    return 0;
}

//
// @brief draws a pattern on the screen
// @see PATTERN_* above
//
int clock_drawPattern(const char* pattern)
{
    if(pattern == NULL)
        OriginateErrorEx(-1, "%d", "pattern is NULL");

    for (int i = 0; i < CLOCK_SCREEN_HEIGHT; ++i)
    {
        const char ch = pattern[i];

        _setPixel(0, i, ch & 0x01);
        _setPixel(1, i, ch & 0x02);
        _setPixel(2, i, ch & 0x04);
        _setPixel(3, i, ch & 0x08);
        _setPixel(4, i, ch & 0x10);
        _setPixel(5, i, ch & 0x20);
        _setPixel(6, i, ch & 0x40);
        _setPixel(7, i, ch & 0x80);
    }

    return 0;
}
