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
// @brief A main structure to work with BinaryClock from the user code
//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_STATE_H
#define BINARY_CLOCK_LIB_CLOCK_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

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
#define CLOCK_STATE_SHOW_EVENTS             7
#define CLOCK_STATE_SHOW_EVENT_YEAR_INFO    8

#define CLOCK_STATE_COUNT                   9


#define CLOCK_BUTTON_INFO       0U
#define CLOCK_BUTTON_SET        1U
#define CLOCK_BUTTON_LEFT       2U
#define CLOCK_BUTTON_RIGHT      3U

#define CLOCK_BUTTON_COUNT      4U

#define STATE_TEXT_SIZE       101U

#define MIN_YEAR  2000
#define MAX_YEAR  ( MIN_YEAR + CLOCK_MAX_BINARY_NUMBER )

#define CLOCK_EVENT_INDEX_LOOKUP (-1)

#include "clock_event.h"

typedef struct {
    unsigned int  state;                   // current state of the clock
    int           step;                    // current step of the _state_
    unsigned int  stepMillis;              // current step time (for animation)
    unsigned long lastUptime;              // in milliseconds
    DateTime      dateTime;                // this gets updated in the beginning of clock_update()
    DateTime      oldDateTime;             // this gets copied from _dateTime_ at the end of clock_update()
    ClockButtons  buttons;                 // the state of the clock buttons
    char          text[STATE_TEXT_SIZE];   // a state may set this to some text
    struct {
        ClockEvent *ptr;         // the pointer to the head of the events array
        size_t      size;        // the size of the events array
        int         index;       // index of the currently shown event (default is CLOCK_EVENT_INDEX_LOOKUP, meaning look up the next closest event)
    } events;                              // events information
} ClockState;

#ifdef __cplusplus
}
#endif

#endif
