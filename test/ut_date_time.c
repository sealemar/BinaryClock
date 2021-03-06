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
// @brief BinaryClock lib/date_time unit tests
//
// developed by Sergey Markelov (11/19/2013)
//

#include <string.h>

#include <date_time.h>
#include "ut_date_time.h"

#define DATE_TIME_STRING_SIZE (sizeof(DateTime) * 12)

static char *dateTimeToString(const DateTime *dt, char s[DATE_TIME_STRING_SIZE])
{
    snprintf(s, DATE_TIME_STRING_SIZE, "%s %02d, %04d @ %2d:%02d:%02d.%03d",
             DateTimeMonthsStr[dt->month], dt->day, dt->year, dt->hour, dt->minute, dt->second, dt->millisecond);
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

static int test_date_time_addMillis_correct()
{
    DateTime dt1;
    DateTime dt2;

    dt1.year        = 2013;
    dt1.month       = NOVEMBER;
    dt1.day         = 18;
    dt1.hour        = 22;
    dt1.minute      = 6;
    dt1.second      = 34;
    dt1.millisecond = 0;

    memcpy(&dt2, &dt1, sizeof(dt2));

    Call(date_time_addMillis(&dt1, 0));
    Call(assert_dateTime(&dt1, &dt2));


    // + 26 + (53 * 60) seconds

    dt2.hour   = 23;
    dt2.minute = 0;
    dt2.second = 0;

    Call(date_time_addMillis(&dt1, 1000UL * (26 + (53 * 60))));
    Call(assert_dateTime(&dt1, &dt2));


    // + ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) * 1000 + 999 milliseconds

    dt2.year        = 2013;
    dt2.month       = DECEMBER;
    dt2.day         = 29;
    dt2.hour        = 19;
    dt2.minute      = 15;
    dt2.second      = 35;
    dt2.millisecond = 999;

    Call(date_time_addMillis(&dt1, 1000UL * ((((((40 * 24) + 20) * 60) + 15) * 60) + 35) + 999));
    Call(assert_dateTime(&dt1, &dt2));


    // + 34 days and 1 millisecond

    dt2.year        = 2014;
    dt2.month       = FEBRUARY;
    dt2.day         = 1;
    dt2.second      = 36;
    dt2.millisecond = 0;

    Call(date_time_addMillis(&dt1, MILLIS_IN_A_DAY * 34 + 1));
    Call(assert_dateTime(&dt1, &dt2));


    // + 28 days

    dt2.month = MARCH;
    dt2.day   = 1;

    Call(date_time_addMillis(&dt1, MILLIS_IN_A_DAY * 28));
    Call(assert_dateTime(&dt1, &dt2));


    // 2012-02-01 + 28 days == 2012-02-29

    dt1.year = dt2.year = 2012;
    dt1.month = dt2.month = FEBRUARY;
    dt1.day = 1;
    dt2.day = 29;

    Call(date_time_addMillis(&dt1, MILLIS_IN_A_DAY * 28));
    Call(assert_dateTime(&dt1, &dt2));

    return 0;
}

static int test_date_time_normalize_handlesOverflows()
{
    DateTime dt1;
    DateTime dt2;


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

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // 31 months, 180 days, 220 hours, 68 minutes, 10000 seconds, 5001 milliseconds
    //

    dt1.year        = 2013;
    dt1.month       = 30;
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

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));

    return 0;
}

static int test_date_time_normalize_handlesUnderflows()
{
    DateTime dt1;
    DateTime dt2;


    //
    // -(6 * 60) seconds
    //

    dt1.year        = 2013;
    dt1.month       = NOVEMBER;
    dt1.day         = 18;
    dt1.hour        = 22;
    dt1.minute      = 6;
    dt1.second      = -(6 * 60);
    dt1.millisecond = 0;

    memcpy(&dt2, &dt1, sizeof(dt2));

    dt2.minute = 0;
    dt2.second = 0;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // - 24 hours
    //

    dt1.hour -= 24;
    dt2.day -= 1;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // - MILLIS_IN_A_DAY
    //

    dt1.millisecond -= MILLIS_IN_A_DAY;
    dt2.day -= 1;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // 0 day
    //
    dt1.day = 0;

    dt2.month = OCTOBER;
    dt2.day = 31;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // -1 day
    //
    dt1.day = -1;

    dt2.month = SEPTEMBER;
    dt2.day = 29;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));


    //
    // some ridiculous negative
    //
    dt1.year        =   -100;
    dt1.month       =    -42;
    dt1.day         =   -101;
    dt1.hour        =   -900;
    dt1.minute      =   -141;
    dt1.second      =   -324;
    dt1.millisecond = -23232;

    dt2.year        = -104;
    dt2.month       = FEBRUARY;
    dt2.day         =   12;
    dt2.hour        =    9;
    dt2.minute      =   33;
    dt2.second      =   12;
    dt2.millisecond =  768;

    Call(date_time_normalize(&dt1));
    Call(assert_dateTime(&dt1, &dt2));

    return 0;
}

static int test_date_time_timeToStr_correct()
{
    if(DATE_TIME_TIME_STR_SIZE != 9) {
        OriginateErrorEx(-1, "%d", "Unexpected DATE_TIME_TIME_STR_SIZE = %d. Should be 9", DATE_TIME_TIME_STR_SIZE);
    }

    DateTime dt;
    char str[DATE_TIME_TIME_STR_SIZE];

    dt.hour = 12;
    dt.minute = 45;
    dt.second = 59;

    Call(date_time_timeToStr(&dt, str));
    assert_str(str, "12:45:59");


    dt.hour = 23;
    dt.minute = 5;
    dt.second = 8;

    Call(date_time_timeToStr(&dt, str));
    assert_str(str, "23:05:08");


    dt.hour = 0;
    dt.minute = 9;
    dt.second = 18;

    Call(date_time_timeToStr(&dt, str));
    assert_str(str, "0:09:18");

    return 0;
}

static int test_date_time_timeToStr_returnsERANGEIfValuesAreOutOfRange()
{
    DateTime dt;
    char str[DATE_TIME_TIME_STR_SIZE];

    dt.hour = HOURS_COUNT;
    dt.minute = 45;
    dt.second = 59;

    assert_function(date_time_timeToStr(&dt, str), ERANGE);


    dt.hour = 23;
    dt.minute = 60;
    dt.second = 8;

    assert_function(date_time_timeToStr(&dt, str), ERANGE);


    dt.hour = 23;
    dt.minute = 6;
    dt.second = -8;

    assert_function(date_time_timeToStr(&dt, str), ERANGE);

    return 0;
}

static int test_date_time_dateToStr_correct()
{
    if(DATE_TIME_DATE_STR_SIZE != 22) {
        OriginateErrorEx(-1, "%d", "Unexpected DATE_TIME_DATE_STR_SIZE = %d. Should be 22", DATE_TIME_DATE_STR_SIZE);
    }

    struct {
        DateTime dt;
        int dayOfWeek;
    } dts[] = {
        { date_time_initDate(2013, NOVEMBER, 28), THURSDAY },
        { date_time_initDate(1968, JANUARY,  31), WEDNESDAY },
    }, *dt = dts;
    char str[DATE_TIME_DATE_STR_SIZE];
    char expected[DATE_TIME_DATE_STR_SIZE];

    for(size_t i = 0; i < countof(dts); ++i, ++dt) {
        Call( date_time_dateToStr( &(dt->dt), str) );
        sprintf(expected, "%s %02d %04d %s",
                          DateTimeMonthsStr[dt->dt.month],
                          dt->dt.day,
                          dt->dt.year,
                          DateTimeDayOfWeekStr[dt->dayOfWeek]);
        assert_str(str, expected);

        size_t l = strlen(expected);
        if(l >= DATE_TIME_DATE_STR_SIZE) {
            OriginateErrorEx(EFAULT, "%d", "strlen(expected) [%zu] should be less than %d",
                                           l, DATE_TIME_DATE_STR_SIZE);
        }
    }

    return 0;
}

static int test_date_time_dateToStr_returnsERANGEIfValuesAreOutOfRange()
{
    DateTime dt;
    char str[DATE_TIME_DATE_STR_SIZE];

    dt.year = -1;
    dt.month = JANUARY;
    dt.day = 31;

    assert_function(date_time_dateToStr(&dt, str), ERANGE);


    dt.year = 2013;
    dt.month = DECEMBER + 1;
    dt.day = 31;

    assert_function(date_time_dateToStr(&dt, str), ERANGE);


    dt.year = 2013;
    dt.month = DECEMBER;
    dt.day = 32;

    assert_function(date_time_dateToStr(&dt, str), ERANGE);

    return 0;
}

static int test_date_time_daysInMonth_correct()
{
    int daysInMonth;

    Call(date_time_daysInMonth(2013, JANUARY, &daysInMonth));
    assert_number(daysInMonth, 31, "%d", "%d");


    Call(date_time_daysInMonth(2013, FEBRUARY, &daysInMonth));
    assert_number(daysInMonth, 28, "%d", "%d");

    Call(date_time_daysInMonth(2012, FEBRUARY, &daysInMonth));
    assert_number(daysInMonth, 29, "%d", "%d");

    Call(date_time_daysInMonth(2012, DECEMBER, &daysInMonth));
    assert_number(daysInMonth, 31, "%d", "%d");

    Call(date_time_daysInMonth(2012, APRIL, &daysInMonth));
    assert_number(daysInMonth, 30, "%d", "%d");

    return 0;
}

static int test_date_time_daysInMonth_returnsERANGEIfMonthIsOutOfRange()
{
    int daysInMonth;

    assert_function(date_time_daysInMonth(2013, JANUARY - 1, &daysInMonth), ERANGE);
    assert_function(date_time_daysInMonth(2013, DECEMBER + 1, &daysInMonth), ERANGE);

    return 0;
}

static int test_date_time_calculateDayOfWeek_correct()
{
    int dayOfWeek;

    Call(date_time_calculateDayOfWeek(2013, SEPTEMBER, 13, &dayOfWeek));
    assert_int(dayOfWeek, FRIDAY);

    Call(date_time_calculateDayOfWeek(2013, DECEMBER, 4, &dayOfWeek));
    assert_int(dayOfWeek, WEDNESDAY);

    Call(date_time_calculateDayOfWeek(2000, JANUARY, 1, &dayOfWeek));
    assert_int(dayOfWeek, SATURDAY);

    Call(date_time_calculateDayOfWeek(2000, DECEMBER, 31, &dayOfWeek));
    assert_int(dayOfWeek, SUNDAY);

    Call(date_time_calculateDayOfWeek(1777, APRIL, 30, &dayOfWeek));
    assert_int(dayOfWeek, WEDNESDAY);

    Call(date_time_calculateDayOfWeek(1582, OCTOBER, 4, &dayOfWeek));
    assert_int(dayOfWeek, MONDAY);

    return 0;
}

static TestUnit testSuite[] = {
    { test_date_time_addMillis_correct, "date_time_addMillis() correct", FALSE },
    { test_date_time_normalize_handlesOverflows, "date_time_normalize() handles overflows", FALSE },
    { test_date_time_normalize_handlesUnderflows, "date_time_normalize() handles underflows", FALSE },
    { test_date_time_timeToStr_correct, "date_time_timeToStr() converts time correctly", FALSE },
    { test_date_time_timeToStr_returnsERANGEIfValuesAreOutOfRange, "date_time_timeToStr() returns ERANGE if _dt_ values are out of range", FALSE },
    { test_date_time_dateToStr_correct, "date_time_dateToStr() converts date correctly", FALSE },
    { test_date_time_dateToStr_returnsERANGEIfValuesAreOutOfRange, "date_time_dateToStr() returns ERANGE if _dt_ values are out of range", FALSE },
    { test_date_time_daysInMonth_correct, "date_time_daysInMonth() returns correct result", FALSE },
    { test_date_time_daysInMonth_returnsERANGEIfMonthIsOutOfRange, "date_time_daysInMonth() returns ERANGE if month is out of range", FALSE },
    { test_date_time_calculateDayOfWeek_correct, "date_time_calculateDayOfWeek() returns correct result", FALSE },
};

int ut_date_time()
{
    return runTestSuite(testSuite);
}
