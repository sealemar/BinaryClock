//
// developed by Sergey Markelov (11/10/2013)
//

#include <arch/clock_bits.h>
#include <logger.h>
#include "ut_clock.h"

FILE *errStream;
FILE *outStream;

int main()
{
    errStream = stderr;
    outStream = stdout;

    int errors = 0;

    // TODO: Makefile - *.o, *.d -> obj/
    // TODO: Makefile - lib*.a, exec -> bin/
    // TODO: think about author header

    int res = ut_clock_tests();
    if(res) { IgnoreError(res, "%d"); ++errors; }

    // TODO: refactor with struct and extra information when a new unit test is added
    Log("1 test suite ran. %d tests suites succeded, %d failed", 1 - errors, errors);

    return 0;
}
