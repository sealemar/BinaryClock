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

#ifndef BINARY_CLOCK_LIB_CLOCK_BUTTON_H
#define BINARY_CLOCK_LIB_CLOCK_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

//
// @brief The maximum amount of buttons which ClockButtons can hold
//
#define CLOCK_BUTTON_MAX_COUNT  4U

typedef struct {
    uint8_t buttons;      // first 4 bits are switch state, last 4 bits are pressed state
} ClockButtons;

//
// @warning Internal, don't reference
//
#define _SWITCH_STATE_PART 4U

//
// @brief Initialized clockButtons
// @param clockButtons an actual ClockButtons object (not a pointer to)
//
#define clock_button_init(clockButtons) { (clockButtons).buttons = 0; }

//
// @brief Checks whether the button by a given index is pressed
// @param clockButtons an actual ClockButtons object (not a pointer to)
// @param index which button to test
// @returns 0 if the button is not pressed
//
#define clock_button_isPressed(clockButtons, index) ((clockButtons).buttons & (1 << index))

//
// @brief Checks whether the button by a given index was clicked.
//        A click counts when the button had been in pressed state and was
//        changed to released state, i.e.
//
//        clock_button_press(clockButtons, 3, TRUE);
//        clock_button_press(clockButtons, 3, FALSE);
//        //
//        // now the button at index 3 is considered to be clicked
//
// @note calling clock_button_press() with the same _isPressed_ state on the same button
//       clears the switch state, i.e.
//
//        clock_button_press(clockButtons, 3, TRUE);
//        clock_button_press(clockButtons, 3, FALSE);
//        clock_button_press(clockButtons, 3, FALSE);
//        //
//        // now the button at index 3 is NOT considered to be clicked
//
//
// @param clockButtons an actual ClockButtons object (not a pointer to)
// @param index which button to test
// @returns 0 if the button is not clicked
//
#define clock_button_wasClicked(clockButtons, index) ( \
    ((clockButtons).buttons & (1 << (index + _SWITCH_STATE_PART))) \
 && (! clock_button_isPressed((clockButtons), index)) )

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
int clock_button_press(ClockButtons *clockButtons, size_t index, Bool isPressed);

#ifdef __cplusplus
}
#endif

#endif
