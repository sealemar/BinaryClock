//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_STATE_H
#define BINARY_CLOCK_LIB_CLOCK_STATE_H

#include "date_time.h"
#include "clock_button.h"

#define CLOCK_BUTTON_INFO       0U
#define CLOCK_BUTTON_SET        1U
#define CLOCK_BUTTON_LEFT       2U
#define CLOCK_BUTTON_RIGHT      3U

typedef struct {
    unsigned int state;           // current state of the clock
    int step;                     // current step of the _state_
    unsigned int stepMillis;      // current step time (for animation)
    unsigned long lastUptime;     // in milliseconds
    DateTime dateTime;            // this gets updated in the beginning of clock_update()
    DateTime oldDateTime;         // this gets copied from _dateTime_ at the end of clock_update()
    ClockButtons buttons;         // the state of the clock buttons
} ClockState;

#endif
