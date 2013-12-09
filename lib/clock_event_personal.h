//
// developed by Sergey Markelov (12/07/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H
#define BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H

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
// @warning make sure that event.name is lesser than
//
//          STATE_TEXT_SIZE - stren(" on " + date_time_dateToStr()) - 1
//          STATE_TEXT_SIZE - stren(" in %d days") - 1
//
//          event.name + " on " + date_time_dateToStr()
//          or
//          event.name + " in %d days"
//
extern ClockEvent ClockEvents[CLOCK_EVENTS_SIZE];

#endif
