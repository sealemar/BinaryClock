//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_STATE_H
#define BINARY_CLOCK_LIB_CLOCK_STATE_H

#include "date_time.h"
#include "clock_button.h"
#include "clock.h"

#define CLOCK_STATE_HELLO                   0
#define CLOCK_STATE_SHOW_TIME               1
#define CLOCK_STATE_SHOW_DATE               2
#define CLOCK_STATE_SHOW_TIME_BIG_ENDIAN    3
#define CLOCK_STATE_SHOW_DATE_BIG_ENDIAN    4
#define CLOCK_STATE_SET_TIME                5
#define CLOCK_STATE_SET_DATE                6

#define CLOCK_STATE_COUNT                   7


#define CLOCK_BUTTON_INFO       0U
#define CLOCK_BUTTON_SET        1U
#define CLOCK_BUTTON_LEFT       2U
#define CLOCK_BUTTON_RIGHT      3U

#define CLOCK_BUTTON_COUNT      4U

#define STATE_TEXT_SIZE       101U

#define MIN_YEAR  2000
#define MAX_YEAR  ( MIN_YEAR + CLOCK_MAX_BINARY_NUMBER )


typedef struct {
    unsigned int state;           // current state of the clock
    int step;                     // current step of the _state_
    unsigned int stepMillis;      // current step time (for animation)
    unsigned long lastUptime;     // in milliseconds
    DateTime dateTime;            // this gets updated in the beginning of clock_update()
    DateTime oldDateTime;         // this gets copied from _dateTime_ at the end of clock_update()
    ClockButtons buttons;         // the state of the clock buttons
    char text[STATE_TEXT_SIZE];   // a state may set this to some text
} ClockState;

#endif
