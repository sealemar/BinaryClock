//
// developed by Sergey Markelov (11/30/2013)
//

//
// @brief This is personal events. Set it to whatever you want
//

#include "clock_event.h"

const ClockEvent ClockEvents[CLOCK_EVENTS_SIZE] = {
    clock_event_initDayOfMonth(1,  JANUARY,     0, "New year"),
    clock_event_initDayOfMonth(4,  APRIL,    1392, "Fool's day"),
    clock_event_initDayOfYear (256, 2009, "Programmer's day"),
    clock_event_initDayOfWeek (THURSDAY, 4, TRUE, NOVEMBER, 1574, "Thanksgiving"),
    clock_event_initDayOfMonth(25, DECEMBER,    0, "Christmas"),
};