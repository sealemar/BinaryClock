//
// developed by Sergey Markelov (11/30/2013)
//

//
// @brief This is personal events. Set it to whatever you want
//

#include "clock_event.h"
#include "clock_event_personal.h"

const ClockEvent ClockEvents[CLOCK_EVENTS_SIZE] = {
    clock_event_initDayOfMonth(1,  JANUARY,     0, "New year"),
    clock_event_initDayOfMonth(1,  APRIL,    1392, "Fool's day"),
    clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000, "System Administrator Appreciation Day"),
    clock_event_initDayOfYear (256, 2009, "Programmer's day"),
    clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, "Thanksgiving"),
    clock_event_initDayOfMonth(25, DECEMBER,    0, "Christmas"),
};
