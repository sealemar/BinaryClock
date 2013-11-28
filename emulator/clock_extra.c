//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <errno.h>
#include <unistd.h>     // for usleep()
#include <sys/time.h>   // for gettimeofday()

#include <logger.h>
#include <clock.h>
#include "clock_extra.h"
#include "include.h"
#include "emulator_button.h"

#define GETCH_TIMEOUT 50

static const char BannerStr[] = "BinaryClock Emulator version " TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR)
                                " Developed by Sergey Markelov";

static WINDOW *WndBanner    = NULL;
static WINDOW *WndClockFace = NULL;
static WINDOW *WndNotes     = NULL;


static int destroyWindows()
{
    Call(emulator_deinit_buttons());
    _destroyWindow(WndClockFace);
    _destroyWindow(WndNotes);
    _destroyWindow(WndBanner);

    return 0;
}

static int createWindows()
{
    int res = destroyWindows();
    if(res) ContinueError(res, "%d");

    _newwin(WndBanner, 2, 0, 0, 0);
    _newwin(WndClockFace, CLOCK_SCREEN_HEIGHT, CLOCK_SCREEN_WIDTH << 1, 2, 0);
    emulator_init_buttons();
    _newwin(WndNotes, 0, 0, CLOCK_SCREEN_HEIGHT + 5, 0);

    return 0;
}

static int initWindowBanner()
{
    _mvwprintw(WndBanner, 0, 0, "%s", BannerStr);
    _wrefresh(WndBanner);

    return 0;
}

int setPixelRaw(int x, int y, Bool turnOn)
{
    const int pixel = turnOn ? PIXEL_ON | A_BOLD : PIXEL_OFF | A_DIM;
    x <<= 1;

    if(has_colors())
    {
        const int attr = turnOn
                       ? COLOR_PAIR(COLOR_ON)
                       : COLOR_PAIR(COLOR_OFF);

        _mvwaddch(WndClockFace, y, x, pixel | attr);
    }
    else
    {
        _mvwaddch(WndClockFace, y, x, pixel);
    }

    return 0;
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

    int res = setPixelRaw(x, y, turnOn);
    if(res) ContinueError(res, "%d");

    _wrefresh(WndClockFace);

    return 0;
}

int emulator_uptimeMillis(unsigned long *millis)
{
    struct timeval tv;
    int res = gettimeofday(&tv, NULL);
    if(res) OriginateErrorEx(errno, "%d", "clock_gettime(CLOCK_MONOTONIC, &ts) failed with %d", errno);

    *millis = tv.tv_usec / 1000 + tv.tv_sec * 1000;

    return 0;
}

void emulator_delay(unsigned long millis)
{
    usleep(millis * 1000);
}

//
// @brief initializes emulator
//
int emulator_init()
{
    clock_setPixel = emulator_setPixel;
    clock_uptimeMillis = emulator_uptimeMillis;

    initscr();              // Start curses mode
    raw();                  // Line buffering disabled
    keypad(stdscr, TRUE);   // We get F1, F2 etc..
    noecho();               // Don't echo() while we do getch
    curs_set(0);            // make the cursor invisible
    timeout(GETCH_TIMEOUT); // delay for that many milliseconds in getch()

    if(has_colors())
    {
        start_color();
        init_pair(COLOR_ON,  COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_OFF, COLOR_BLUE, COLOR_BLACK);
    }

    Call(createWindows());

    refresh();

    Call(initWindowBanner());

    clock_clearScreen();

    return 0;
}

void clock_deinit()
{
    destroyWindows();
    endwin();
}

//
// @brief clears screen turning all pixels off
//
int clock_clearScreen()
{
    for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
    {
        for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
        {
            int res = setPixelRaw(x, y, FALSE);
            if(res) ContinueError(res, "%d");
        }
    }
    _wrefresh(WndClockFace);

    return 0;
}
