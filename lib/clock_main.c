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
// @brief Clock main functionality. The user code should call clock_init()
// once and then clock_update() repeatedly.
//
// @example see arduino/binary_clock.ino or emulator/main.c to understand how it works
//
// void setup()
// {
//     clock_clearScreen();
//     Call(clock_init(&clockState));
// }
//
// void loop()
// {
//     for(size_t i = 0; i < countof(buttons); ++i) {
//         Call(clock_button_press( &(clockState.buttons), i, digitalRead(buttons[i]) == LOW ? TRUE : FALSE));
//     }
//     Call(clock_update(&clockState));
// }
//
// developed by Sergey Markelov (11/25/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include <string.h>

#include "clock_event.h"
#include "clock_extern.h"
#include "clock_main.h"
#include "clock_time.h"

//
// In milliseconds
// These numbers mean "not earlier than", because the actual time
// depends on the speed of the program main loop and the delay in
// the main loop.
//
#define CLOCK_ANIMATION_TEXT_STEP_TIME                   70U
#define CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME   200U

//
// @brief Updates step time such that the function will be finished immediately
// if clockState->stepMillis hasn't reached _stepAnimationMillis_ yet. Otherwise
// clockState->stepMillis will be updated to the modulus of _stepAnimationMillis_
// so that if some frames happen to be lost due to a delay, the animation will
// try to continue more or less smoothly.
//
#define updateStepTime(clockState, stepAnimationMillis) { \
    if(clockState->stepMillis < (stepAnimationMillis)) return 0; \
    clockState->stepMillis %= (stepAnimationMillis); \
}

//
// @brief blinks a binary number
// @param dateTimeValue is one of (second, minute, hour, day, month, year) of DateTime
//
#define blinkBinaryNumber(dateTimeValue, zeroValue) { \
    dateTimeValue = (dateTimeValue == zeroValue ? (zeroValue) + (CLOCK_MAX_BINARY_NUMBER) : zeroValue); }

//
// @brief Sets the clock state
// @param clockState a pointer to ClockState
// @param nextState the state
// @param nextStepMillis if the next step has animation step time, this should be set to that value, so
//        that the first frame will be shown immediately
// @param doClearScreen if true the clock screen will be first cleared
//
#define setClockState(clockState, nextState, nextStepMillis, doClearScreen) { \
    clockState->step  = 0; \
    clockState->state = nextState; \
    clockState->stepMillis = nextStepMillis; \
    if(doClearScreen) { clock_clearScreen(); } \
}

//
// @brief checks a button and changes the state if it was clicked
// @param clockState a pointer to ClockState
// @param buttonId the button to check
// @param nextState the state to change to if the button was clicked
// @param nextStepMillis if the next step has animation step time, this should be set to that value, so
//        that the first frame will be shown immediately
// @param doClearScreen if true the clock screen will be first cleared
//
#define checkButton(clockState, buttonId, nextState, nextStepMillis, doClearScreen) { \
    if(clock_button_wasClicked(clockState->buttons, buttonId)) { \
        setClockState(clockState, nextState, nextStepMillis, doClearScreen); \
        return 0; \
    } \
}

//
// @brief Adjusts days after the month was changed. If the new month has fewer days
//        than the old one, then dateTime.day will be set to the new max.
//
// @param dateTime a dereferenced DateTime
//
#define adjustDays(dateTime) { \
    int _d; \
    Call(date_time_daysInMonth( (dateTime).year, (dateTime).month, &_d )); \
    if( (dateTime).day > _d ) { \
        (dateTime).day = _d; \
    } \
}

//
// This text will be shown at clock_state_hello
//
static const char StateHelloText[] = "BinaryClock by sealemar v."
                                     TOSTRING(VERSION_MAJOR) "."
                                     TOSTRING(VERSION_MINOR) "."
                                     TOSTRING(VERSION_FIX)
                                     " \001 ";

static const char NoEventsStr[] = " No events \002 ";

//
// @brief a callback function for slideText()
//
static int showEventsSlideTextCompletesCallback(ClockState *clockState)
{
    if( (size_t)clockState->events.index == clockState->events.size - 1) {
        clockState->events.index = 0;
    } else {
        ++(clockState->events.index);
    }

    int index;
    Call( clock_event_findClosestFromList(clockState->events.ptr, clockState->events.size,
                                            clockState->dateTime.month, clockState->dateTime.day,
                                            &index ) );

    if(clockState->events.index == index) {
        setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, TRUE);
    } else {
        setClockState(clockState, clockState->state, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);
    }

    return 0;
}

//
// @brief A helper function to slide a text
// @param clockState
// @param text a text to slide
// @param nextState which state should be the next after the text sliding is finished
// @param nextStepMillis if the next step has animation step time, this should be set to that value, so
//        that the first frame will be shown immediately
// @param callback an optional pointer to a callback function which should be called when the slideText()
//                 completes. If it is not NULL, _nextState_ and _nextStepMillis_ will be ignored and
//                 the callback will be called.
//
// @returns 0 on ok
//
static int slideText(
        ClockState *clockState,
        const char *text,
        unsigned int nextState,
        unsigned int nextStepMillis,
        int (* callback)(ClockState *clockState))
{
    updateStepTime(clockState, CLOCK_ANIMATION_TEXT_STEP_TIME);

    unsigned char pattern[CLOCK_PATTERN_SIZE];
    Bool isLastStep;

    Call(clock_slideText(text, clockState->step, &isLastStep, pattern));

    Call(clock_drawPattern(pattern));

    if(!isLastStep) {
        ++(clockState->step);
    } else {
        if(callback) {
            Call( callback(clockState) );
        } else {
            setClockState(clockState, nextState, nextStepMillis, TRUE);
        }
    }

    return 0;
}

static int clock_state_hello(ClockState *clockState)
{
    checkButton(clockState, CLOCK_BUTTON_SET,
                CLOCK_STATE_SHOW_TIME, 0, TRUE);

    Call(slideText(clockState, StateHelloText, CLOCK_STATE_SHOW_TIME, 0, NULL));

    return 0;
}

static int clock_state_showTime(ClockState *clockState)
{
    checkButton(clockState, CLOCK_BUTTON_INFO,
                CLOCK_STATE_SHOW_TIME_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    checkButton(clockState, CLOCK_BUTTON_SET,
                CLOCK_STATE_SET_TIME, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME, FALSE);

    checkButton(clockState, CLOCK_BUTTON_LEFT,
                CLOCK_STATE_SHOW_DATE, 0, TRUE);

    checkButton(clockState, CLOCK_BUTTON_RIGHT,
                CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Show time
    //
    const DateTime *dt    = &(clockState->dateTime);
    const DateTime *oldDt = &(clockState->oldDateTime);

    if(clockState->step == 0
    || dt->second != oldDt->second
    || dt->minute != oldDt->minute
    || dt->hour   != oldDt->hour) {
        clockState->step = 1;
        Call(clock_displayTime(&(clockState->dateTime)));
    }
    return 0;
}

static int clock_state_showDate(ClockState *clockState)
{
    checkButton(clockState, CLOCK_BUTTON_INFO,
                CLOCK_STATE_SHOW_DATE_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    checkButton(clockState, CLOCK_BUTTON_SET,
                CLOCK_STATE_SET_DATE, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME, FALSE);

    checkButton(clockState, CLOCK_BUTTON_LEFT,
                CLOCK_STATE_SHOW_TIME, 0, TRUE);

    checkButton(clockState, CLOCK_BUTTON_RIGHT,
                CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Show date
    //
    const DateTime *dt    = &(clockState->dateTime);
    const DateTime *oldDt = &(clockState->oldDateTime);

    if(clockState->step == 0
    || dt->day   != oldDt->day
    || dt->month != oldDt->month
    || dt->year  != oldDt->year) {
        clockState->step = 1;
        Call(clock_displayDate(&(clockState->dateTime)));
    }
    return 0;
}

static int clock_state_showTimeBigEndian(ClockState *clockState)
{
    checkButton(clockState, CLOCK_BUTTON_INFO,
                CLOCK_STATE_SHOW_TIME, 0, TRUE);

    checkButton(clockState, CLOCK_BUTTON_LEFT,
                CLOCK_STATE_SHOW_DATE_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    checkButton(clockState, CLOCK_BUTTON_RIGHT,
                CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Prepare the text if this is step 0
    //
    if(clockState->step == 0) {
        clockState->text[0] = ' ';
        Call(date_time_timeToStr( &(clockState->dateTime), clockState->text + 1 ));
    }

    Call(slideText(clockState, clockState->text, CLOCK_STATE_SHOW_TIME, 0, NULL));

    return 0;
}

static int clock_state_showDateBigEndian(ClockState *clockState)
{
    checkButton(clockState, CLOCK_BUTTON_INFO,
                CLOCK_STATE_SHOW_DATE, 0, TRUE);

    checkButton(clockState, CLOCK_BUTTON_LEFT,
                CLOCK_STATE_SHOW_TIME_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    checkButton(clockState, CLOCK_BUTTON_RIGHT,
                CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Prepare the text if this is step 0
    //
    if(clockState->step == 0) {
        clockState->text[0] = ' ';
        Call(date_time_dateToStr( &(clockState->dateTime), clockState->text + 1 ));
    }

    Call(slideText(clockState, clockState->text, CLOCK_STATE_SHOW_DATE, 0, NULL));

    return 0;
}

static int clock_state_setTime(ClockState *clockState)
{
    //
    // Go from hours, to minutes, then to seconds, then change the state to show time?
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        clockState->step += 2;
        if(clockState->step > 5) {
            setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, FALSE);
            return 0;
        }
    }

    DateTime *dt = &(clockState->dateTime);

    //
    // Decrease selected time
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            --(dt->hour);
            if(dt->hour < 0) {
                dt->hour = HOURS_COUNT - 1;
            }
        } else if(clockState->step == 2 || clockState->step == 3) {
            --(dt->minute);
            if(dt->minute < 0) {
                dt->minute = 59;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            dt->second = 0;
        }
    }

    //
    // Increase selected time
    //
    else if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_RIGHT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            ++(dt->hour);
            if(dt->hour >= HOURS_COUNT) {
                dt->hour = 0;
            }
        } else if(clockState->step == 2 || clockState->step == 3) {
            ++(dt->minute);
            if(dt->minute >= 60) {
                dt->hour = 60;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            dt->second = 30;
        }
    }

    //
    // Set time
    //
    updateStepTime(clockState, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME);

    // step 0,1 - blink hours
    // step 2,3 - blink minutes
    // step 4,5 - blink seconds
    DateTime tempDt;
    memcpy(&tempDt, dt, sizeof(tempDt));
    switch(clockState->step) {
        case 0: blinkBinaryNumber(tempDt.hour, 0); clockState->step = 1; break;
        case 1: clockState->step = 0; break;

        case 2: blinkBinaryNumber(tempDt.minute, 0); clockState->step = 3; break;
        case 3: clockState->step = 2; break;

        case 4: blinkBinaryNumber(tempDt.second, 0); clockState->step = 5; break;
        case 5: clockState->step = 4; break;

#ifdef PARAM_CHECKS
        default:
            OriginateErrorEx(EINVAL, "%d", "Unexpected clockState->step = %d. Should be 0 < step < 6", clockState->step);
#endif
    }
    Call(clock_displayTime(&tempDt));

    return 0;
}

static int clock_state_setDate(ClockState *clockState)
{
    //
    // Go from months, to days, then to years, then change the state to show time?
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        clockState->step += 2;
        if(clockState->step > 5) {
            setClockState(clockState, CLOCK_STATE_SHOW_DATE, 0, FALSE);
            return 0;
        }
    }

    DateTime *dt = &(clockState->dateTime);
    Bool wasChanged = FALSE;

    //
    // Decrease selected time
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            --(dt->month);
            if(dt->month < JANUARY) {
                dt->month = DECEMBER;
            }
            adjustDays(clockState->dateTime);
        } else if(clockState->step == 2 || clockState->step == 3) {
            --(dt->day);
            if(dt->day < 1) {
                int d;
                Call(date_time_daysInMonth(dt->year, dt->month, &d));
                dt->day = d;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            --(dt->year);
            if(dt->year < MIN_YEAR) {
                dt->year = MAX_YEAR;
            }
            adjustDays(clockState->dateTime);
        }
        wasChanged = TRUE;
    }

    //
    // Increase selected time
    //
    else if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_RIGHT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            ++(dt->month);
            if(dt->month > DECEMBER) {
                dt->month = JANUARY;
            }
            adjustDays(clockState->dateTime);
        } else if(clockState->step == 2 || clockState->step == 3) {
            ++(dt->day);
            int d;
            Call(date_time_daysInMonth(dt->year, dt->month, &d));
            if(dt->day > d) {
                dt->day = 1;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            ++(dt->year);
            if(dt->year > MAX_YEAR) {
                dt->year = MIN_YEAR;
            }
            adjustDays(clockState->dateTime);
        }
        wasChanged = TRUE;
    }

    //
    // Update events
    //
    if(wasChanged) {
        if(clockState->dateTime.year != clockState->oldDateTime.year) {
            Call( clock_event_initList(clockState->events.ptr, clockState->events.size, clockState->dateTime.year) );
        }
        Call( clock_event_updateList(clockState->events.ptr, clockState->events.size, &(clockState->dateTime) ) );
    }

    //
    // Set date
    //
    updateStepTime(clockState, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME);

    // step 0,1 - blink months
    // step 2,3 - blink days
    // step 4,5 - blink years
    DateTime tempDt;
    memcpy(&tempDt, dt, sizeof(tempDt));
    switch(clockState->step) {
        case 0: tempDt.month = -1; clockState->step = 1; break;
        case 1: clockState->step = 0; break;

        case 2: blinkBinaryNumber(tempDt.day, 0); clockState->step = 3; break;
        case 3: clockState->step = 2; break;

        case 4: blinkBinaryNumber(tempDt.year, MIN_YEAR); clockState->step = 5; break;
        case 5: clockState->step = 4; break;

#ifdef PARAM_CHECKS
        default:
            OriginateErrorEx(EINVAL, "%d", "Unexpected clockState->step = %d. Should be 0 < step < 6", clockState->step);
#endif
    }
    Call(clock_displayDate(&tempDt));

    return 0;
}

static int clock_state_showEvents(ClockState *clockState)
{
    NullCheck(clockState->events.ptr);

#ifdef PARAM_CHECKS
    if(clockState->events.size == 0) {
        OriginateErrorEx(EINVAL, "%d", "clockState->events.size is zero");
    }
#endif

    //
    // Look up the next closest event
    //
    if(clockState->events.index == CLOCK_EVENT_INDEX_LOOKUP) {
        Call( clock_event_findClosestFromList(clockState->events.ptr, clockState->events.size,
                                              clockState->dateTime.month, clockState->dateTime.day,
                                              &(clockState->events.index) ) );
    }

    //
    // Show event year information
    //
    checkButton(clockState, CLOCK_BUTTON_SET,
                CLOCK_STATE_SHOW_EVENT_YEAR_INFO, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Show date
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_INFO)) {
        clockState->events.index = CLOCK_EVENT_INDEX_LOOKUP;
        setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, TRUE);
        return 0;
    }

    //
    // Previous event
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        if(clockState->events.index == 0) {
            clockState->events.index = clockState->events.size - 1;
        } else {
            --(clockState->events.index);
        }
        setClockState(clockState, clockState->state, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);
        return 0;
    }

    //
    // Next event
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_RIGHT)) {
        if( (size_t)clockState->events.index == clockState->events.size - 1) {
            clockState->events.index = 0;
        } else {
            ++(clockState->events.index);
        }
        setClockState(clockState, clockState->state, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);
        return 0;
    }

    //
    // Show event
    //
    if(clockState->step == 0) {
        if(clockState->events.size == 0) {
            strcpy(clockState->text, NoEventsStr);
        } else {
            clockState->text[0] = ' ';
            Call( clock_event_toStr( &(clockState->events.ptr[clockState->events.index]), clockState->text + 1) );
        }
    }

    Call(slideText(clockState, clockState->text, 0, 0, showEventsSlideTextCompletesCallback));

    return 0;
}

static int clock_state_showEventYearInfo(ClockState *clockState)
{
    NullCheck(clockState->events.ptr);

#ifdef PARAM_CHECKS
    if(clockState->events.size == 0) {
        OriginateErrorEx(EINVAL, "%d", "clockState->events.size is zero");
    }
#endif

    //
    // Look up the next closest event
    //
    if(clockState->events.index == CLOCK_EVENT_INDEX_LOOKUP) {
        Call( clock_event_findClosestFromList(clockState->events.ptr, clockState->events.size,
                                              clockState->dateTime.month, clockState->dateTime.day,
                                              &(clockState->events.index) ) );
    }

    //
    // Skip
    //
    checkButton(clockState, CLOCK_BUTTON_SET,
                CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);

    //
    // Show event year information
    //
    if(clockState->step == 0) {
        if(clockState->events.size == 0) {
            strcpy(clockState->text, NoEventsStr);
        } else {
            clockState->text[0] = ' ';
            Call( clock_event_yearInfoToStr( &(clockState->events.ptr[clockState->events.index]), clockState->text + 1) );
        }
    }

    Call(slideText(clockState, clockState->text, CLOCK_STATE_SHOW_EVENTS, CLOCK_ANIMATION_TEXT_STEP_TIME, NULL));

    return 0;
}


//
// ClockStateFunctionMap - a static array of const function pointers
// to handle ClockState->state
//
static int (* const ClockStateFunctionMap[])(ClockState *) = {
    clock_state_hello,                //  CLOCK_STATE_HELLO
    clock_state_showTime,             //  CLOCK_STATE_SHOW_TIME
    clock_state_showDate,             //  CLOCK_STATE_SHOW_DATE
    clock_state_showTimeBigEndian,    //  CLOCK_STATE_SHOW_TIME_BIG_ENDIAN
    clock_state_showDateBigEndian,    //  CLOCK_STATE_SHOW_DATE_BIG_ENDIAN
    clock_state_setTime,              //  CLOCK_STATE_SET_TIME
    clock_state_setDate,              //  CLOCK_STATE_SET_DATE
    clock_state_showEvents,           //  CLOCK_STATE_SHOW_EVENTS
    clock_state_showEventYearInfo,    //  CLOCK_STATE_SHOW_EVENT_YEAR_INFO
};

//
// @brief Initializes clockState
// @param clockState a structure which holds the entire state of the clock
// @note call this function before calling clock_update() for the first time
//
int clock_init(ClockState *clockState)
{
    NullCheck(clockState);

    memset(clockState, 0, sizeof(ClockState));

    //
    // Init clockState->dateTime
    //
    if(clock_extern_initDateTime != NULL) {
        Call( clock_extern_initDateTime( &(clockState->dateTime) ) );
        memcpy( &(clockState->oldDateTime), &(clockState->dateTime), sizeof(DateTime) );
    } else {
        clockState->dateTime.day    = 1;
        clockState->oldDateTime.day = 1;
    }

    unsigned long millis;
    Call( clock_extern_uptimeMillis(&millis) );
    Call( clock_updateUptimeMillis(millis, &(clockState->lastUptime), &millis) );

    clockState->events.ptr   = ClockEvents;
    clockState->events.size  = CLOCK_EVENTS_SIZE;
    clockState->events.index = CLOCK_EVENT_INDEX_LOOKUP;

    Call( clock_event_initList(clockState->events.ptr, clockState->events.size, clockState->dateTime.year) );
    Call( clock_event_updateList(clockState->events.ptr, clockState->events.size, &(clockState->dateTime) ) );

    return 0;
}

//
// @brief Call this function from the main loop
// @param clockState a structure which holds the entire state of the clock
// @returns 0 on success
// EINVAL - if clockState is NULL
//
// @note call clock_init() before calling clock_update() for the first time
//
int clock_update(ClockState *clockState)
{
    NullCheck(clockState);

    unsigned long millis;
    Call(clock_extern_uptimeMillis(&millis));
    Call(clock_updateUptimeMillis(millis, &(clockState->lastUptime), &millis));
    Call(date_time_addMillis(&(clockState->dateTime), millis));

    clockState->stepMillis += millis;

#ifdef PARAM_CHECKS
    if(clockState->state >= countof(ClockStateFunctionMap)) {
        OriginateErrorEx(EINVAL, "%d", "clockState->state = %u is not implemented", clockState->state);
    }
#endif

    if(clockState->dateTime.day != clockState->oldDateTime.day) {
        Call( clock_event_updateList(clockState->events.ptr, clockState->events.size, &(clockState->dateTime) ) );
    }

    Call(ClockStateFunctionMap[clockState->state](clockState));

    memcpy(&(clockState->oldDateTime), &(clockState->dateTime), sizeof(DateTime));

    return 0;
}
