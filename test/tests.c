//
// developed by Sergey Markelov (11/10/2013)
//

#include <clock_extern_functions.h>
#include "clock_extra.h"
#include "ut_clock.h"
#include "ut_clock_time.h"
#include "ut_date_time.h"
#include "ut_clock_button.h"

FILE *errStream;
FILE *outStream;

static TestUnit testSuites[] = {
    { ut_clock, "ut_clock", FALSE },
    { ut_clock_time, "ut_clock_time", FALSE },
    { ut_date_time, "ut_date_time", FALSE },
    { ut_clock_button, "ut_clock_button", FALSE },
};

int main()
{
    errStream = stderr;
    outStream = stdout;
    clock_setPixel = test_setPixel;

    return runTestSuite(testSuites);
}
