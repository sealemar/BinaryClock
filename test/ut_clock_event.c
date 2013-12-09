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
    assert_int(event.yearStarted, 1392);

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
    assert_int(event.yearStarted, 1574);

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
    assert_int(event.yearStarted, 2009);

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

    struct {
        const ClockEvent *event;
        int year;
        int month;
        int dayOfMonth;
        int dayOfWeek;
    } detailsList[] = {
        { &events[0], 2013, JANUARY,    1, TUESDAY },
        { &events[1], 2013, APRIL,      1, MONDAY },
        { &events[2], 2013, JULY,      26, FRIDAY },
        { &events[3], 2013, SEPTEMBER, 13, FRIDAY },
        { &events[4], 2013, NOVEMBER,  28, THURSDAY },
        { &events[5], 2013, DECEMBER,  25, WEDNESDAY },
        { &events[3], 2000, SEPTEMBER, 12, TUESDAY },
    }, *details = detailsList;

    ClockEventDetails d;

    //
    // code
    //

    for(size_t i = 0; i < countof(detailsList); ++i, ++details) {
        Call( clock_event_getEventDetails( details->event, details->year, &d) );

        assert_int_ex(d.dayOfWeek, details->dayOfWeek, "i = %zu", i);
        assert_int_ex(d.dayOfMonth, details->dayOfMonth, "i = %zu", i);
        assert_int_ex(d.month, details->month, "i = %zu", i);
    }

    return 0;
}

static int test_clock_event_initList_correct()
{
    ClockEvent events[] = {
        clock_event_initDayOfMonth(1,  JANUARY,     0, "New year"),
        clock_event_initDayOfMonth(1,  APRIL,    1392, "Fool's day"),
        clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000, "System Administrator Appreciation Day"),
        clock_event_initDayOfYear (256, 2009, "Programmer's day"),
        clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, "Thanksgiving"),
        clock_event_initDayOfMonth(25, DECEMBER,    0, "Christmas"),
    };

    const ClockEventDetails details[] = {
        clock_event_detailsInit(JANUARY,    1, TUESDAY),
        clock_event_detailsInit(APRIL,      1, MONDAY),
        clock_event_detailsInit(JULY,      26, FRIDAY),
        clock_event_detailsInit(SEPTEMBER, 13, FRIDAY),
        clock_event_detailsInit(NOVEMBER,  28, THURSDAY),
        clock_event_detailsInit(DECEMBER,  25, WEDNESDAY),
    };

    //
    // code
    //

    Call( clock_event_initList(events, countof(events), 2013) );

    for(size_t i = 0; i < countof(events); ++i) {
        assert_int_ex( clock_event_getDayOfWeek(events[i]) , details[i].dayOfWeek, "i = %zu", i);
        assert_int_ex( clock_event_getDayOfMonth(events[i]) , details[i].dayOfMonth, "i = %zu", i);
        assert_int_ex( clock_event_getMonth(events[i]) , details[i].month, "i = %zu", i);
    }

    return 0;
}

static int test_clock_event_updateList_correct()
{
    const char *eventsNames[] = {
        "New year",
        "Fool's day",
        "System Administrator Appreciation Day",
        "Programmer's day",
        "Thanksgiving",
        "Christmas",
    };

    ClockEvent events[] = {
        clock_event_initDayOfMonth(25, DECEMBER,    0, eventsNames[5]),
        clock_event_initDayOfMonth(1,  JANUARY,     0, eventsNames[0]),
        clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, eventsNames[4]),
        clock_event_initDayOfMonth(1,  APRIL,    1392, eventsNames[1]),
        clock_event_initDayOfYear (256, 2009, eventsNames[3]),
        clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000, eventsNames[2]),
    };

    struct {
        int year;
        int month;
        int dayOfMonth;
        int dayOfWeek;
    } details[] = {
        { 2019, JANUARY,    1, TUESDAY },
        { 2019, APRIL,      1, MONDAY },
        { 2019, JULY,      26, FRIDAY },
        { 2019, SEPTEMBER, 13, FRIDAY },
        { 2018, NOVEMBER,  22, THURSDAY },
        { 2018, DECEMBER,  25, TUESDAY },
    };

    //
    // This date is a good one to check when an event is rolled over to the next year.
    // In 2018 Thanksgiving falls on November 22. In 2019 it is in November 28. So, any day
    // before November 29 should cause the event calculation for 2018. And only starting
    // from November 29 it should start calculating to November 28, 2019.
    //
    int year = 2018;
    const DateTime dt = date_time_initDate(year, NOVEMBER, 28);

    //
    // code
    //

    Call( clock_event_initList(events, countof(events), year) );
    Call( clock_event_updateList(events, countof(events), &dt) );

    for(size_t i = 0; i < countof(events); ++i) {
        assert_str_ex(events[i].name, eventsNames[i], "i = %zu", i);
        assert_int_ex(events[i].yearCalculated, details[i].year, "i = %zu", i);
        assert_int_ex( clock_event_getDayOfWeek(events[i]) , details[i].dayOfWeek, "i = %zu", i);
        assert_int_ex( clock_event_getDayOfMonth(events[i]) , details[i].dayOfMonth, "i = %zu", i);
        assert_int_ex( clock_event_getMonth(events[i]) , details[i].month, "i = %zu", i);
    }

    return 0;
}

static int test_clock_event_findClosestFromList_correct()
{
    ClockEvent events[] = {
        clock_event_initDayOfMonth(1,  JANUARY,     0, "New year"),
        clock_event_initDayOfMonth(1,  APRIL,    1392, "Fool's day"),
        clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000, "System Administrator Appreciation Day"),
        clock_event_initDayOfYear (256, 2009, "Programmer's day"),
        clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, "Thanksgiving"),
        clock_event_initDayOfMonth(25, DECEMBER,    0, "Christmas"),
    };

    struct {
        int month;
        int day;
        int index;
    } eventsIndeces[] = {
        { JANUARY,    1, 0 },
        { MARCH,     31, 1 },
        { APRIL,      1, 1 },
        { SEPTEMBER, 13, 3 },
        { NOVEMBER,  27, 4 },
        { NOVEMBER,  28, 4 },
        { NOVEMBER,  29, 5 },
        { DECEMBER,  25, 5 },
        { DECEMBER,  26, 0 },
    }, *eventIndex = eventsIndeces;

    int year = 2013;
    const DateTime dt = date_time_initDate(year, NOVEMBER, 28);

    //
    // code
    //

    Call( clock_event_initList(events, countof(events), year) );
    Call( clock_event_updateList(events, countof(events), &dt) );

    for(size_t i = 0; i < countof(eventsIndeces); ++i, ++eventIndex) {
        int index;
        Call( clock_event_findClosestFromList( events, countof(events), eventIndex->month, eventIndex->day, &index) );

        assert_int_ex(index, eventIndex->index, "i = %zu", i);
    }

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_event_initDayOfMonth_correct, "clock_event_initDayOfMonth() is correct", FALSE },
    { test_clock_event_initDayOfWeek_correct, "clock_event_initDayOfWeek() is correct", FALSE },
    { test_clock_event_initDayOfYear_correct, "clock_event_initDayOfYear() is correct", FALSE },
    { test_clock_event_getEventDetails_correct, "clock_event_getEventDetails() is correct", FALSE },
    { test_clock_event_initList_correct, "clock_event_initList() is correct", FALSE },
    { test_clock_event_updateList_correct, "clock_event_updateList() is correct", FALSE },
    { test_clock_event_findClosestFromList_correct, "clock_event_findClosestFromList() is correct", FALSE },
};

int ut_clock_event()
{
    return runTestSuite(testSuite);
}
