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
    assert_false(clock_event_isDayOfWeekEvent(event));
    assert_false(clock_event_isDayOfYearEvent(event));

    assert_int(clock_event_getDayOfMonth(event), 4);
    assert_int(clock_event_getMonth(event), APRIL);
    assert_int(clock_event_getDayOfYear(event), 0);
    assert_int(clock_event_getDayOfWeek(event), 0);
    assert_int(clock_event_getWeekOfMonth(event), 0);
    assert_false(clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static int test_clock_event_initDayOfWeek_correct()
{
    const char name[] = "Thanksgiving";
    ClockEvent event = clock_event_initDayOfWeek(THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, name);

    assert_str(event.name, name);
    assert_int(event.year, 1574);

    assert_false(clock_event_isDayOfMonthEvent(event));
    assert_true(clock_event_isDayOfWeekEvent(event));
    assert_false(clock_event_isDayOfYearEvent(event));

    assert_int(clock_event_getDayOfMonth(event), 0);
    assert_int(clock_event_getMonth(event), NOVEMBER);
    assert_int(clock_event_getDayOfYear(event), DAY_OF_WEEK_FLAG);
    assert_int(clock_event_getDayOfWeek(event), THURSDAY);
    assert_int(clock_event_getWeekOfMonth(event), 3);
    assert_true(clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static int test_clock_event_initDayOfYear_correct()
{
    const char name[] = "Programmer's day";
    ClockEvent event = clock_event_initDayOfYear(256, 2009, name);

    assert_str(event.name, name);
    assert_int(event.year, 2009);

    assert_false(clock_event_isDayOfMonthEvent(event));
    assert_false(clock_event_isDayOfWeekEvent(event));
    assert_true(clock_event_isDayOfYearEvent(event));

    assert_int(clock_event_getDayOfMonth(event), 0);
    assert_int(clock_event_getMonth(event), 0);
    assert_int(clock_event_getDayOfYear(event), 256);
    assert_int(clock_event_getDayOfWeek(event), 0);
    assert_int(clock_event_getWeekOfMonth(event), 0);
    assert_false(clock_event_isFromBeginningOfMonth(event));

    return 0;
}

static int test_clock_event_getEventDetails_correct()
{
    const ClockEvent events[] = {
        clock_event_initDayOfMonth(1,  JANUARY,     0, "New year"),
        clock_event_initDayOfMonth(1,  APRIL,    1392, "Fool's day"),
        clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000, "System Administrator Appreciation Day"),
        clock_event_initDayOfYear (256, 2009, "Programmer's day"),
        clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, "Thanksgiving"),
        clock_event_initDayOfMonth(25, DECEMBER,    0, "Christmas"),
    };

    int year = 2013;
    ClockEventDetails details;

    Call( clock_event_getEventDetails( &(events[0]), year, &details) );
    assert_int(details.dayOfWeek, TUESDAY);
    assert_int(details.dayOfMonth, 1);
    assert_int(details.month, JANUARY);

    Call( clock_event_getEventDetails( &(events[1]), year, &details) );
    assert_int(details.dayOfWeek, MONDAY);
    assert_int(details.dayOfMonth, 1);
    assert_int(details.month, APRIL);

    Call( clock_event_getEventDetails( &(events[2]), year, &details) );
    assert_int(details.dayOfWeek, FRIDAY);
    assert_int(details.dayOfMonth, 26);
    assert_int(details.month, JULY);

    Call( clock_event_getEventDetails( &(events[3]), year, &details) );
    assert_int(details.dayOfWeek, FRIDAY);
    assert_int(details.dayOfMonth, 13);
    assert_int(details.month, SEPTEMBER);

    Call( clock_event_getEventDetails( &(events[4]), year, &details) );
    assert_int(details.dayOfWeek, THURSDAY);
    assert_int(details.dayOfMonth, 28);
    assert_int(details.month, NOVEMBER);

    Call( clock_event_getEventDetails( &(events[5]), year, &details) );
    assert_int(details.dayOfWeek, WEDNESDAY);
    assert_int(details.dayOfMonth, 25);
    assert_int(details.month, DECEMBER);

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_event_initDayOfMonth_correct, "clock_event_initDayOfMonth() is correct", FALSE },
    { test_clock_event_initDayOfWeek_correct, "clock_event_initDayOfWeek() is correct", FALSE },
    { test_clock_event_initDayOfYear_correct, "clock_event_initDayOfYear() is correct", FALSE },
    { test_clock_event_getEventDetails_correct, "clock_event_getEventDetails() is correct", FALSE },
};

int ut_clock_event()
{
    return runTestSuite(testSuite);
}
