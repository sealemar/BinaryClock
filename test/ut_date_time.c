//
// developed by Sergey Markelov (11/19/2013)
//

#include <memory.h>

#include <date_time.h>
#include "ut_date_time.h"

#define DATE_TIME_STRING_SIZE (sizeof(DateTime) * 12)

static char *dateTimeToString(const DateTime *dt, char s[DATE_TIME_STRING_SIZE])
{
    snprintf(s, DATE_TIME_STRING_SIZE, "%04d-%02d-%02d @ %2d:%02d:%02d.%03d",
             dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->millisecond);
    s[DATE_TIME_STRING_SIZE - 1] = 0;

    return s;
}

static int assert_dateTime(DateTime *dt1, const DateTime *dt2)
{
    if(memcmp(dt1, dt2, sizeof(DateTime))) {
        char dt1Str[DATE_TIME_STRING_SIZE];
        char dt2Str[DATE_TIME_STRING_SIZE];

        OriginateErrorEx(-1, "%d", "Unexpected result { %s } != expected { %s }",
                         dateTimeToString(dt1, dt1Str),
                         dateTimeToString(dt2, dt2Str));
    }

    return 0;
}

static int test_dateTime_addMillis_correct()
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

    int res = dateTime_addMillis(&dt1, 0);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    // + 26 + (53 * 60) seconds

    dt2.hour   = 23;
    dt2.minute = 0;
    dt2.second = 0;

    res = dateTime_addMillis(&dt1, 1000UL * (26 + (53 * 60)));
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    // + ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) * 1000 + 999 milliseconds

    dt2.year        = 2013;
    dt2.month       = DECEMBER;
    dt2.day         = 29;
    dt2.hour        = 19;
    dt2.minute      = 15;
    dt2.second      = 35;
    dt2.millisecond = 999;

    res = dateTime_addMillis(&dt1, 1000UL * ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) + 999);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    // + 34 days and 1 millisecond

    dt2.year        = 2014;
    dt2.month       = FEBRUARY;
    dt2.day         = 1;
    dt2.second      = 36;
    dt2.millisecond = 0;

    res = dateTime_addMillis(&dt1, MILLIS_IN_A_DAY * 34 + 1);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    // + 28 days

    dt2.month = MARCH;
    dt2.day   = 1;

    res = dateTime_addMillis(&dt1, MILLIS_IN_A_DAY * 28);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    // 2012-02-01 + 28 days == 2012-02-29

    dt1.year = dt2.year = 2012;
    dt1.month = dt2.month = FEBRUARY;
    dt1.day = 1;
    dt2.day = 29;

    res = dateTime_addMillis(&dt1, MILLIS_IN_A_DAY * 28);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");

    return 0;
}

static int test_dateTime_normalize_handlesOverflows()
{
    DateTime dt1 = { 0 };
    DateTime dt2 = { 0 };


    //
    // 120 seconds
    //

    dt1.year        = 2013;
    dt1.month       = NOVEMBER;
    dt1.day         = 18;
    dt1.hour        = 22;
    dt1.minute      = 6;
    dt1.second      = 120;
    dt1.millisecond = 0;

    memcpy(&dt2, &dt1, sizeof(dt2));

    dt2.minute = 8;
    dt2.second = 0;

    int res = dateTime_normalize(&dt1);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");


    //
    // 31 months, 180 days, 220 hours, 68 minutes, 10000 seconds, 5001 milliseconds
    //

    dt1.year        = 2013;
    dt1.month       = 31;
    dt1.day         = 180;
    dt1.hour        = 220;
    dt1.minute      = 68;
    dt1.second      = 10000;
    dt1.millisecond = 5001;

    dt2.year        = 2016;
    dt2.month       = JANUARY;
    dt2.day         = 5;
    dt2.hour        = 7;
    dt2.minute      = 54;
    dt2.second      = 45;
    dt2.millisecond = 1;

    res = dateTime_normalize(&dt1);
    if(res) ContinueError(res, "%d");
    res = assert_dateTime(&dt1, &dt2);
    if(res) ContinueError(res, "%d");

    return 0;
}

static TestUnit testSuite[] = {
    { test_dateTime_addMillis_correct, "dateTime_addMillis() correct", FALSE },
    { test_dateTime_normalize_handlesOverflows, "dateTime_normalize() handles overflows", FALSE },
};

int ut_date_time()
{
    return runTestSuite(testSuite);
}
