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

int checkClockScreen()
{
    for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
    {
        for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
        {
            int res = clock_setPixel(x, y, 1);
            if(res) ContinueError(res, "%d");
            clock_setPixel(x, y, 0);
            if(getch() == 27) return 0;
        }
    }

    return 0;
}

int traverseAlphabet()
{
    int i = 0;
    int ch;
    do {
        int res = clock_drawPattern(CLOCK_ALPHABET[i]);
        if(res) ContinueError(res, "%d");

        ch = getch();
        if(ch == 'h' || ch == 'k') {
            --i;
            if(i == -1) {
                i = CLOCK_ALPHABET_SIZE - 1;
            }
        } else {
            ++i;
            i %= CLOCK_ALPHABET_SIZE;
        }
    } while (ch != 27);

    return 0;
}

int slideText(const char* text)
{
    Bool isLastStep = FALSE;

    for(size_t step = 0; !isLastStep; ++step) {
        unsigned char pattern[CLOCK_PATTERN_SIZE];

        int res = clock_slideText(text, step, &isLastStep, pattern);
        //int res = clock_slidePattern(CLOCK_ALPHABET[CLOCK_SMILEY_FACE_SAD], CLOCK_ALPHABET[CLOCK_SMILEY_FACE_SMILE], step, &isLastStep, pattern);
        if(res) ContinueError(res, "%d");

        res = clock_drawPattern(pattern);
        if(res) ContinueError(res, "%d");

        if(getch() == 27) return 0;
    }

    return 0;
}

int main()
{
    errStream = stderr;
    outStream = stdout;

    int res = clock_init();
    if(res) ContinueError(res, "%d");

    clock_clearScreen();

    //res = slideText(" Hello world!!!");
    res = clock_displayBinaryNumber(0x1c, 2, 2);
    if(res) ContinueError(res, "%d");
    getch();

    endwin();

    return 0;
}
