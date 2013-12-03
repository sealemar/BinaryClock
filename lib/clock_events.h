//
// developed by Sergey Markelov (11/30/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENTS_FUNCTIONS_H
#define BINARY_CLOCK_LIB_CLOCK_EVENTS_FUNCTIONS_H

#define CLOCK_EVENTS_SIZE           10

#define DATE_TYPE_MONTH_DAY         1
#define DATE_TYPE_DAY_OF_YEAR       2
#define DATE_TYPE_MONTH_DAY_OF_WEEK 3

//
// @brief ClockEvent describes an event.
// Any event starts from a _year_. Then it may have _month_ and
// _dayOfMonth_ or _dayOfYear_ (the first two take precedence).
// Setting _dayOfYear_ may be needed, i.e. for the Programmer's Day
// http://en.wikipedia.org/wiki/Programmers'_Day
// which is 256th day of a year.
// The rest is calculated.
//
typedef struct {
    const int year;
    unsigned char dateType;
    unsigned char month;
    unsigned char dayOfMonth;
    unsigned short dayOfYear;
    const char *name;                   // event name
} ClockEvent;

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
extern const ClockEvent ClockEvents[CLOCK_EVENTS_SIZE];

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
int clock_events_daysToEvent(const unsigned short dayOfYear, const ClockEvent *clockEvent, int *daysToEvent);

void clock_events_init();

#endif
