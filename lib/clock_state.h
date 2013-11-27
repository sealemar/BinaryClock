
//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_STATE_H
#define BINARY_CLOCK_LIB_CLOCK_STATE_H

#include "date_time.h"

typedef struct {
    unsigned char isOn;         // 1 bit = 1 button. 0 - off, 1 - on
    unsigned char wasSwitched;  // 1 bit = 1 bit from _isOn_. 0 - state hasn't changed, 1 - state has changed
} ClockButtons;

typedef struct {
    unsigned int state;         // current state of the clock
    int step;                   // current step of the _state_
    unsigned long lastUptime;   // in milliseconds
    DateTime dateTime;          // this gets updated in the beginning of clock_update()
    DateTime oldDateTime;       // this gets copied from _dateTime_ at the end of clock_update()
    ClockButtons buttons;       // the state of the clock buttons
} ClockState;

#endif
