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
// @brief Clock main functionality. The user code should call clock_init()
// once and then clock_update() repeatedly.
//
// @example see arduino/binary_clock.ino or emulator/main.c to understand how it works
//
// void setup()
// {
//     clock_clearScreen();
//     Call(clock_init(&clockState));
// }
//
// void loop()
// {
//     for(size_t i = 0; i < countof(buttons); ++i) {
//         Call(clock_button_press( &(clockState.buttons), i, digitalRead(buttons[i]) == LOW ? TRUE : FALSE));
//     }
//     Call(clock_update(&clockState));
// }
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
