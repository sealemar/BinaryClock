//
// developed by Sergey Markelov (11/10/2013)
//

#include <clock.h>
#include "clock_extra.h"
#include "ut_clock.h"

FILE *errStream;
FILE *outStream;

TestUnit testSuites[] = {
    { ut_clock_tests, "ut_clock_tests", FALSE },
};

int main()
{
    errStream = stderr;
    outStream = stdout;
    clock_setPixel = test_setPixel;

    return runTestSuite(testSuites);
}
