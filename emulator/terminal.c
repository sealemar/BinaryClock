//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <unistd.h>
#include <clock.h>
#include <logger.h>

FILE *errStream;
FILE *outStream;

int main()
{
    errStream = stderr;
    outStream = stdout;
    clock_setPixel = emulator_setPixel;

    int res = clock_init();
    if(res) ContinueError(res, "%d");

    clock_clearScreen();

//     res = clock_setPixel(1, 1, 1);
//     if(res) ContinueError(res, "%d");
//
//     usleep(5000000);
//
//     res = clock_setPixel(1, 1, 0);
//     if(res) ContinueError(res, "%d");

    for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
    {
        for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
        {
            res = clock_setPixel(x, y, 1);
            if(res) ContinueError(res, "%d");
            usleep(100 * 1000);
            clock_setPixel(x, y, 0);
        }
    }

    res = clock_drawPattern(PATTERN_SMILEY);
    if(res) ContinueError(res, "%d");

    getch();
    endwin();

    return 0;
}
