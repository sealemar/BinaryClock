//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <logger.h>

#define PIXEL_ON   'o'
#define PIXEL_OFF  '-'
#define COLOR_ON     1
#define COLOR_OFF    2

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
            _setPixel(x, y, FALSE);
        }
    }
    refresh();
}
