//
// developed by Sergey Markelov (12/02/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include <string.h>
#include <stdlib.h> // for qsort()

#include "clock_event.h"

#define clock_event_setDayOfWeek(event, dayOfWeek) { \
    (event).blob_2 &= ~(7 << 4); \
    (event).blob_2 |= (dayOfWeek & 7) << 4; \
}

#define clock_event_setDayOfMonth(event, dayOfMonth) { \
    (event).blob_1 &= ~(0x1f); \
    (event).blob_1 |= dayOfMonth & 0x1f; \
}

#define clock_event_setMonth(event, month) { \
    (event).blob_2 &= ~(0x0f); \
    (event).blob_2 |= month & 0x0f; \
}

#define clock_event_setEventDetails(event, eventDetails, year) { \
    clock_event_setDayOfWeek((event), (eventDetails).dayOfWeek); \
    clock_event_setDayOfMonth((event), (eventDetails).dayOfMonth); \
    clock_event_setMonth((event), (eventDetails).month); \
    (event).yearCalculated = year; \
}

#define clock_event_isBefore(event, month, day) ( \
    clock_event_getMonth(event) < (month) \
 || ( clock_event_getMonth(event) == (month) && clock_event_getDayOfMonth(event) < (day) ) \
)

#define clock_event_detailsIsBefore(eventDetails, monthOther, dayOther) ( \
    (eventDetails).month < monthOther \
 || ( (eventDetails).month == monthOther && (eventDetails).dayOfMonth < dayOther ) \
)

//
// @brief qsort() comparator for ClockEvent
//
static int _clock_event_compare(const void *l, const void *r)
{
    ClockEvent *lhs = (ClockEvent *)l;
    ClockEvent *rhs = (ClockEvent *)r;

    uint_fast8_t lMonth = clock_event_getMonth(*lhs);
    uint_fast8_t rMonth = clock_event_getMonth(*rhs);
    uint_fast8_t lDay = clock_event_getDayOfMonth(*lhs);
    uint_fast8_t rDay = clock_event_getDayOfMonth(*rhs);

    if(lMonth < rMonth) return -1;
    if(lMonth == rMonth) {
        if(lDay < rDay) return -1;
        if(lDay == rDay) return 0;
    }

    return 1;
}

static inline int _getDetailsForDayOfMonthEvent(const ClockEvent *event, int year, ClockEventDetails *eventDetails)
{
    eventDetails->month = clock_event_getMonth(*event);
    eventDetails->dayOfMonth = clock_event_getDayOfMonth(*event);

    Call( date_time_calculateDayOfWeek(
                year,
                eventDetails->month,
                eventDetails->dayOfMonth,
                &(eventDetails->dayOfWeek) ));

    return 0;
}

static inline int _getDetailsForDayOfWeekEvent(const ClockEvent *event, int year, ClockEventDetails *eventDetails)
{
    int_fast8_t month = clock_event_getMonth(*event);
    int_fast8_t dayOfWeek = clock_event_getDayOfWeek(*event);
    int_fast8_t weekNumber = clock_event_getWeekOfMonth(*event);
    int lastDayOfMonth;
    int dayOfMonth = 0;
    Call( date_time_daysInMonth(year, month, &lastDayOfMonth) );

    if(clock_event_isFromBeginningOfMonth(*event)) {
        int lastDayOfWeek;
        Call( date_time_calculateDayOfWeek(year, month, 1, &lastDayOfWeek) );
        if(lastDayOfWeek > dayOfWeek) {
            ++weekNumber;
        }
        dayOfMonth = 1 + weekNumber * 7 + (dayOfWeek - lastDayOfWeek);
    } else {
        int lastDayOfWeek;
        Call( date_time_calculateDayOfWeek(year, month, lastDayOfMonth, &lastDayOfWeek) );
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

static inline int _getDetailsForDayOfYearEvent(const ClockEvent *event, int year, ClockEventDetails *eventDetails)
{
    int d = clock_event_getDayOfYear(*event);
    int daysInMonth = 0;
    int_fast8_t month = JANUARY - 1;

    while(d > daysInMonth) {
        ++month;
        d -= daysInMonth;
        Call( date_time_daysInMonth(year, month, &daysInMonth) );
    }

    eventDetails->dayOfMonth = d;
    eventDetails->month = month;

    Call( date_time_calculateDayOfWeek(
                year,
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
        Call(_getDetailsForDayOfMonthEvent(event, year, eventDetails));
    } else if(clock_event_isDayOfWeekEvent(*event)) {
        Call(_getDetailsForDayOfWeekEvent(event, year, eventDetails));
    } else if(clock_event_isDayOfYearEvent(*event)) {
        Call(_getDetailsForDayOfYearEvent(event, year, eventDetails));
    }
#ifdef PARAM_CHECKS
    else {
        OriginateErrorEx(ERANGE, "%d", "Unexpected event date init type");
    }
#endif

    return 0;
}

//
// @brief prints an event to string
// @param event
// @param str a string to output to
// @returns 0 on ok
//   EINVAL if _event_ is NULL
//          if _str_ is NULL
//
//   EOVERFLOW if _strSize_ is to small
//
int clock_event_toStr(const ClockEvent *event, char str[EVENT_STRING_BUFFER_SIZE])
{
    NullCheck(event);
    NullCheck(str);

    size_t nameLen = strlen(event->name);

    strcpy(str, event->name);
    str += nameLen;
    strcpy(str, " - ");
    str += 3;

    DateTime dt = date_time_initDate(event->yearCalculated, clock_event_getMonth(*event), clock_event_getDayOfMonth(*event));
    Call( date_time_dateToStr(&dt, str) );

    return 0;
}

//
// @brief Updates a list of ClockEvents by setting the missing date/time parts.
//        This function should be called on a list of events every time the day changes.
//        In addition this function should be called after clock_event_initList().
//        It updates and then sorts _eventsList_ such that if an event comes before
//        dateTime->day / dateTime->month than it will be treated like the next year event.
//        Any other event will be treated as the same to dateTime->year event.
//
// @param eventsList a pointer to an array of ClockEvent
// @param sz the number of elements in _eventsList_
// @param dateTime a pointer to DateTime for which the event list should be updated
//
// @returns 0 on ok
//   EINVAL if _eventsList_ is NULL
//
int clock_event_updateList(ClockEvent *eventsList, size_t sz, const DateTime *dateTime)
{
    NullCheck(eventsList);
    NullCheck(dateTime);

    if(sz == 0) return 0;

    Bool wasChanged = FALSE;

    //
    // See if anything needs to be updated
    //
    int nextYear = dateTime->year + 1;
    ClockEvent *event = eventsList;
    for(size_t i = 0; i < sz; ++i, ++event) {

        // don't recalculate the event if it has been already updated to the next year
        if(event->yearCalculated == nextYear) {
            continue;
        }

        if(clock_event_isBefore(*event, dateTime->month, dateTime->day)) {
            ClockEventDetails eventDetails;
            CallEx( clock_event_getEventDetails(event, nextYear, &eventDetails),
                    "on eventsList[%zu], size %zu", i, sz);

            if(clock_event_detailsIsBefore(eventDetails, dateTime->month, dateTime->day)) {
                clock_event_setEventDetails(*event, eventDetails, nextYear);
                wasChanged = TRUE;
            }
        }
    }

    //
    // Sort the events list if something was changed
    //
    if(wasChanged) {
        qsort(eventsList, sz, sizeof(ClockEvent), _clock_event_compare);
    }

    return 0;
}

//
// @brief initializes a list of ClockEvents by setting the missing date/time parts
// @param eventsList a pointer to an array of ClockEvent
// @param sz the number of elements in _eventsList_
// @param year a year to init the list to
//
// @returns 0 on ok
//   EINVAL if _eventsList_ is NULL
//
int clock_event_initList(ClockEvent *eventsList, size_t sz, int year)
{
    NullCheck(eventsList);

    if(sz == 0) return 0;

    ClockEvent *event = eventsList;
    for(size_t i = 0; i < sz; ++i, ++event) {
        ClockEventDetails eventDetails;
        CallEx( clock_event_getEventDetails(event, year, &eventDetails),
                "on eventsList[%zu], size %zu", i, sz);
        clock_event_setEventDetails(*event, eventDetails, year);
    }

    return 0;
}

//
// @brief finds the next closest to _month_ and _dayOfMonth_ event from _eventsList_
// @param eventsList
// @param sz the number of elements in _eventsList_
// @param month
// @param dayOfMonth
// @param index the result will be returned here
// @returns 0 on ok
//   EINVAL if _eventsList_ is NULL
//   EINVAL if _index_ is NULL
//   ERANGE if _month_ is < JANUARY of _month_ is > DECEMBER
//   ERANGE if _dayOfMonth_ is < 1 or _dayOfMonth_ is > daysInMonth(month)
//
int clock_event_findClosestFromList(const ClockEvent *eventsList, size_t sz, int month, int dayOfMonth, int *index)
{
    NullCheck(eventsList);
    NullCheck(index);
#if PARAM_CHECKS
    if(month < JANUARY || month > DECEMBER) {
        OriginateErrorEx(ERANGE, "%d", "month = [%d] should be >= %d and < %d", month, JANUARY, DECEMBER);
    }
    int _d;
    Call( date_time_daysInMonth(0, month, &_d) ); // leap year to account for February 29
    if(dayOfMonth < 1 || dayOfMonth > _d) {
        OriginateErrorEx(ERANGE, "%d", "dayOfMonth = [%d] should be > 0 and <= %d", dayOfMonth, _d);
    }
#endif

    *index = 0;

    if(sz == 0) return 0;

    const ClockEvent *event = eventsList;
    for(size_t i = 0; i < sz; ++i, ++event) {
        if( ! clock_event_isBefore(*event, month, dayOfMonth)) {
            *index = i;
            break;
        }
    }

    return 0;
}
