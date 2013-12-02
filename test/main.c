//
// developed by Sergey Markelov (11/10/2013)
//

#include <clock_extern.h>
#include "test.h"
#include "ut_clock.h"
#include "ut_clock_alphabet.h"
#include "ut_clock_button.h"
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
};

int main()
{
    errStream = stderr;
    outStream = stdout;
    clock_extern_setPixel = test_setPixel;
    clock_extern_clearScreen = test_clearScreen;

    return runTestSuite(testSuites);
}
