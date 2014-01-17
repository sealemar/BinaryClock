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
// developed by Sergey Markelov (11/30/2013)
//
// @brief This is personal events. Set it to whatever you want
//

#include "clock_event.h"
#include "clock_event_personal.h"

ClockEvent ClockEvents[CLOCK_EVENTS_SIZE] = {
    clock_event_initDayOfMonth(1,  JANUARY,     0,                           "New year"),
    clock_event_initDayOfMonth(14, FEBRUARY,  270,                           "Valentine's day"),
    clock_event_initDayOfMonth(1,  APRIL,    1392,                           "Fool's day"),
    clock_event_initDayOfWeek (FRIDAY, 0, WEEK_FROM_END, JULY, 2000,         "SysAdmin's Day"),
    clock_event_initDayOfYear (256, 2009,                                    "Programmer's day"),
    clock_event_initDayOfWeek (THURSDAY, 3, WEEK_FROM_START, NOVEMBER, 1574, "Thanksgiving"),
    clock_event_initDayOfMonth(25, DECEMBER,    0,                           "Christmas"),
};
