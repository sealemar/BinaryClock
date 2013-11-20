//
// developed by Sergey Markelov (11/17/2013)
//

#include "clock_time.h"

static unsigned long UptimeMillis = 0; // the number of milliseconds since the program has started

//
// @brief Updates the program uptime to a given number of milliseconds.
// @param millis the current uptime in milliseconds
// @returns the number of milliseconds since this function was called the last time
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
unsigned long clock_updateUptimeMillis(unsigned long millis)
{
    if(UptimeMillis == millis) return 0;

    unsigned long delta = 0;

    if(millis < UptimeMillis) {
        // The number was overflown
        delta = MAX_UPTIME_MILLIS - UptimeMillis + 1 + millis;
    } else {
        delta = millis - UptimeMillis;
    }

    // Now update the uptime
    UptimeMillis = millis;

    return delta;
}
