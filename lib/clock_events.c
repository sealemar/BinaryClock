//
// developed by Sergey Markelov (12/02/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "clock_events.h"

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
int clock_events_daysToEvent(const DateTime *dateTime, const ClockEvent *clockEvent, int *daysToEvent)
{
    NullCheck(dateTime);
    NullCheck(clockEvent);
    NullCheck(daysToEvent);



    return 0;
}
