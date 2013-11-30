//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <errno.h>
#include <time.h>       // for localtime_r()
#include <sys/time.h>   // for gettimeofday()

#include <logger.h>
#include <clock.h>
#include <clock_extern.h>

#include "emulator.h"
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
    Call(emulator_button_deinit());
    _destroyWindow(WndClockFace);
    _destroyWindow(WndNotes);
    _destroyWindow(WndBanner);

    return 0;
}

static int createWindows()
{
    int res = destroyWindows();
    if(res) ContinueError(res, "%d");

    CallMalloc( WndBanner, newwin(2, 0, 0, 0) );
    CallMalloc( WndClockFace, newwin(CLOCK_SCREEN_HEIGHT, CLOCK_SCREEN_WIDTH << 1, 2, 0) );
    Call( emulator_button_init() );
    CallMalloc( WndNotes, newwin(0, 0, CLOCK_SCREEN_HEIGHT + 5, 0) );

    return 0;
}

static int initWindowBanner()
{
    CallNcurses( mvwprintw(WndBanner, 0, 0, "%s", BannerStr) );
    CallNcurses( wrefresh(WndBanner) );

    return 0;
}

static int emulator_setPixelRaw(int x, int y, Bool turnOn)
{
    const int pixel = turnOn ? PIXEL_ON | A_BOLD : PIXEL_OFF | A_DIM;
    x <<= 1;

    if(has_colors())
    {
        const int attr = turnOn
                       ? COLOR_PAIR(COLOR_ON)
                       : COLOR_PAIR(COLOR_OFF);

        CallNcurses( mvwaddch(WndClockFace, y, x, pixel | attr) );
    }
    else
    {
        CallNcurses( mvwaddch(WndClockFace, y, x, pixel) );
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
static int emulator_setPixel(int x, int y, Bool turnOn)
{
    if(x < 0 || x >= CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(EINVAL, "%d", "Invalid _x_ value [%d], should be 0 < x < %d", x, CLOCK_SCREEN_WIDTH);
    if(y < 0 || y >= CLOCK_SCREEN_HEIGHT)
        OriginateErrorEx(EINVAL, "%d", "Invalid _y_ value [%d], should be 0 < y < %d", y, CLOCK_SCREEN_HEIGHT);

    Call(emulator_setPixelRaw(x, y, turnOn));
    CallNcurses( wrefresh(WndClockFace) );

    return 0;
}

static int emulator_uptimeMillis(unsigned long *millis)
{
    struct timeval tv;
    CallOriginateErrno(gettimeofday(&tv, NULL));

    *millis = tv.tv_usec / 1000 + tv.tv_sec * 1000;

    return 0;
}

static int emulator_initDateTime(DateTime *dt)
{
    NullCheck(dt);

    struct timeval tv;
    CallOriginateErrno(gettimeofday(&tv, NULL));

    struct tm ts;
    localtime_r( &tv.tv_sec, &ts );

    dt->millisecond = tv.tv_usec / 1000;
    dt->second = ts.tm_sec;
    dt->minute = ts.tm_min;
    dt->hour   = ts.tm_hour;
    dt->day    = ts.tm_mday;
    dt->month  = ts.tm_mon + JANUARY;
    dt->year   = ts.tm_year + 1900;

    return 0;
}

static int emulator_clearScreen()
{
    for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x) {
        for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y) {
            Call(emulator_setPixelRaw(x, y, FALSE));
        }
    }
    CallNcurses( wrefresh(WndClockFace) );

    return 0;
}

//
// @brief initializes emulator
//
int emulator_init()
{
    clock_extern_setPixel = emulator_setPixel;
    clock_extern_uptimeMillis = emulator_uptimeMillis;
    clock_extern_initDateTime = emulator_initDateTime;
    clock_extern_clearScreen = emulator_clearScreen;

    CallMallocQuiet( initscr() );         // Start curses mode
    CallNcurses( raw() );                 // Line buffering disabled
    CallNcurses( keypad(stdscr, TRUE) );  // We get F1, F2 etc..
    CallNcurses( noecho() );              // Don't echo() while we do getch
    CallNcurses( curs_set(0) );           // make the cursor invisible
    timeout(GETCH_TIMEOUT); // delay for that many milliseconds in getch()

    if(has_colors()) {
        CallNcurses( start_color() );
        CallNcurses( init_pair(COLOR_ON,  COLOR_YELLOW, COLOR_BLACK) );
        CallNcurses( init_pair(COLOR_OFF, COLOR_BLUE, COLOR_BLACK) );
    }

    Call(createWindows());
    CallNcurses( refresh() );
    Call(initWindowBanner());
    Call(clock_extern_clearScreen());

    return 0;
}

//
// @brief call this function from the main loop
//
int emulator_update(const ClockState *cs)
{
    Call(emulator_button_update(cs));

    return 0;
}

int emulator_deinit()
{
    Call(destroyWindows());
    CallNcurses( endwin() );

    return 0;
}
