// Copyright [2013] [Sergey Markelov]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// developed by Sergey Markelov (11/10/2013)
//
// @brief BinaryClock emulator (console, uses ncurses)
//

#include <stdlib.h>
#include <ncurses.h>

#include <logger.h>
#include <clock_main.h>
#include "emulator.h"
#include "emulator_button.h"

FILE *errStream;
FILE *outStream;

void atExit()
{
    emulator_deinit();
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
