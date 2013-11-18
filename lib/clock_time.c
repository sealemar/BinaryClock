//
// developed by Sergey Markelov (11/17/2013)
//

#include "clock_time.h"

static unsigned long UptimeMillis = 0; // the number of milliseconds since the program has started

#define SECONDS_IN_A_DAY    86400

inline static unsigned int _daysInMonth(DateTime *dt)
{
    switch(dt->month) {
        case JANUARY:
        case MARCH:
        case MAY:
        case JULY:
        case AUGUST:
        case OCTOBER:
        case DECEMBER:
            return 31;

        case FEBRUARY:
            return (dt->year % 4 == 0) ? 29 : 28;

        default:
            return 30;
    };
}


//
// @brief Updates the program uptime to a given number of milliseconds.
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
// @param millis the current uptime in milliseconds
//
// @returns the number of milliseconds since this function was called the last time
//
unsigned long clock_updateUptimeMillis(unsigned long millis)
{
    if(UptimeMillis == millis) return 0;

    unsigned long delta = 0;

    if(millis < UptimeMillis) {
        // The number was overflown
        delta = ~(0UL) - UptimeMillis + 1 + millis;
    } else {
        delta = UptimeMillis - millis;
    }

    // Now update the uptime
    UptimeMillis = millis;

    return delta;
}

//
// @brief Adds a given number of milliseconds to DateTime
// @param millis a number of milliseconds to add to _dt_
// @param dt a pointer to DateTime which needs to be updated
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_addMillisToDateTime(unsigned long millis, DateTime *dt)
{
#ifdef PARAM_CHECKS
    if(dt == NULL)
        OriginateErrorEx(EINVAL, "%d", "dt is NULL");
#endif

    // Get seconds
    millis /= 1000;

    //
    // Since _v_ is unsigned long and all of DateTime member variables
    // are unsigned int, first calculate days part, then start from the
    // seconds.
    //
    dt->day += millis / SECONDS_IN_A_DAY;
    millis %= SECONDS_IN_A_DAY;

    dt->second += millis;
    if(dt->second >= 60) {
        dt->minute += dt->second / 60;
        dt->second %= 60;
    }
    if(dt->minute >= 60) {
        dt->hour   += dt->minute / 60;
        dt->minute %= 60;
    }
    if(dt->hour >= 24) {
        dt->day  += dt->hour / 24;
        dt->hour %= 24;
    }

    for(unsigned int d = _daysInMonth(dt); dt->day > d; d = _daysInMonth(dt)) {
        dt->day -= d;
        ++(dt->month);

        if(dt->month > 12) {
            dt->month = 1;
            ++(dt->year);
        }
    }

    return 0;
}
