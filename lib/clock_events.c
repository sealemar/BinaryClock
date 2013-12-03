//
// developed by Sergey Markelov (12/02/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "date_time.h"
#include "clock_events.h"

static int clock_events_initDates()
{
    ClockEvent *event = (ClockEvent *)ClockEvents;
    for(size_t i = 0; i < CLOCK_EVENTS_SIZE; ++i, ++event) {
        switch(event->dateType) {
            case DATE_TYPE_MONTH_DAY:
                for(int month = event->month; month >= JANUARY; --month) {
                    date_time_daysInMonth();
                }
                break;
            case DATE_TYPE_DAY_OF_YEAR:
            case DATE_TYPE_MONTH_DAY_OF_WEEK:
#ifdef PARAM_CHECKS
            default:
                OriginateErrorEx(ERANGE, "%d", "event->dateType [%d] is not supported", event->dateType);
#endif
        }
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
int clock_events_daysToEvent(const unsigned short dayOfYear, const ClockEvent *clockEvent, int *daysToEvent)
{
    NullCheck(clockEvent);
    NullCheck(daysToEvent);
    if(dayOfYear > 366 || dayOfYear == 0) {
        OriginateErrorEx(EINVAL, "%d", "dayOfYear should be <= 366 and > 0");
    }


    return 0;
}

int clock_events_init()
{
}
