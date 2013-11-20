//
// developed by Sergey Markelov (11/19/2013)
//

#ifndef BINARY_CLOCK_LIB_DATE_TIME_H
#define BINARY_CLOCK_LIB_DATE_TIME_H

#define MILLIS_IN_A_DAY   86400000UL

#define JANUARY    1
#define FEBRUARY   2
#define MARCH      3
#define APRIL      4
#define MAY        5
#define JUNE       6
#define JULY       7
#define AUGUST     8
#define SEPTEMBER  9
#define OCTOBER   10
#define NOVEMBER  11
#define DECEMBER  12

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
int dateTime_normalize(DateTime *dt);

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
int dateTime_addMillis(DateTime *dt, unsigned long millis);

#endif
