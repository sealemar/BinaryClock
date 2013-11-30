//
// developed by Sergey Markelov (11/30/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EXTERN_FUNCTIONS_H
#define BINARY_CLOCK_LIB_CLOCK_EXTERN_FUNCTIONS_H

#include "date_time.h"

//
// @note the implementation needs to set clock_setPixel to point to a real function.
//       It is stubbed to NULL by default
// @brief switches one pixel on or off
// @param x x coordinate (0 < x < SCREEN_WIDTH)
// @param y y coordinate (0 < y < SCREEN_HEIGHT)
// @param turnOn if not 0, the pixel will be turned on
// @returns 0 on ok
// EINVAL - if _x_ or _y_ is out of range
//
extern int (* clock_setPixel)(int x, int y, Bool turnOn);

//
// @note the implementation needs to set clock_setPixel to point to a real function.
//       It is stubbed to NULL by default
// @param millis will be set to the clock uptime in milliseconds
// @returns 0 on ok
// implementation error if fails
//
// @note On Arduino that will be most likely millis() function.
//
extern int (* clock_uptimeMillis)(unsigned long *millis);

//
// @brief This pointer to a function is stubbed to NULL by default.
//        If it is not NULL, then it will be called from
//        clock_init(ClockState *) and ClockState->DateTime will be
//        initialized by this funciton. I.e. an implementation may
//        initialize the date and time to current date and time.
//
// @param dt &(clockState->dateTime) will be passed from clock_init(ClockState *)
//
// @returns 0 on ok
//
extern int (* clock_initDateTime)(DateTime *dt);

#endif
