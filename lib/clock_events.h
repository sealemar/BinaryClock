//
// developed by Sergey Markelov (11/30/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENTS_FUNCTIONS_H
#define BINARY_CLOCK_LIB_CLOCK_EVENTS_FUNCTIONS_H

#include "date_time.h"

typedef struct {
    const int year;         // if set to CLOCK_EVENT_NO_START_YEAR, the event won't count years
    const int month;
    const int day;
    const char *name;       // event name
} ClockEvent;

//
// @brief ClockEvents is a list of all events of which the clock knows.
//        These can be personalized events, such as birthdays and other
//        special days.
//
// @note This array is defined in lib/clock_events_personal.c
//
extern const ClockEvent ClockEvents[];

//
// @brief Calculates a number of days left to a given event
// @param dateTime date and time from which the number of days should be calculated
// @param clockEvent a specific event to calculate the number of days to
// @param daysToEvent the number of days will be returned here
//
// @returns 0 on ok
//   EINVAL if dateTime is NULL
//          if clockEvent is NULL
//          if daysToEvent is NULL
//
// @note dateTime should be normalized.
//       clockEvent should be of an existing day, (i.e. not 34 Feb 2000).
//       This function doesn't do correctness check.
//
int clock_events_daysToEvent(const DateTime *dateTime, const ClockEvent *clockEvent, int *daysToEvent);

#endif
