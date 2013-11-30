//
// developed by Sergey Markelov (11/25/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <logger.h>
#endif

#include <string.h>

#include "clock.h"
#include "clock_time.h"
#include "clock_main.h"

//
// In milliseconds
// These numbers mean "not earlier than", because the actual time
// depends on the speed of the program main loop and the delay in
// the main loop.
//
#define CLOCK_ANIMATION_TEXT_STEP_TIME                  100U
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
    if(doClearScreen) { Call(clock_clearScreen()); } \
}

//
// @brief Adjusts days after the month was changed. If the new month has fewer days
//        than the old one, then dateTime.day will be set to the new max.
//
// @param dateTime a dereferenced DateTime
//
#define adjustDays(dateTime) { \
    int _d; \
    Call(date_time_daysInMonth( &(dateTime), &_d )); \
    if( (dateTime).day > _d ) { \
        (dateTime).day = _d; \
    } \
}

//
// This text will be shown at clock_state_hello
//
static const char StateHelloText[] = " Hello Sergey!!!";

// TODO: think about DateTimeInit
static const DateTime DateTimeInit = { 2013, NOVEMBER, 29, 20, 7, 52, 0 };

//
// @brief A helper function to slide a text
// @param clockState
// @param text a text to slide
// @param nextState which state should be the next after the text sliding is finished
// @param nextStepMillis if the next step has animation step time, this should be set to that value, so
//        that the first frame will be shown immediately
//
// @returns 0 on ok
//
static int slideText(
        ClockState *clockState,
        const char *text,
        unsigned int nextState,
        unsigned int nextStepMillis)
{
    updateStepTime(clockState, CLOCK_ANIMATION_TEXT_STEP_TIME);

    unsigned char pattern[CLOCK_PATTERN_SIZE];
    Bool isLastStep;

    Call(clock_slideText(text, clockState->step, &isLastStep, pattern));

    Call(clock_drawPattern(pattern));

    if(!isLastStep) {
        ++(clockState->step);
    } else {
        setClockState(clockState, nextState, nextStepMillis, TRUE);
    }

    return 0;
}

static int clock_state_hello(ClockState *clockState)
{
    //
    // Click on CLOCK_BUTTON_SET skips this step
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, TRUE);
        return 0;
    }

    Call(slideText(clockState, StateHelloText, CLOCK_STATE_SHOW_TIME, 0));

    return 0;
}

static int clock_state_showTime(ClockState *clockState)
{
    //
    // If CLOCK_BUTTON_INFO was clicked -- show time in text
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_INFO)) {
        setClockState(clockState, CLOCK_STATE_SHOW_TIME_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);
        return 0;
    }

    //
    // Need to change state to set time?
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        setClockState(clockState, CLOCK_STATE_SET_TIME, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME, FALSE);
        return 0;
    }

    //
    // Show date
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        setClockState(clockState, CLOCK_STATE_SHOW_DATE, 0, TRUE);
        return 0;
    }

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
    //
    // If CLOCK_BUTTON_INFO was clicked -- show date in text
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_INFO)) {
        setClockState(clockState, CLOCK_STATE_SHOW_DATE_BIG_ENDIAN, CLOCK_ANIMATION_TEXT_STEP_TIME, TRUE);
        return 0;
    }

    //
    // Need to change state to set date?
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_SET)) {
        setClockState(clockState, CLOCK_STATE_SET_DATE, CLOCK_ANIMATION_BLINK_BINARY_NUMBER_STEP_TIME, FALSE);
        return 0;
    }

    //
    // Show time
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_LEFT)) {
        setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, TRUE);
        return 0;
    }

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
    //
    // If CLOCK_BUTTON_INFO was clicked -- show time in binary
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_INFO)) {
        setClockState(clockState, CLOCK_STATE_SHOW_TIME, 0, TRUE);
        return 0;
    }

    //
    // Prepare the text if this is step 0
    //
    if(clockState->step == 0) {
        clockState->text[0] = ' ';
        Call(date_time_timeToStr( &(clockState->dateTime), clockState->text + 1 ));
    }

    Call(slideText(clockState, clockState->text, CLOCK_STATE_SHOW_TIME, 0));

    return 0;
}

static int clock_state_showDateBigEndian(ClockState *clockState)
{
    //
    // If CLOCK_BUTTON_INFO was clicked -- show date in binary
    //
    if(clock_button_wasClicked(clockState->buttons, CLOCK_BUTTON_INFO)) {
        setClockState(clockState, CLOCK_STATE_SHOW_DATE, 0, TRUE);
        return 0;
    }

    //
    // Prepare the text if this is step 0
    //
    if(clockState->step == 0) {
        clockState->text[0] = ' ';
        Call(date_time_dateToStr( &(clockState->dateTime), clockState->text + 1 ));
    }

    Call(slideText(clockState, clockState->text, CLOCK_STATE_SHOW_DATE, 0));

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
                Call(date_time_daysInMonth(dt, &d));
                dt->day = d;
            }
        } else if(clockState->step == 4 || clockState->step == 5) {
            --(dt->year);
            if(dt->year < MIN_YEAR) {
                dt->year = MAX_YEAR;
            }
            adjustDays(clockState->dateTime);
        }
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
            Call(date_time_daysInMonth(dt, &d));
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
    memcpy( &(clockState->dateTime), &DateTimeInit, sizeof(DateTime));
    memcpy( &(clockState->oldDateTime), &DateTimeInit, sizeof(DateTime));

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
    Call(date_time_addMillis(&(clockState->dateTime), millis));

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
