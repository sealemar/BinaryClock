#include <clock_time.h>
#include "ut_clock_time.h"

static int validate_clock_updateUptimeMillis_delta(unsigned long updateMillis, unsigned long expectedDelta)
{
    unsigned long delta = clock_updateUptimeMillis(updateMillis);
    if(delta != expectedDelta) {
        OriginateErrorEx(-1, "%d", "Unexpected delta = %lu. Expected = %lu", delta, expectedDelta);
    }

    return 0;
}

static int test_clock_updateUptimeMillis_returnsCorrectDelta()
{
    clock_updateUptimeMillis(0);

    int res;
    res = validate_clock_updateUptimeMillis_delta(0, 0);
    if(res) ContinueError(res, "%d");

    res = validate_clock_updateUptimeMillis_delta(100, 100);
    if(res) ContinueError(res, "%d");

    res = validate_clock_updateUptimeMillis_delta(5000, 4900);
    if(res) ContinueError(res, "%d");

    clock_updateUptimeMillis(MAX_UPTIME_MILLIS - 5);

    res = validate_clock_updateUptimeMillis_delta(MAX_UPTIME_MILLIS, 5);
    if(res) ContinueError(res, "%d");

    return 0;
}

static int test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly()
{
    clock_updateUptimeMillis(MAX_UPTIME_MILLIS);

    int res;
    res = validate_clock_updateUptimeMillis_delta(0, 1);
    if(res) ContinueError(res, "%d");

    clock_updateUptimeMillis(5);

    res = validate_clock_updateUptimeMillis_delta(4, MAX_UPTIME_MILLIS);
    if(res) ContinueError(res, "%d");

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_updateUptimeMillis_returnsCorrectDelta, "clock_updateUptimeMillis() returns correct delta", FALSE },
    { test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly, "clock_updateUptimeMillis() handles integers overflows correctly", FALSE },
};

int ut_clock_time()
{
    return runTestSuite(testSuite);
}
