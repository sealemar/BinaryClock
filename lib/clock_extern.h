// Copyright [2013] [Sergey Markelov]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// @brief External API. The variables should point to the appropriate functions
// implemented in the user code.
//
// developed by Sergey Markelov (11/30/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EXTERN_FUNCTIONS_H
#define BINARY_CLOCK_LIB_CLOCK_EXTERN_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "date_time.h"

//
// @note required
// @note the implementation needs to set clock_setPixel to point to a real function.
//       It is stubbed to NULL by default
// @brief switches one pixel on or off
// @param x x coordinate (0 < x < SCREEN_WIDTH)
// @param y y coordinate (0 < y < SCREEN_HEIGHT)
// @param turnOn if not 0, the pixel will be turned on
// @returns 0 on ok
// EINVAL - if _x_ or _y_ is out of range
//
extern int (* clock_extern_setPixel)(int x, int y, Bool turnOn);

//
// @note required
// @note the implementation needs to set clock_setPixel to point to a real function.
//       It is stubbed to NULL by default
// @param millis will be set to the clock uptime in milliseconds
// @returns 0 on ok
// implementation error if fails
//
// @note On Arduino that will be most likely millis() function.
//
extern int (* clock_extern_uptimeMillis)(unsigned long *millis);

//
// @note optional
// @brief The implementation may set this pointer to a function in which case
//        clock_clearScreen() will call it. It will call
//        clock_drawPattern(ClockAlphabet[CLOCK_BLANK])
//        otherwise
//
extern int (* clock_extern_clearScreen)(void);

//
// @note optional
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
extern int (* clock_extern_initDateTime)(DateTime *dt);

#ifdef __cplusplus
}
#endif

#endif
