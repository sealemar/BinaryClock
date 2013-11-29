//
// developed by Sergey Markelov (11/25/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#ifndef ARDUINO
#include <string.h>
#endif

#include "clock.h"
#include "clock_time.h"
#include "clock_main.h"

//
// Update animation every 100 milliseconds
//
#define CLOCK_STATE_HELLO_STEP_TIME    100U
#define CLOCK_STATE_SET_STEP_TIME      200U

#define updateStepTime(clockState, step) { \
    if(clockState->stepMillis < (step)) return 0; \
    clockState->stepMillis %= (step); \
}

//
// @brief blinks a binary number
// @param dateTimeValue is one of (second, minute, hour, day, month, year) of DateTime
//
#define blinkBinaryNumber(dateTimeValue) { dateTimeValue = (dateTimeValue == 0 ? CLOCK_MAX_BINARY_NUMBER : 0); }

static int clock_state_hello(ClockState *clockState)
{
    updateStepTime(clockState, CLOCK_STATE_HELLO_STEP_TIME);

    const char text[] = " Hello Sergey!!!";
    unsigned char  pattern[CLOCK_PATTERN_SIZE];
    Bool isLastStep;

    Call(clock_slideText(text, clockState->step, &isLastStep, pattern));

    Call(clock_drawPattern(pattern));

    if(!isLastStep) {
        ++(clockState->step);
    } else {
        clockState->step  = 0;
        clockState->state = CLOCK_STATE_SHOW_TIME;
    }

    return 0;
}

static int clock_state_showTime(ClockState *clockState)
{
    //
    // Need to change state to set time?
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        clockState->step  = 0;
        clockState->state = CLOCK_STATE_SET_TIME;
        clockState->stepMillis = CLOCK_STATE_SET_STEP_TIME;
        return 0;
    }

    //
    // Show time
    //
    const DateTime *dt    = &(clockState->dateTime);
    const DateTime *oldDt = &(clockState->oldDateTime);

    if(dt->second != oldDt->second
    || dt->minute != oldDt->minute
    || dt->hour   != oldDt->hour) {
        Call(clock_displayTime(&(clockState->dateTime)));
    }
    return 0;
}

static int clock_state_showDate(ClockState *clockState)
{
    const DateTime *dt    = &(clockState->dateTime);
    const DateTime *oldDt = &(clockState->oldDateTime);

    if(dt->day   != oldDt->day
    || dt->month != oldDt->month
    || dt->year  != oldDt->year) {
        Call(clock_displayDate(&(clockState->dateTime)));
    }
    return 0;
}

static int clock_state_showTimeBigEndian(ClockState *clockState)
{
    (void)clockState;
    return 0;
}

static int clock_state_showDateBigEndian(ClockState *clockState)
{
    (void)clockState;
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
            clockState->step  = 0;
            clockState->state = CLOCK_STATE_SHOW_TIME;
            clockState->stepMillis = 0;
            return 0;
        }
    }

    //
    // Decrease selected time
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            --(clockState->dateTime.hour);
            if(clockState->dateTime.hour < 0) {
                clockState->dateTime.hour = HOURS_COUNT - 1;
            }
        } else if(clockState->step == 2 || clockState->step == 3) {
            --(clockState->dateTime.minute);
            if(clockState->dateTime.minute < 0) {
                clockState->dateTime.minute = 59;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            clockState->dateTime.second = 0;
        }
    }

    //
    // Increase selected time
    //
    else if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_RIGHT)) {
        if(clockState->step == 0 || clockState->step == 1) {
            ++(clockState->dateTime.hour);
            if(clockState->dateTime.hour >= HOURS_COUNT) {
                clockState->dateTime.hour = 0;
            }
        } else if(clockState->step == 2 || clockState->step == 3) {
            ++(clockState->dateTime.minute);
            if(clockState->dateTime.minute >= 60) {
                clockState->dateTime.hour = 60;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            clockState->dateTime.second = 30;
        }
    }

    //
    // Set time
    //
    updateStepTime(clockState, CLOCK_STATE_SET_STEP_TIME);

    // step 0,1 - blink hours
    // step 2,3 - blink minutes
    // step 4,5 - blink seconds
    DateTime dt;
    memcpy(&dt, &(clockState->dateTime), sizeof(dt));
    switch(clockState->step) {
        case 0: blinkBinaryNumber(dt.hour); clockState->step = 1; break;
        case 1: clockState->step = 0; break;

        case 2: blinkBinaryNumber(dt.minute); clockState->step = 3; break;
        case 3: clockState->step = 2; break;

        case 4: blinkBinaryNumber(dt.second); clockState->step = 5; break;
        case 5: clockState->step = 4; break;

#ifdef PARAM_CHECKS
        default:
            OriginateErrorEx(EINVAL, "%d", "Unexpected clockState->step = %d. Should be 0 < step < 6", clockState->step);
#endif
    }
    Call(clock_displayTime(&dt));

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
};

//
// @brief Initializes clockState
// @param clockState a structure which holds the entire state of the clock
// @note call this function before calling clock_update() for the first time
//
int clock_init(ClockState *clockState)
{
#ifdef PARAM_CHECKS
    if(clockState == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockState is NULL");
#endif

    memset(clockState, 0, sizeof(ClockState));

    // TODO: remove before release
    clockState->state = CLOCK_STATE_SHOW_TIME;

    unsigned long millis;
    Call(clock_uptimeMillis(&millis));
    Call(clock_updateUptimeMillis(millis, &(clockState->lastUptime), &millis));

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
#ifdef PARAM_CHECKS
    if(clockState == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockState is NULL");
#endif

    unsigned long millis;
    Call(clock_uptimeMillis(&millis));
    Call(clock_updateUptimeMillis(millis, &(clockState->lastUptime), &millis));
    Call(dateTime_addMillis(&(clockState->dateTime), millis));

    clockState->stepMillis += millis;

#ifdef PARAM_CHECKS
    if(clockState->state >= countof(ClockStateFunctionMap)) {
        OriginateErrorEx(EINVAL, "%d", "clockState->state = %d is not implemented", clockState->state);
    }
#endif

    Call(ClockStateFunctionMap[clockState->state](clockState));

    memcpy(&(clockState->oldDateTime), &(clockState->dateTime), sizeof(DateTime));

    return 0;
}
