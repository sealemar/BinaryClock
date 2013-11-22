//
// developed by Sergey Markelov (11/19/2013)
//

#ifndef BINARY_CLOCK_LIB_DATE_TIME_H
#define BINARY_CLOCK_LIB_DATE_TIME_H

#define MILLIS_IN_A_DAY   86400000UL

#define JANUARY    0
#define FEBRUARY   1
#define MARCH      2
#define APRIL      3
#define MAY        4
#define JUNE       5
#define JULY       6
#define AUGUST     7
#define SEPTEMBER  8
#define OCTOBER    9
#define NOVEMBER  10
#define DECEMBER  11

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
} DateTime;

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
int dateTime_normalize(DateTime *dt);

//
// @brief Adds a given number of milliseconds to DateTime
// @param dt a pointer to DateTime which needs to be updated
// @param millis a number of milliseconds to add to _dt_
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
// @note This function should be used on _dt_ after getting a delta of
//       uptime with lib/clock_time::clock_updateUptimeMillis().
//       dateTime_addMillis() calls dateTime_normalize() internally.
//
int dateTime_addMillis(DateTime *dt, unsigned long millis);

#endif
