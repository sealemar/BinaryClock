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
// @brief BinaryClock emulator buttons functionality
//
// developed by Sergey Markelov (11/28/2013)
//

#include <clock_button.h>

//
// @brief Checks the emulator buttons and updates _clockButtons_ if
//        a matching controling _ch_ is found for a button.
//
// @param clockButtons - a pointer to a structure to update
// @param ch that is what getch() returned
// @param matched if not NULL will be set to TRUE if ch was matched at least once
//
// @returns 0 on ok
//          EINVAL if _clockButtons_ is NULL
//
int emulator_button_press(ClockButtons *clockButtons, int ch, Bool *matched);

//
// @brief Updates the visual state of emulator's buttons
// @param clockState - the clock state object
// @returns 0 on ok
//          EINVAL if _clockButtons_ is NULL
//
int emulator_button_update(const ClockState *clockState);

int emulator_button_init();
int emulator_button_deinit();
