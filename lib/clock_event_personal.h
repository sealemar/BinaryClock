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
// developed by Sergey Markelov (12/07/2013)
//
// @brief This is personal events. Set it to whatever you want
//

#ifndef BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H
#define BINARY_CLOCK_LIB_CLOCK_EVENT_PERSONAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_EVENTS_SIZE 7

#include "clock_event.h"

//
// @brief ClockEvents is a list of all events of which the clock knows.
//        These can be personalized events, such as birthdays and other
//        special days.
//
// @note This array is defined in lib/clock_events_personal.c
//
// @note clock_events_init() needs to be called at the program startup and then
// every time when the year changes.
//
// @warning make sure, that ClockEvent.name is not longer than
//          EVENT_STRING_BUFFER_SIZE - 4 - DATE_TIME_DATE_STR_SIZE
//
extern ClockEvent ClockEvents[CLOCK_EVENTS_SIZE];

#ifdef __cplusplus
}
#endif

#endif
