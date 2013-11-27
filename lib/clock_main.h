//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_MAIN_H
#define BINARY_CLOCK_LIB_CLOCK_MAIN_H

#include "clock_state.h"
//
// @brief Call this function from the main loop
// @param clockState a structure which holds the entire state of the clock
// @returns 0 on success
// EINVAL - if clockState is NULL
//
int clock_update(ClockState *clockState);

#endif
