//
// developed by Sergey Markelov (11/10/2013)
//

#include <stdlib.h>
#include <ncurses.h>

#include <logger.h>
#include <clock_main.h>
#include <alphabet.h>
#include "clock_extra.h"

FILE *errStream;
FILE *outStream;

#define DELAY_BETWEEN_LOOP_ITERATIONS 100

void atExit()
{
    clock_deinit();
}

int main()
{
    errStream = stderr;
    outStream = stdout;

    Call(emulator_init());
    atexit(atExit);

    clock_clearScreen();

    ClockState cs;

    Call(clock_init(&cs));

    while(getch() != 27)
    {
        emulator_delay(DELAY_BETWEEN_LOOP_ITERATIONS);
        Call(clock_update(&cs));
    }

//     res = checkDisplayBinaryNumber();
//     res = displayDateTime();
//     if(res) ContinueError(res, "%d");
//     getch();

    return 0;
}
