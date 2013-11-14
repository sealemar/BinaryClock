//
// developed by Sergey Markelov (11/10/2013)
//

#include <ncurses.h>
#include <clock.h>
#include <logger.h>
#include <alphabet.h>

#include "clock_extra.h"
FILE *errStream;
FILE *outStream;

int main()
{
    errStream = stderr;
    outStream = stdout;

    int res = clock_init();
    if(res) ContinueError(res, "%d");

    clock_clearScreen();

//     for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
//     {
//         for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
//         {
//             res = clock_setPixel(x, y, 1);
//             if(res) ContinueError(res, "%d");
//             usleep(100 * 1000);
//             clock_setPixel(x, y, 0);
//         }
//     }

    int i = 0;
    int ch;
    do {
        res = clock_drawPattern(CLOCK_ALPHABET[i]);
        if(res) ContinueError(res, "%d");

        ch = getch();
        if(ch == 'h' || ch == 'k') --i;
        else ++i;
        i %= CLOCK_ALPHABET_SIZE;
    } while (ch != 27);

    endwin();

    return 0;
}
