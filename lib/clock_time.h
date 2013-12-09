//
// developed by Sergey Markelov (11/17/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_TIME_H
#define BINARY_CLOCK_LIB_CLOCK_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "clock_state.h"

#define MAX_UPTIME_MILLIS (~(0UL))

//
// @brief Updates the program uptime to a given number of milliseconds.
// @param millis the current uptime in milliseconds
// @param lastMillis time when this function was last called
// @param delta the delta between clockState->lastUptime and millis will be returned here
// @returns 0 if success
// EINVAL if lastMillis is NULL
//        if delta is NULL
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
int clock_updateUptimeMillis(unsigned long millis, unsigned long *lastMillis, unsigned long *delta);

#ifdef __cplusplus
}
#endif

#endif
