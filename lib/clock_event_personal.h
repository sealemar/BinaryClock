//
// developed by Sergey Markelov (12/07/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H
#define BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_EVENTS_SIZE 6

#include "clock_event.h"

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
// @warning make sure, that ClockEvent.name is not longer than
//          EVENT_STRING_BUFFER_SIZE - 4 - DATE_TIME_DATE_STR_SIZE
//
extern ClockEvent ClockEvents[CLOCK_EVENTS_SIZE];

#ifdef __cplusplus
}
#endif

#endif
