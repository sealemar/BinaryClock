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
