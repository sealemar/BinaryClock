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
// developed by Sergey Markelov (11/27/2013)
//
// @brief A functionality to work with BinaryClock buttons.
// Generally a button should be pressed to interact with the clock
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "clock_button.h"

//
// @brief Call this function to press or release a button
//
// @note  Calling this function on the same _index_ with the same _isPressed_ state
//        clears the switch state of a button. Calling the function with different
//        _isPressed_ state on the same _index_ sets the switch state of a button.
//        Consider that while calling clock_button_isClicked()
//
// @param clockButtons a pointer to ClockButtons
// @param index which button to press/release
// @param isPressed true if the button needs to be pressed
// @returns 0 on ok
// EINVAL - if _clockButtons_ is NULL
// ERANGE - if _index_ is greater than the maximum
//
int clock_button_press(ClockButtons *clockButtons, size_t index, Bool isPressed)
{
    NullCheck(clockButtons);
#ifdef PARAM_CHECKS
    if(index >= CLOCK_BUTTON_MAX_COUNT)
        OriginateErrorEx(ERANGE, "%d", "index should be less than %d", CLOCK_BUTTON_MAX_COUNT);
#endif

    //
    // Process switch state
    //
    if(!!clock_button_isPressed(*clockButtons, index) != !!isPressed) {
        // the state was changed
        clockButtons->buttons |= 1 << (index + _SWITCH_STATE_PART);
    } else {
        // the state was not changed
        clockButtons->buttons &= ~(1 << (index + _SWITCH_STATE_PART));
        return 0;
    }

    //
    // Process pressed state
    //
    if(isPressed) {
        clockButtons->buttons |= 1 << index;
    } else {
        clockButtons->buttons &= ~(1 << index);
    }

    return 0;
}
