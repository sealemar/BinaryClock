//
// developed by Sergey Markelov (11/18/2013)
//

#include <clock_time.h>
#include "ut_clock_time.h"

static int assert_clock_updateUptimeMillis_delta(
        unsigned long updateMillis,
        unsigned long expectedDelta,
        unsigned long *lastMillis)
{
    if(lastMillis == NULL) OriginateErrorEx(EINVAL, "%d", "lastMillis is NULL");

    unsigned long delta;
    int res = clock_updateUptimeMillis(updateMillis, lastMillis, &delta);
    if(res) ContinueError(res, "%d");
    if(delta != expectedDelta) {
        OriginateErrorEx(-1, "%d", "Unexpected delta = %lu. Expected = %lu", delta, expectedDelta);
    }

    return 0;
}


static int test_clock_updateUptimeMillis_returnsCorrectDelta()
{
    unsigned long lastMillis;
    unsigned long delta;
    int res = clock_updateUptimeMillis(0, &lastMillis, &delta);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(0, 0, &lastMillis);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(100, 100, &lastMillis);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(5000, 4900, &lastMillis);
    if(res) ContinueError(res, "%d");

    res = clock_updateUptimeMillis(MAX_UPTIME_MILLIS - 5, &lastMillis, &delta);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(MAX_UPTIME_MILLIS, 5, &lastMillis);
    if(res) ContinueError(res, "%d");

    return 0;
}

static int test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly()
{
    unsigned long lastMillis;
    unsigned long delta;
    int res = clock_updateUptimeMillis(MAX_UPTIME_MILLIS, &lastMillis, &delta);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(0, 1, &lastMillis);
    if(res) ContinueError(res, "%d");

    res = clock_updateUptimeMillis(5, &lastMillis, &delta);
    if(res) ContinueError(res, "%d");

    res = assert_clock_updateUptimeMillis_delta(4, MAX_UPTIME_MILLIS, &lastMillis);
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
