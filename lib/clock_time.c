//
// developed by Sergey Markelov (11/17/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "clock_time.h"

//
// @brief Updates the program uptime to a given number of milliseconds.
// @param millis the current uptime in milliseconds
// @param lastMillis time when this function was last called
// @param delta the delta between clockState->lastUptime and millis will be returned here
// @returns 0 if success
// EINVAL if lastMillis is NULL
//        if delta is NULL
//
// @note This function handles integer overflows.
//
//       From: http://arduino.cc/en/Reference/Millis :
//
//       millis()
//
//       Description:
//       Returns the number of milliseconds since the Arduino board began running the current program.
//       This number will overflow (go back to zero), after approximately 50 days.
//
int clock_updateUptimeMillis(unsigned long millis, unsigned long *lastMillis, unsigned long *delta)
{
    NullCheck(lastMillis);
    NullCheck(delta);

    *delta = 0;

    if(*lastMillis == millis) return 0;

    if(millis < (*lastMillis)) {
        // The number was overflown
        *delta = MAX_UPTIME_MILLIS - (*lastMillis) + 1 + millis;
    } else {
        *delta = millis - (*lastMillis);
    }

    // Now update the uptime
    *lastMillis = millis;

    return 0;
}
