//
// developed by Sergey Markelov (11/17/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_TIME_H
#define BINARY_CLOCK_LIB_CLOCK_TIME_H

#define MAX_UPTIME_MILLIS (~(0UL))

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
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} DateTime;

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
unsigned long clock_updateUptimeMillis(unsigned long millis);

//
// @brief Adds a given number of milliseconds to DateTime
// @param millis a number of milliseconds to add to _dt_
// @param dt a pointer to DateTime which needs to be updated
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_addMillisToDateTime(unsigned long millis, DateTime *dt);

#endif
