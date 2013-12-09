//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_MAIN_H
#define BINARY_CLOCK_LIB_CLOCK_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "clock_state.h"

//
// @brief Initializes clockState
// @param clockState a structure which holds the entire state of the clock
// @note call this function before calling clock_update() for the first time
//
int clock_init(ClockState *clockState);

//
// @brief Call this function from the main loop
// @param clockState a structure which holds the entire state of the clock
// @returns 0 on success
// EINVAL - if clockState is NULL
//
// @note call clock_init() before calling clock_update() for the first time
//
int clock_update(ClockState *clockState);

#ifdef __cplusplus
}
#endif

#endif
