//
// developed by Sergey Markelov (11/18/2013)
//

#include <memory.h>

#include <clock_time.h>
#include "clock_extra.h"
#include "ut_clock_time.h"

#define MILLIS_IN_A_DAY    86400000UL
#define DATE_TIME_STRING_SIZE (sizeof(DateTime) * 12)

char *dateTimeToString(const DateTime *dt, char s[DATE_TIME_STRING_SIZE])
{
    snprintf(s, DATE_TIME_STRING_SIZE, "%04u-%02u-%02u @ %2u:%02u:%02u.%03d",
             dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->millisecond);
    s[DATE_TIME_STRING_SIZE - 1] = 0;

    return s;
}

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

static int validate_dateTime(DateTime *dt1, unsigned long millisToAddToDt1, const DateTime *dt2)
{
    int res = clock_addMillisToDateTime(millisToAddToDt1, dt1);
    if(res) ContinueError(res, "%d");

    if(memcmp(dt1, dt2, sizeof(DateTime))) {
        char dt1Str[DATE_TIME_STRING_SIZE];
        char dt2Str[DATE_TIME_STRING_SIZE];

        OriginateErrorEx(-1, "%d", "Unexpected result { %s } != expected { %s }",
                         dateTimeToString(dt1, dt1Str),
                         dateTimeToString(dt2, dt2Str));
    }

    return 0;
}

static int test_clock_addMillisToDateTime_correct()
{
    DateTime dt1 = { 0 };
    DateTime dt2 = { 0 };

    dt1.year        = 2013;
    dt1.month       = NOVEMBER;
    dt1.day         = 18;
    dt1.hour        = 22;
    dt1.minute      = 6;
    dt1.second      = 34;
    dt1.millisecond = 0;

    memcpy(&dt2, &dt1, sizeof(dt2));

    int res = validate_dateTime(&dt1, 0, &dt2);
    if(res) ContinueError(res, "%d");


    // + 26 + (53 * 60) seconds

    dt2.hour   = 23;
    dt2.minute = 0;
    dt2.second = 0;

    res = validate_dateTime(&dt1, 1000UL * (26 + (53 * 60)), &dt2);
    if(res) ContinueError(res, "%d");


    // + ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) * 1000 + 999 milliseconds

    dt2.year        = 2013;
    dt2.month       = DECEMBER;
    dt2.day         = 29;
    dt2.hour        = 19;
    dt2.minute      = 15;
    dt2.second      = 35;
    dt2.millisecond = 999;

    res = validate_dateTime(&dt1, 1000UL * ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) + 999, &dt2);
    if(res) ContinueError(res, "%d");


    // + 34 days and 1 millisecond

    dt2.year        = 2014;
    dt2.month       = FEBRUARY;
    dt2.day         = 1;
    dt2.second      = 36;
    dt2.millisecond = 0;

    res = validate_dateTime(&dt1, MILLIS_IN_A_DAY * 34 + 1, &dt2);
    if(res) ContinueError(res, "%d");


    // + 28 days

    dt2.month = MARCH;
    dt2.day   = 1;

    res = validate_dateTime(&dt1, MILLIS_IN_A_DAY * 28, &dt2);
    if(res) ContinueError(res, "%d");


    // 2012-02-01 + 28 days == 2012-02-29

    dt1.year = dt2.year = 2012;
    dt1.month = dt2.month = FEBRUARY;
    dt1.day = 1;
    dt2.day = 29;

    res = validate_dateTime(&dt1, MILLIS_IN_A_DAY * 28, &dt2);
    if(res) ContinueError(res, "%d");

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_updateUptimeMillis_returnsCorrectDelta, "clock_updateUptimeMillis() returns correct delta", FALSE },
    { test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly, "clock_updateUptimeMillis() handles integers overflows correctly", FALSE },
    { test_clock_addMillisToDateTime_correct, "clock_addMillisToDateTime_correct() correct", FALSE },
};

int ut_clock_time()
{
    return runTestSuite(testSuite);
}
