//
// developed by Sergey Markelov (11/27/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_BUTTON_H
#define BINARY_CLOCK_LIB_CLOCK_BUTTON_H

#define CLOCK_BUTTON_MAX_INDEX  4U

typedef struct {
    unsigned char buttons;      // first 4 bits are switch state, last 4 bits are pressed state
} ClockButtons;

//
// @brief Initialized clockButtons
// @param clockButtons a pointer to ClockButtons
// @returns 0 on ok
// EINVAL - if _clockButtons_ is NULL
//          if _index_ is greater than the maximum
//          if _isPressed_ is NULL
//
int clock_button_init(ClockButtons *clockButtons);

//
// @brief Checks whether the button by a given index is pressed
// @param clockButtons a pointer to ClockButtons
// @param index which button to test
// @param isPressed the result is returned here
// @returns 0 on ok
// EINVAL - if _clockButtons_ is NULL
//          if _index_ is greater than the maximum
//          if _isPressed_ is NULL
//
int clock_button_isPressed(const ClockButtons *clockButtons, size_t index, Bool *isPressed);

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
// @param clockButtons a pointer to ClockButtons
// @param index which button to test
// @param wasClicked the result is returned here
// @returns 0 on ok
// EINVAL - if _clockButtons_ is NULL
//          if _index_ is greater than the maximum
//          if _isClicked_ is NULL
//
int clock_button_wasClicked(const ClockButtons *clockButtons, size_t index, Bool *wasClicked);

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
//          if _index_ is greater than the maximum
//
int clock_button_press(ClockButtons *clockButtons, size_t index, Bool isPressed);

#endif