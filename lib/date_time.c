//
// developed by Sergey Markelov (11/19/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "date_time.h"

inline static int _daysInMonth(int month, int year)
{
    if(month < JANUARY) {
        --year;
        month = DECEMBER;
    }

    switch(month) {
        case JANUARY:
        case MARCH:
        case MAY:
        case JULY:
        case AUGUST:
        case OCTOBER:
        case DECEMBER:
            return 31;

        case FEBRUARY:
            return (year % 4 == 0) ? 29 : 28;

        default:
            return 30;
    };
}

inline static void _normalize(int *minor, int *major, int minorBase)
{
    if(*minor >= minorBase) {
        *major += *minor / minorBase;
        *minor %= minorBase;
    } else if(*minor < 0) {
        int t = (- *minor) / minorBase;
        *minor += t * minorBase;
        if(*minor < 0) {
            ++t;
            *minor += minorBase;
        }
        *major -= t;
    }
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
// @note When dealing with underflows, day 0 will mean previous month
//       last day. Day -1 will mean previous month, last day -1, i.e.
//
//       dt.month = FEBRUARY;
//       dt.day   = 0;
//
//       dateTime_normalize(&dt);
//
//       dt.month = JANUARY;
//       dt.day   = 31;
//
int dateTime_normalize(DateTime *dt)
{
#ifdef PARAM_CHECKS
    if(dt == NULL)
        OriginateErrorEx(EINVAL, "%d", "dt is NULL");
#endif

    _normalize(&(dt->millisecond), &(dt->second), 1000);
    _normalize(&(dt->second),      &(dt->minute),   60);
    _normalize(&(dt->minute),      &(dt->hour),     60);
    _normalize(&(dt->hour),        &(dt->day),      24);

    //
    // Now process month overflow and underflow.
    // Days will be processed afterwards, because they
    // depend on a month
    //
    _normalize(&(dt->month), &(dt->year), 12);

    //
    // Process day overflows
    //
    for(int d = _daysInMonth(dt->month, dt->year);
        dt->day > d;
        d = _daysInMonth(dt->month, dt->year)) {

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
    for(int d = _daysInMonth(dt->month - 1, dt->year);
        dt->day < 1;
        d = _daysInMonth(dt->month - 1, dt->year)) {

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
//       dateTime_addMillis() calls dateTime_normalize() internally.
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
