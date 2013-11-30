//
// developed by Sergey Markelov (11/10/2013)
//

#include <stdlib.h>
#include <ncurses.h>

#include <logger.h>
#include <clock_main.h>
#include <alphabet.h>
#include "emulator.h"
#include "emulator_button.h"

FILE *errStream;
FILE *outStream;

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

    ClockState cs;
    Call(clock_init(&cs));

    for(int ch = getch(); ch != 27; ch = getch())
    {
        Call(emulator_button_press(&cs.buttons, ch, NULL));
        Call(emulator_update(&cs));
        Call(clock_update(&cs));
    }

    return 0;
}