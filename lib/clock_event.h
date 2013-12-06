//
// developed by Sergey Markelov (11/30/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENT_FUNCTIONS_H
#define BINARY_CLOCK_LIB_CLOCK_EVENT_FUNCTIONS_H

#include "date_time.h"

//
// @brief ClockEvent describes an event.
// @param year a year of the event
// @param name a name of the event
// @param blob_1
//
//          byte #2     byte #1
//        00 00 0000  000 0 0000
//       |NW| Day of year| DoM  |
//             DoW flag
//
//    NW  - Number of week in the month. 00 - first week, 01 - second, 10 - third, 11 - fourth
//
//    Day of year / DoW indicator -
//          0 - the event is specified by day and month
//          DAY_OF_WEEK_FLAG (0x1ff) - the event is specified by day of week, week, and month
//          1..356 - the event is specified by day of year
//
//    DoM - Day of the month
//
// @param blob_2
//
//        0 000 0000
//        B|DoW|Mnth|
//
//    B    - Whether the number of week (NW above) counts from the beginning
//           of the month. 0 - from the beginning, 1 - from the end
//           If from the end, then NW = 00 mean the last week, 01 - the last, but one, etc.
//
//    DoW  - Day of the week
//    Mnth - Month
//
// @note
// Setting day of year may be needed, i.e. for the Programmer's Day
// http://en.wikipedia.org/wiki/Programmers'_Day
// which is 256th day of a year.
//
// Setting number of week and day of week may be needed i.e. for Thanksgiving
// http://en.wikipedia.org/wiki/Thanksgiving
// which is the fourth Thursday of November in the USA.
//
// Setting B may be needed i.e. for Nevada Day
// http://en.wikipedia.org/wiki/Nevada_Day
// which is the last Friday of October
//
// @note
// There are three options to specify the date for an event:
//
// 1. Set day of year
// 2. Set { B, NW, DoW, month }
// 3. Set month and day
//
// Idealy, only one of the three above should be used, but if more than one is specified,
// then the priority goes from the first which is the highest to the third which is the lowest.
// i.e, you set
// Day of year AND { B, NW, DoW, month }, then Day of year will take precedence.
//
// @warning Don't work with blob_1 and blob_2 directly, use helper macros below
//
typedef struct {
    const int   year;
    uint16_t    blob_1;
    uint8_t     blob_2;
    const char *name;                   // event name
} ClockEvent;

typedef struct {
    int month;
    int dayOfWeek;
    int dayOfMonth;
} ClockEventDetails;

#define DAY_OF_WEEK_FLAG 0x01ff

//
// @brief ClockEvents is a list of all events of which the clock knows.
//        These can be personalized events, such as birthdays and other
//        special days.
//
// @note This array is defined in lib/clock_events_personal.c
//
// @note clock_events_init() needs to be called at the program startup and then
// every time when the year changes.
//
#define CLOCK_EVENTS_SIZE 10
extern const ClockEvent ClockEvents[CLOCK_EVENTS_SIZE];

//
// @brief Initilizer for an event which is set with month and day
// @param day day of month
// @param month month of the event
// @param year year when the event first occured
// @param name name of the event
//
#define clock_event_initDayOfMonth(day, month, year, name) { year, (day & 0x1f), (month & 0xf), name }

//
// @brief Initilizer for an event which is set with
// day of week, week of month, and month
// @param dayOfWeek day of week. See macros in lib/date_time.h
// @param weekOfMonth week of the month. [1..4]
// @param fromBeginningOfMonth whether the _weekOfMonth_ should be counted from the beginning of the month
//          This can be any number or TRUE / FALSE which can be treated in the boolean context
//
// @param month month of the event
// @param year year when the event first occured
// @param name name of the event
//
#define clock_event_initDayOfWeek(dayOfWeek, weekOfMonth, fromBeginningOfMonth, month, year, name) { \
    year, \
    ((((weekOfMonth - 1) & 3) << 14) | DAY_OF_WEEK_FLAG << 5), \
    (((fromBeginningOfMonth ? 1 : 0) << 7) | (((dayOfWeek) & 7) << 4) | (month & 0x0f)), \
    name }

//
// @brief Initilizer for an event which is set with day of the year
// @param dayOfYear day of year
// @param year year when the event first occured
// @param name name of the event
//
#define clock_event_initDayOfYear(dayOfYear, year, name) { year, (((dayOfYear) & 0x01ff) << 5), 0, name }

//
// @brief These are the helper macros to get the date information from an event.
//
// @param event a dereferenced ClockEvent (not a pointer to)
//
// @returns the requested date information
//
// @warning Always use these macros to get the date, don't do bit twiddling yourlesf, since
//          the implementation may change.
//
#define clock_event_getDayOfMonth(event)          ( (event).blob_1 & 0x1f )
#define clock_event_getMonth(event)               ( (event).blob_2 & 0x0f )
#define clock_event_getDayOfYear(event)           ( ((event).blob_1 >> 5) & 0x01ff )
#define clock_event_getDayOfWeek(event)           ( ((event).blob_2 >> 4) & 7 )
#define clock_event_getWeekOfMonth(event)         ( ((event).blob_1 >> 14) & 3 )
#define clock_event_isFromBeginningOfMonth(event) ( ((event).blob_2 >> 7) & 1 )

//
// @brief These are the helper macros to check whether an event is specified by
//        1. day and month
//        2. day of week, week, and month
//        3. day of year
//
// @note clock_event_init() function calculates and sets some information in event, these
//       macros identify what an event was set up initially with.
//
// @returns boolean TRUE if the event is what macro name says
//
#define clock_event_isDayOfMonthEvent(event) ( clock_event_getDayOfYear(event) == 0 )
#define clock_event_isDayOfWeekEvent(event)  ( clock_event_getDayOfYear(event) == DAY_OF_WEEK_FLAG )
#define clock_event_isDayOfYearEvent(event) \
    ( clock_event_getDayOfYear(event) && clock_event_getDayOfYear(event) != DAY_OF_WEEK_FLAG )

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
int clock_event_daysToEvent(const unsigned short dayOfYear, const ClockEvent *clockEvent, int *daysToEvent);

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
int clock_event_getEventDetails(const ClockEvent *event, int year, ClockEventDetails *eventDetails);

//
// @brief
//
int clock_event_updateList(ClockEvent *eventsList, size_t sz, const DateTime *dt);

//
// @brief initializes a list of ClockEvents
//
int clock_event_initList(ClockEvent *eventsList, size_t sz, int year);

#define clock_event_init(dateTime) clock_event_initList((ClockEvent *)ClockEvents, CLOCK_EVENTS_SIZE, dateTime)
#define clock_event_update(dateTime) clock_event_updateList((ClockEvent *)ClockEvents, CLOCK_EVENTS_SIZE, dateTime)

#endif
