//
// developed by Sergey Markelov (12/02/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "date_time.h"
#include "clock_event.h"

#define clock_event_setDayOfWeek(event, dayOfWeek) { \
    (event).blob_2 &= ~(7 << 4); \
    (event).blob_2 |= (dayOfWeek & 7) << 4; \
}

static inline int _updateDayOfMonthEvent(ClockEvent *event)
{
    int dayOfWeek;

    Call(date_time_calculateDayOfWeek(
                event->year,
                clock_event_getMonth(*event),
                clock_event_getDayOfMonth(*event),
                &dayOfWeek));

    clock_event_setDayOfWeek(*event, dayOfWeek);

    return 0;
}

static int clock_event_initDates()
{
    ClockEvent *event = (ClockEvent *)ClockEvents;
    for(size_t i = 0; i < CLOCK_EVENTS_SIZE; ++i, ++event) {
        if(clock_event_isDayOfMonthEvent(*event)) {
            Call(_updateDayOfMonthEvent(event));
        } else if(clock_event_isDayOfWeekEvent(*event)) {
        } else if(clock_event_isDayOfYearEvent(*event)) {
        }
#ifdef PARAM_CHECKS
        else {
            OriginateErrorEx(ERANGE, "%d", "Unexpected event date init type");
        }
#endif
    }

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

int clock_event_init()
{
    Call(clock_event_initDates());

    return 0;
}
