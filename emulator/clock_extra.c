//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <errno.h>
#include <unistd.h>     // for usleep()

#include <logger.h>
#include <clock.h>
#include "clock_extra.h"

static const char BannerStr[] = "BinaryClock Emulator version " TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR)
                                " Developed by Sergey Markelov";

static WINDOW *WndBanner    = NULL;
static WINDOW *WndClockFace = NULL;
static WINDOW *WndButtons   = NULL;
static WINDOW *WndNotes     = NULL;

typedef struct {
    const int x;
    const int y;
    const int letter;
    Bool isOn;
} Button;

static Button Buttons[] = {
    { 0, 0, '1', FALSE },
    { 0, 2, '2', FALSE },
    { 0, 4, '3', FALSE },
    { 0, 6, '4', FALSE },
};

#define _destroyWindow(wnd) \
    if(wnd) { \
        int _res = delwin(wnd); \
        if(_res) OriginateErrorEx(_res, "%d", "delwnd(" TOSTRING(wnd) ") returned %d", _res); \
        wnd = NULL; \
    }

#define _newwin(wnd, height, width, beginY, beginX) { \
    wnd = newwin(height, width, beginY, beginX); \
    if(wnd == NULL) { \
        OriginateErrorEx(ENOMEM, "%d", "newwin(%d, %d, %d, %d) failed", height, width, beginY, beginX); \
    } \
}

#define _mvwprintw(wnd, y, x, format, ...) { \
    int _res = mvwprintw(wnd, y, x, format, ##__VA_ARGS__); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "mvwprintw(" TOSTRING(wnd) ", %d, %d, ...) failed with %d", \
                                     y, x, _res); \
    } \
}

#define _mvwaddch(wnd, y, x, ch) { \
    int _res = mvwaddch(wnd, y, x, ch); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "mvwaddch(" TOSTRING(wnd) ", %d, %d, " TOSTRING(ch) ") failed with %d", \
                                    y, x, _res); \
    } \
}

#define _wrefresh(wnd) { \
    int _res = wrefresh(wnd); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "wrefresh(" TOSTRING(wnd) ") failed with %d", _res); \
    } \
}


static int destroyWindows()
{
    _destroyWindow(WndClockFace);
    _destroyWindow(WndButtons);
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
    _newwin(WndButtons, CLOCK_SCREEN_HEIGHT, 0, 2, (CLOCK_SCREEN_WIDTH << 1) + 3);
    _newwin(WndNotes, 0, 0, CLOCK_SCREEN_HEIGHT + 5, 0);

    return 0;
}

static int initWindowBanner()
{
    _mvwprintw(WndBanner, 0, 0, "%s", BannerStr);
    _wrefresh(WndBanner);

    return 0;
}

static int initWindowButtons()
{
    const Button *btn = Buttons;

    for(size_t i = 0; i < countof(Buttons); ++i, ++btn) {
        const int attr = has_colors()
                       ? btn->isOn ? COLOR_PAIR(COLOR_OFF) : COLOR_PAIR(COLOR_OFF)
                       : 0;

        _mvwaddch(WndButtons, btn->y, btn->x, '|' | attr);
        _mvwaddch(WndButtons, btn->y, btn->x + 1, btn->letter | attr);
        _mvwaddch(WndButtons, btn->y, btn->x + 2, '|' | attr);
    }

    _wrefresh(WndButtons);

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

void emulator_delay(unsigned long millis)
{
    usleep(millis * 1000);
}

//
// @brief initializes clock
//
int clock_init()
{
    clock_setPixel = emulator_setPixel;

    initscr();              // Start curses mode
    raw();                  // Line buffering disabled
    keypad(stdscr, TRUE);   // We get F1, F2 etc..
    noecho();               // Don't echo() while we do getch
    curs_set(0);            // make the cursor invisible
    timeout(0);             // getch() in non blocking mode

    if(has_colors())
    {
        start_color();
        init_pair(COLOR_ON,  COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_OFF, COLOR_BLUE, COLOR_BLACK);
    }

    int res = createWindows();
    if(res) ContinueError(res, "%d");

    refresh();

    res = initWindowBanner();
    if(res) ContinueError(res, "%d");

    res = initWindowButtons();
    if(res) ContinueError(res, "%d");

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
