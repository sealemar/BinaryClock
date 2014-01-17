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
// @brief BinaryClock tests main module
//
// developed by Sergey Markelov (11/10/2013)
//

#include <clock_extern.h>
#include "test.h"
#include "ut_clock.h"
#include "ut_clock_alphabet.h"
#include "ut_clock_button.h"
#include "ut_clock_event.h"
#include "ut_clock_time.h"
#include "ut_date_time.h"

FILE *errStream;
FILE *outStream;

static TestUnit testSuites[] = {
    { ut_clock, "ut_clock", FALSE },
    { ut_clock_time, "ut_clock_time", FALSE },
    { ut_date_time, "ut_date_time", FALSE },
    { ut_clock_button, "ut_clock_button", FALSE },
    { ut_clock_alphabet, "ut_clock_alphabet", FALSE },
    { ut_clock_event, "ut_clock_event", FALSE },
};

int main()
{
    errStream = stderr;
    outStream = stdout;
    clock_extern_setPixel = test_setPixel;
    clock_extern_clearScreen = test_clearScreen;

    return runTestSuite(testSuites);
}
