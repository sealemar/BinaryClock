//
// developed by Sergey Markelov (12/03/2013)
//

#include <string.h>

#include <clock_event.h>
#include "ut_clock_event.h"

static int test_clock_event_initDayOfMonth_correct()
{
    const char name[] = "Fool's day";
    ClockEvent event = clock_event_initDayOfMonth(4, APRIL, 1392, name);

    assert_str(event.name, name);
    assert_int(event.year, 1392);

    assert_true(clock_event_isDayOfMonthEvent(event));
    assert_true(!clock_event_isDayOfWeekEvent(event));
    assert_true(!clock_event_isDayOfWeekYear(event));

    assert_int(clock_event_getDayOfMonth(event), 4);
    assert_int(clock_event_getMonth(event), APRIL);
    assert_int(clock_event_getDayOfYear(event), 0);
    assert_int(clock_event_getDayOfWeek(event), 0);
    assert_int(clock_event_getWeekOfMonth(event), ZERO_WEEK_OF_MONTH);
    assert_true(!clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static int test_clock_event_initDayOfWeek_correct()
{
    const char name[] = "Thanksgiving";
    ClockEvent event = clock_event_initDayOfWeek(THURSDAY, 4, TRUE, NOVEMBER, 1574, name);

    assert_str(event.name, name);
    assert_int(event.year, 1574);

    assert_true(!clock_event_isDayOfMonthEvent(event));
    assert_true(clock_event_isDayOfWeekEvent(event));
    assert_true(!clock_event_isDayOfWeekYear(event));

    assert_int(clock_event_getDayOfMonth(event), 0);
    assert_int(clock_event_getMonth(event), NOVEMBER);
    assert_int(clock_event_getDayOfYear(event), DAY_OF_WEEK_FLAG);
    assert_int(clock_event_getDayOfWeek(event), THURSDAY);
    assert_int(clock_event_getWeekOfMonth(event), 4);
    assert_true(clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static int test_clock_event_initDayOfYear_correct()
{
    const char name[] = "Programmer's day";
    ClockEvent event = clock_event_initDayOfYear(256, 2009, name);

    assert_str(event.name, name);
    assert_int(event.year, 2009);

    assert_true(!clock_event_isDayOfMonthEvent(event));
    assert_true(!clock_event_isDayOfWeekEvent(event));
    assert_true(clock_event_isDayOfWeekYear(event));

    assert_int(clock_event_getDayOfMonth(event), 0);
    assert_int(clock_event_getMonth(event), 0);
    assert_int(clock_event_getDayOfYear(event), 256);
    assert_int(clock_event_getDayOfWeek(event), 0);
    assert_int(clock_event_getWeekOfMonth(event), ZERO_WEEK_OF_MONTH);
    assert_true(!clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_event_initDayOfMonth_correct, "clock_event_initDayOfMonth() is correct", FALSE },
    { test_clock_event_initDayOfWeek_correct, "clock_event_initDayOfWeek() is correct", FALSE },
    { test_clock_event_initDayOfYear_correct, "clock_event_initDayOfYear() is correct", FALSE },
};

int ut_clock_event()
{
    return runTestSuite(testSuite);
}
