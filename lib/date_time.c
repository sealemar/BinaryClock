//
// developed by Sergey Markelov (11/19/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "date_time.h"

inline static int _daysInMonth(DateTime *dt)
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
// @brief Normalizes date and time in a given _dt_ by rolling
//        any excesses in milliseconds over to seconds, from seconds
//        over to minutes and so on. It also works correctly with
//        negative numbers.
//
// @param dt a pointer to an instance of DateTime
//
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int dateTime_normalize(DateTime *dt)
{
#ifdef PARAM_CHECKS
    if(dt == NULL)
        OriginateErrorEx(EINVAL, "%d", "dt is NULL");
#endif

    //
    // Process millisecond overflow and underflow
    //
    if(dt->millisecond >= 1000) {
        dt->second      += dt->millisecond / 1000;
        dt->millisecond %= 1000;
    } else if(dt->millisecond < 0) {
        int t = (dt->millisecond / 1000) + 1;
        dt->second += t;
        dt->millisecond -= t * 1000;
    }

    //
    // Process second overflow and underflow
    //
    if(dt->second >= 60) {
        dt->minute += dt->second / 60;
        dt->second %= 60;
    } else if(dt->second < 0) {
        int t = (dt->second / 60) + 1;
        dt->minute += t;
        dt->second -= t * 60;
    }

    //
    // Process minute overflow and underflow
    //
    if(dt->minute >= 60) {
        dt->hour   += dt->minute / 60;
        dt->minute %= 60;
    } else if(dt->minute < 0) {
        int t = (dt->minute / 60) + 1;
        dt->hour += t;
        dt->minute -= t * 60;
    }

    //
    // Process hour overflow and underflow
    //
    if(dt->hour >= 24) {
        dt->day  += dt->hour / 24;
        dt->hour %= 24;
    } else if(dt->hour < 0) {
        int t = (dt->hour / 24) + 1;
        dt->day += t;
        dt->hour -= t * 24;
    }

    //
    // Now process month overflow and underflow.
    // Days will be processed afterwards, because they
    // depend on a month
    //
    if(dt->month > DECEMBER) {
        int t = dt->month - 1;
        dt->year  += t / 12;
        dt->month = (t % 12) + 1;
    } else if(dt->month < JANUARY) {
        int t = ((dt->month - 1) / 12) + 1;
        dt->year  += t;
        dt->month -= t * 12 - 1;
    }

    //
    // Process day overflows
    //
    for(int d = _daysInMonth(dt); dt->day > d; d = _daysInMonth(dt)) {
        dt->day -= d;
        ++(dt->month);

        if(dt->month > DECEMBER) {
            dt->month = JANUARY;
            ++(dt->year);
        }
    }

    //
    // Process day underflows
    //
    for(int d = _daysInMonth(dt); dt->day < 1; d = _daysInMonth(dt)) {
        dt->day += d;
        --(dt->month);

        if(dt->month < JANUARY) {
            dt->month = DECEMBER;
            --(dt->year);
        }
    }

    return 0;
}

//
// @brief Adds a given number of milliseconds to DateTime
// @param dt a pointer to DateTime which needs to be updated
// @param millis a number of milliseconds to add to _dt_
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
// @note This function should be used on _dt_ after getting a delta of
//       uptime with lib/clock_time::clock_updateUptimeMillis()
//
int dateTime_addMillis(DateTime *dt, unsigned long millis)
{
#ifdef PARAM_CHECKS
    if(dt == NULL)
        OriginateErrorEx(EINVAL, "%d", "dt is NULL");
#endif

    //
    // Since _v_ is unsigned long and all of DateTime member variables
    // are int, first calculate days part, then start from the
    // milliseconds.
    //
    dt->day += millis / MILLIS_IN_A_DAY;
    millis %= MILLIS_IN_A_DAY;

    dt->millisecond += millis;

    //
    // Now normalize the result
    //
    int res = dateTime_normalize(dt);
#ifdef PARAM_CHECKS
    if(res) ContinueError(res, "%d");
#endif

    return res;
}
