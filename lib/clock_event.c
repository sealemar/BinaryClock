//
// developed by Sergey Markelov (12/02/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "clock_event.h"

#define clock_event_setDayOfWeek(event, dayOfWeek) { \
    (event).blob_2 &= ~(7 << 4); \
    (event).blob_2 |= (dayOfWeek & 7) << 4; \
}

#define clock_event_setDayOfMonth(event, dayOfMonth) { \
    (event).blob_1 &= ~(0x1f); \
    (event).blob_1 |= dayOfMonth & 0x1f; \

#define clock_event_setMonth(event, month) { \
    (event).blob_2 &= ~(0x0f); \
    (event).blob_2 |= month & 0x0f; \
}

static inline int _getDetailsForDayOfMonthEvent(const ClockEvent *event, ClockEventDetails *eventDetails)
{
    eventDetails->month = clock_event_getMonth(*event);
    eventDetails->dayOfMonth = clock_event_getDayOfMonth(*event);

    Call( date_time_calculateDayOfWeek(
                event->year,
                eventDetails->month,
                eventDetails->dayOfMonth,
                &(eventDetails->dayOfWeek) ));

    return 0;
}

static inline int _getDetailsForDayOfWeekEvent(const ClockEvent *event, ClockEventDetails *eventDetails)
{
    int_fast8_t month = clock_event_getMonth(*event);
    int_fast8_t dayOfWeek = clock_event_getDayOfWeek(*event);
    int_fast8_t weekNumber = clock_event_getWeekOfMonth(*event);
    int lastDayOfMonth;
    int dayOfMonth = 0;
    Call( date_time_daysInMonth(event->year, month, &lastDayOfMonth) );

    if(clock_event_isFromBeginningOfMonth(*event)) {
        int lastDayOfWeek;
        Call( date_time_calculateDayOfWeek(event->year, month, 1, &lastDayOfWeek) );
        if(lastDayOfWeek > dayOfWeek) {
            ++weekNumber;
        }
        dayOfMonth = 1 + weekNumber * 7 + (dayOfWeek - lastDayOfWeek);
    } else {
        int lastDayOfWeek;
        Call( date_time_calculateDayOfWeek(event->year, month, lastDayOfMonth, &lastDayOfWeek) );
        if(lastDayOfWeek < dayOfWeek) {
            ++weekNumber;
        }
        dayOfMonth = lastDayOfMonth - weekNumber * 7 - (lastDayOfWeek - dayOfWeek);
    }

#ifdef PARAM_CHECKS
    if(dayOfMonth > lastDayOfMonth || dayOfMonth <= 0) {
        OriginateErrorEx(ERANGE, "%d", "Calculated day of month [%d] is out of range [1..%d] "
                "for the month of %s, for the %d %s from the %s of the month",
                dayOfMonth, lastDayOfMonth, DateTimeMonthsStr[month],
                weekNumber, DateTimeDayOfWeekStr[dayOfWeek],
                clock_event_isFromBeginningOfMonth(*event) ? "beginning" : "end");
    }
#endif

    eventDetails->dayOfWeek = dayOfWeek;
    eventDetails->dayOfMonth = dayOfMonth;
    eventDetails->month = month;

    return 0;
}

static inline int _getDetailsForDayOfYearEvent(const ClockEvent *event, ClockEventDetails *eventDetails)
{
    int d = clock_event_getDayOfYear(*event);
    int daysInMonth = 0;
    int_fast8_t month = JANUARY - 1;

    while(d >= daysInMonth) {
        ++month;
        d -= daysInMonth;
        Call( date_time_daysInMonth(event->year, month, &daysInMonth) );
    }

    eventDetails->dayOfMonth = d;
    eventDetails->month = month;

    Call( date_time_calculateDayOfWeek(
                event->year,
                eventDetails->month,
                eventDetails->dayOfMonth,
                &(eventDetails->dayOfWeek) ));

    return 0;
}

//
// @brief Calculates event details for a given year
//
// @param event a pointer to ClockEvent
// @param year a year to calculate the event for
// @param eventDetails a pointer to ClockEventDetails
//
// @returns 0 on ok
//   EINVAL if _event_ is NULL
//          if _eventDetails_ is NULL
//
int clock_event_getEventDetails(const ClockEvent *event, int year, ClockEventDetails *eventDetails)
{
    NullCheck(event);
    NullCheck(eventDetails);

    if(clock_event_isDayOfMonthEvent(*event)) {
        Call(_getDetailsForDayOfMonthEvent(event, eventDetails));
    } else if(clock_event_isDayOfWeekEvent(*event)) {
        Call(_getDetailsForDayOfWeekEvent(event, eventDetails));
    } else if(clock_event_isDayOfYearEvent(*event)) {
        Call(_getDetailsForDayOfYearEvent(event, eventDetails));
    }
#ifdef PARAM_CHECKS
    else {
        OriginateErrorEx(ERANGE, "%d", "Unexpected event date init type");
    }
#endif

    return 0;
}

//
// @brief Calculates a number of days left to a given event
// @param dayOfYear a day of year from which the number of days should be calculated.
//                  Usually now() in days starting from the January, 1 this year.
// @param clockEvent a specific event to calculate the number of days to
// @param daysToEvent the number of days will be returned here
//
// @returns 0 on ok
//   EINVAL if clockEvent is NULL
//          if daysToEvent is NULL
//
// @note clockEvent should be of an existing day, (i.e. not 34 Feb 2000).
//       This function doesn't do correctness check.
//
int clock_event_daysToEvent(const unsigned short dayOfYear, const ClockEvent *clockEvent, int *daysToEvent)
{
    NullCheck(clockEvent);
    NullCheck(daysToEvent);
    if(dayOfYear > 366 || dayOfYear == 0) {
        OriginateErrorEx(EINVAL, "%d", "dayOfYear should be <= 366 and > 0");
    }


    return 0;
}

int clock_event_update(const DateTime *dt)
{
    NullCheck(dt);

    ClockEvent *event = (ClockEvent *)ClockEvents;
    for(size_t i = 0; i < CLOCK_EVENTS_SIZE; ++i, ++event) {
        Call( clock_event_updateEvent(event) );
    }

    return 0;
}

//
// @brief initializes a list of ClockEvents
//
int clock_event_initList(ClockEvent *eventsList, size_t sz, int year)
{
    NullCheck(events);
    NullCheck(dt);

    ClockEvent *event = eventsList;
    for(size_t i = 0; i < sz; ++i, ++event) {
        ClockEventDetails eventDetails;
        Call( clock_event_getEventDetails(event, year, &eventDetails) );
    }

    return 0;
}
