//
// developed by Sergey Markelov (11/19/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include <string.h>

#include "date_time.h"

const char *DateTimeMonthsStr[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

const char *DateTimeDayOfWeekStr[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

//
// @brief Converts fixed width int to string
// @param n an integer to convert
// @param str char* where to output the result
// @param nWidth how many least significant digits to convert
//
// @note The result will be zero padded on the left if len(str(n)) < nWidth.
//       Negative n is not supported
//
#define fixedWidthIntToString(n, str, nWidth) { \
    for(int _i = ((nWidth) - 1), _t = n; \
        _i >= 0; \
        --_i, _t /= 10) { \
\
        *((str) + _i) = (_t % 10) + '0'; \
    } \
}

static int _daysInMonth(int month, int year)
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

static int _dayOfWeek(int year, int month, int day)
{
    //
    // Gauss's algorithm
    // to determine day of week in Gregorian Calendar
    // http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Formulas_derived_from_Gauss.27s_algorithm
    //

    month -= 2;
    if(month < JANUARY) {
        --year;
        month = (DECEMBER - JANUARY) + 1 - (JANUARY - month);
    }
    unsigned int d = month + 1;
    d = (13 * d - 1) / 5;              // [ 2.6 * d - 0.2 ] --- from Gauss's algorithm
    d += 5 * (year % 4);
    d += 4 * (year % 100);
    d += 6 * (year % 400);
    d += day;
    d %= 7;

    return d;
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
// @brief returns the number of days in a month
// @param year
// @param month
// @param daysInMonth the result will be returned here
// @returns 0 on ok
// EINVAL if _daysInMonth_ is NULL
// ERANGE if month < JANUARY or month > DECEMBER
//
int date_time_daysInMonth(int year, int month, int *daysInMonth)
{
    NullCheck(daysInMonth);
#ifdef PARAM_CHECKS
    if(month < JANUARY || month > DECEMBER)
        OriginateErrorEx(ERANGE, "%d", "month [%d] should be >= %d and =< %d", month, JANUARY, DECEMBER);
#endif

    *daysInMonth = _daysInMonth(month, year);

    return 0;
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
int date_time_normalize(DateTime *dt)
{
    NullCheck(dt);

    _normalize(&(dt->millisecond), &(dt->second), 1000);
    _normalize(&(dt->second),      &(dt->minute),   60);
    _normalize(&(dt->minute),      &(dt->hour),     60);
    _normalize(&(dt->hour),        &(dt->day), HOURS_COUNT);

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
int date_time_addMillis(DateTime *dt, unsigned long millis)
{
    NullCheck(dt);

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
    int res = date_time_normalize(dt);
#ifdef PARAM_CHECKS
    if(res) ContinueError(res, "%d");
#endif

    return res;
}

//
// @brief prints time from _dt_ to _str_ in format
//        hh:mm:ss
//
// @param dt a pointer to DateTime structure which time needs to be printed
// @param str a buffer where to print the time. Should be not less than DATE_TIME_TIME_STR_SIZE chars long
// @returns 0 on ok
//
// EINVAL if _dt_ is NULL
//        if _str_ is NULL
//
// ERANGE if dt->hour < 0 or dt->hour >= 24
//        if dt->minute < 0 or dt->minute >= 60
//        if dt->second < 0 or dt->second >= 60
//
int date_time_timeToStr(const DateTime *dt, char str[DATE_TIME_TIME_STR_SIZE])
{
    NullCheck(dt);
    NullCheck(str);
#ifdef PARAM_CHECKS
    if(dt->hour < 0 || dt->hour >= HOURS_COUNT) {
        OriginateErrorEx(ERANGE, "%d", "dt->hour = [%d] should be >= 0 and < %d", dt->hour, HOURS_COUNT);
    }
    if(dt->minute < 0 || dt->minute >= 60) {
        OriginateErrorEx(ERANGE, "%d", "dt->minute = [%d] should be >= 0 and < 60", dt->minute);
    }
    if(dt->second < 0 || dt->second >= 60) {
        OriginateErrorEx(ERANGE, "%d", "dt->second = [%d] should be >= 0 and < 60", dt->second);
    }
#endif

    int t = dt->hour < 10 ? 1 : 2;
    fixedWidthIntToString(dt->hour, str, t);
    str[t] = ':';
    ++t;
    fixedWidthIntToString(dt->minute, str + t, 2);
    t += 2;
    str[t] = ':';
    ++t;
    fixedWidthIntToString(dt->second, str + t, 2);
    t += 2;
    str[t] = 0;

    return 0;
}

//
// @brief prints date from _dt_ to _str_ in format
//        MMM DD YYYY
// @param dt a pointer to DateTime structure which time needs to be printed
// @param str a buffer where to print the time. Should be not less than DATE_TIME_DATE_STR_SIZE chars long
// @returns 0 on ok
//
// EINVAL if _dt_ is NULL
//        if _str_ is NULL
//
// ERANGE if dt->year < 0 or dt->year > 9999
//        if dt->month < JANUARY or dt->month > DECEMBER
//        if dt->day <= 0 or dt->day > daysInMonth(dt->month)
//
int date_time_dateToStr(const DateTime *dt, char str[DATE_TIME_DATE_STR_SIZE])
{
    NullCheck(dt);
    NullCheck(str);
#ifdef PARAM_CHECKS
    if(dt->year < 0 || dt->year > 9999) {
        OriginateErrorEx(ERANGE, "%d", "dt->year = [%d] should be >= 0 and <= 9999", dt->year);
    }
    if(dt->month < JANUARY || dt->month > DECEMBER) {
        OriginateErrorEx(ERANGE, "%d", "dt->month = [%d] should be >= 0 and < 11", dt->month);
    }
    int _d = _daysInMonth(dt->month, dt->year);
    if(dt->day <= 0 || dt->day > _d) {
        OriginateErrorEx(ERANGE, "%d", "dt->day = [%d] should be > 0 and <= %d", dt->day, _d);
    }
#endif

    memcpy(str, DateTimeMonthsStr[dt->month], 3);
    char *s = str + 3;
    *s = ' ';
    ++s;
    fixedWidthIntToString(dt->day, s, 2);
    s += 2;
    *s = ' ';
    ++s;
    fixedWidthIntToString(dt->year, s, 4);
    s += 4;
    *s = 0;

    return 0;
}

//
// @brief calculates a day of week for a given date
// @param year
// @param month
// @param day
// @returns 0 on ok
// EINVAL if _dayOfWeek_ is NULL
// ERANGE if _month_ < JANUARY or _month_ > DECEMBER
//        if _day_ < 0 or _day_ > daysInMonth(month)
//
// @note the function doesn't check the date correctness
//
int date_time_calculateDayOfWeek(int year, int month, int day, int *dayOfWeek)
{
    NullCheck(dayOfWeek);
#ifdef PARAM_CHECKS
    if(month < JANUARY || month > DECEMBER) {
        OriginateErrorEx(ERANGE, "%d", "month = [%d] should be >= 0 and < 11", month);
    }
    int _d = _daysInMonth(month, year);
    if(day <= 0 || day > _d) {
        OriginateErrorEx(ERANGE, "%d", "day = [%d] should be > 0 and <= %d", day, _d);
    }
#endif

    *dayOfWeek = _dayOfWeek(year, month, day);

    return 0;
}
