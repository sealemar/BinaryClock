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
// @brief BinaryClock lib/clock_button unit tests
//
// developed by Sergey Markelov (11/27/2013)
//

#include <clock_button.h>
#include "ut_clock_button.h"

static const char *StatePressed = "isPressed";
static const char *StateClicked = "wasClicked";

static int assert_state(Bool state, Bool expected, size_t index, const char* stateStr)
{
    if(!!state != !!expected) {
        OriginateErrorEx(-1, "%d", "Unexpected %s = %s at index = %zu. Expected = %s",
                                   stateStr,
                                   state ? "true" : "false",
                                   index,
                                   expected ? "true" : "false");
    }

    return 0;
}

static int test_clock_button_press_indexOutOfBounds()
{
    ClockButtons clockButtons;
    assert_function(clock_button_press(&clockButtons, CLOCK_BUTTON_MAX_COUNT, TRUE), ERANGE);

    return 0;
}

static int test_clock_button_init_pressedAndClickedAreFalse()
{
    ClockButtons clockButtons;

    clock_button_init(clockButtons);

    for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
        Bool state = clock_button_isPressed(clockButtons, i);
        Call(assert_state(state, FALSE, i, StatePressed));

        state = clock_button_wasClicked(clockButtons, i);
        Call(assert_state(state, FALSE, i, StateClicked));
    }

    return 0;
}

static int test_clock_button_press_changesIsPressedState()
{
    ClockButtons clockButtons;

    clock_button_init(clockButtons);

    for(size_t b = 0; b < CLOCK_BUTTON_MAX_COUNT; ++b) {
        //
        // Press the button
        //
        Call(clock_button_press(&clockButtons, b, TRUE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, i == b, i, StatePressed));
        }

        //
        // Release the button
        //
        Call(clock_button_press(&clockButtons, b, FALSE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, FALSE, i, StatePressed));
        }
    }

    return 0;
}

static int test_clock_button_press_doublePressAndDoubleReleaseDontChangeIsPressedState()
{
    ClockButtons clockButtons;

    clock_button_init(clockButtons);

    for(size_t b = 0; b < CLOCK_BUTTON_MAX_COUNT; ++b) {
        //
        // Press the button
        //
        Call(clock_button_press(&clockButtons, b, TRUE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, i == b, i, StatePressed));
        }

        //
        // Second press
        //
        Call(clock_button_press(&clockButtons, b, TRUE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, i == b, i, StatePressed));
        }

        //
        // Release the button
        //
        Call(clock_button_press(&clockButtons, b, FALSE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, FALSE, i, StatePressed));
        }

        //
        // Second release
        //
        Call(clock_button_press(&clockButtons, b, FALSE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            Bool state = clock_button_isPressed(clockButtons, i);
            Call(assert_state(state, FALSE, i, StatePressed));
        }
    }

    return 0;
}

static int test_clock_button_press_changesWasClickedStateAfterRelease()
{
    ClockButtons clockButtons;
    Bool state;

    clock_button_init(clockButtons);

    for(size_t b = 0; b < CLOCK_BUTTON_MAX_COUNT; ++b) {
        //
        // Press the button
        //
        Call(clock_button_press(&clockButtons, b, TRUE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            state = clock_button_wasClicked(clockButtons, i);
            Call(assert_state(state, FALSE, i, StateClicked));
        }

        //
        // Release the button
        //
        Call(clock_button_press(&clockButtons, b, FALSE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            state = clock_button_wasClicked(clockButtons, i);
            Call(assert_state(state, i == b, i, StatePressed));
        }

        //
        // Release the button again to clear the switched state
        //
        Call(clock_button_press(&clockButtons, b, FALSE));

        for(size_t i = 0; i < CLOCK_BUTTON_MAX_COUNT; ++i) {
            state = clock_button_wasClicked(clockButtons, i);
            Call(assert_state(state, FALSE, i, StatePressed));
        }
    }

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_button_press_indexOutOfBounds, "clock_button_press() returns EINVAL if index is out of bounds", FALSE },
    { test_clock_button_init_pressedAndClickedAreFalse, "clock_button_init() initializes clockButtons so that all buttons are in released and non clicked state", FALSE },
    { test_clock_button_press_changesIsPressedState, "clock_button_press() changes isPressed state", FALSE },
    { test_clock_button_press_doublePressAndDoubleReleaseDontChangeIsPressedState, "clock_button_press() double press and double release don't change isPressed state", FALSE },
    { test_clock_button_press_changesWasClickedStateAfterRelease, "clock_button_press() changes wasClicked state after the button has been released", FALSE },
};

int ut_clock_button()
{
    return runTestSuite(testSuite);
}
