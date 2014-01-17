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
// @brief BinaryClock emulator main functions
//
// developed by Sergey Markelov (11/13/2013)
//

#ifndef BINARY_CLOCK_EMULATOR_EMULATOR_H
#define BINARY_CLOCK_EMULATOR_EMULATOR_H

#include <clock_state.h>

#define PIXEL_ON   'o'
#define PIXEL_OFF  '-'
#define COLOR_ON     1
#define COLOR_OFF    2

int emulator_init();

//
// @brief call this function from the main loop
//
int emulator_update(const ClockState *cs);

//
// @brief If clock_init() was called, this function must be called
//        to deinitialize and free resources.
//
int emulator_deinit();

#endif
