//
// developed by Sergey Markelov (11/27/2013)
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
//          if _index_ is greater than the maximum
//
int clock_button_press(ClockButtons *clockButtons, size_t index, Bool isPressed)
{
#ifdef PARAM_CHECKS
    if(clockButtons == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockButtons is NULL");
    if(index >= CLOCK_BUTTON_MAX_COUNT)
        OriginateErrorEx(EINVAL, "%d", "index should be less than %d", CLOCK_BUTTON_MAX_COUNT);
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
