//
// developed by Sergey Markelov (11/27/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include "clock_button.h"

#define IsPressed(clockButtons, index) (clockButtons->buttons & (1 << index))

#define SWITCH_STATE_PART 4

//
// @brief Initialized clockButtons
// @param clockButtons a pointer to ClockButtons
// @returns 0 on ok
// EINVAL - if _clockButtons_ is NULL
//          if _index_ is greater than the maximum
//          if _isPressed_ is NULL
//
int clock_button_init(ClockButtons *clockButtons)
{
#ifdef PARAM_CHECKS
    if(clockButtons == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockButtons is NULL");
#endif

    clockButtons->buttons = 0;

    return 0;
}

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
int clock_button_isPressed(const ClockButtons *clockButtons, size_t index, Bool *isPressed)
{
#ifdef PARAM_CHECKS
    if(clockButtons == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockButtons is NULL");
    if(index >= CLOCK_BUTTON_MAX_INDEX)
        OriginateErrorEx(EINVAL, "%d", "index should be less than %d", CLOCK_BUTTON_MAX_INDEX);
    if(isPressed == NULL)
        OriginateErrorEx(EINVAL, "%d", "isPressed is NULL");
#endif

    *isPressed = IsPressed(clockButtons, index);

    return 0;
}

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
int clock_button_wasClicked(const ClockButtons *clockButtons, size_t index, Bool *wasClicked)
{
#ifdef PARAM_CHECKS
    if(clockButtons == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockButtons is NULL");
    if(index >= CLOCK_BUTTON_MAX_INDEX)
        OriginateErrorEx(EINVAL, "%d", "index should be less than %d", CLOCK_BUTTON_MAX_INDEX);
    if(wasClicked == NULL)
        OriginateErrorEx(EINVAL, "%d", "wasClicked is NULL");
#endif

    *wasClicked = (clockButtons->buttons & (1 << (index + SWITCH_STATE_PART)))
                && (! IsPressed(clockButtons, index));

    return 0;
}

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
int clock_button_press(ClockButtons *clockButtons, size_t index, Bool isPressed)
{
#ifdef PARAM_CHECKS
    if(clockButtons == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockButtons is NULL");
    if(index >= CLOCK_BUTTON_MAX_INDEX)
        OriginateErrorEx(EINVAL, "%d", "index should be less than %d", CLOCK_BUTTON_MAX_INDEX);
#endif

    //
    // Process switch state
    //
    if(!!IsPressed(clockButtons, index) != !!isPressed) {
        // the state was changed
        clockButtons->buttons |= 1 << (index + SWITCH_STATE_PART);
    } else {
        // the state was not changed
        clockButtons->buttons &= ~(1 << (index + SWITCH_STATE_PART));
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
