//
// developed by Sergey Markelov (11/17/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_TIME_H
#define BINARY_CLOCK_LIB_CLOCK_TIME_H

#define MAX_UPTIME_MILLIS (~(0UL))

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

#endif
