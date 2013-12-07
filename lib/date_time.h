//
// developed by Sergey Markelov (11/19/2013)
//

#ifndef BINARY_CLOCK_LIB_DATE_TIME_H
#define BINARY_CLOCK_LIB_DATE_TIME_H

#define MILLIS_IN_A_DAY   86400000UL

#define HOURS_COUNT 24

// "00:00:00" + '\0'
#define DATE_TIME_TIME_STR_SIZE 9

// "MMM DD YYYY" + '\0'
#define DATE_TIME_DATE_STR_SIZE 12

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
} DateTime;

#define SUNDAY     0
#define MONDAY     1
#define TUESDAY    2
#define WEDNESDAY  3
#define THURSDAY   4
#define FRIDAY     5
#define SATURDAY   6

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

#define date_time_initDate(year, month, dayOfMonth) { year, month, dayOfMonth, 0, 0, 0, 0 }

//
// @brief three letter month
//
extern const char *DateTimeMonthsStr[];

//
// @brief day of week string
//
extern const char *DateTimeDayOfWeekStr[];

//
// @brief returns the number of days in a month
// @param year
// @param month
// @param daysInMonth the result will be returned here
// @returns 0 on ok
// EINVAL if _daysInMonth_ is NULL
// ERANGE if month < JANUARY or month > DECEMBER
//
int date_time_daysInMonth(int year, int month, int *daysInMonth);

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
int date_time_normalize(DateTime *dt);

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
int date_time_addMillis(DateTime *dt, unsigned long millis);

//
// @brief prints time from _dt_ to _str_ in format
//        hh:mm:ss
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
int date_time_timeToStr(const DateTime *dt, char str[DATE_TIME_TIME_STR_SIZE]);

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
int date_time_dateToStr(const DateTime *dt, char str[DATE_TIME_DATE_STR_SIZE]);

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
int date_time_calculateDayOfWeek(int year, int month, int day, int *dayOfWeek);

#endif
