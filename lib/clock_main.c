//
// developed by Sergey Markelov (11/25/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <string.h>

#include <logger.h>
#endif

#include "clock.h"
#include "clock_time.h"
#include "clock_main.h"

#define CLOCK_STATE_HELLO                   0
#define CLOCK_STATE_SHOW_TIME               1
#define CLOCK_STATE_SHOW_DATE               2
#define CLOCK_STATE_SHOW_TIME_BIG_ENDIAN    3
#define CLOCK_STATE_SHOW_DATE_BIG_ENDIAN    4

static int clock_state_hello(ClockState *clockState)
{
    const char text[] = " Hello, Sergey!!!";
    unsigned char  pattern[CLOCK_PATTERN_SIZE];
    Bool isLastStep;

    Call(clock_slideText(text, clockState->step, &isLastStep, pattern));

    Call(clock_drawPattern(pattern));

    if(isLastStep) {
        clockState->step  = 0;
        clockState->state = CLOCK_STATE_SHOW_TIME;
    } else {
        ++(clockState->step);
    }

    return 0;
}

static int clock_state_showTime(ClockState *clockState)
{
    Call(clock_displayTime(&(clockState->dateTime)));
    return 0;
}

static int clock_state_showDate(ClockState *clockState)
{
    Call(clock_displayDate(&(clockState->dateTime)));
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

//
// ClockStateFunctionMap - a static array of const function pointers
// to handle ClockState->state
//
static int (* const ClockStateFunctionMap[])(ClockState *) = {
    clock_state_hello,
    clock_state_showTime,
    clock_state_showDate,
    clock_state_showTimeBigEndian,
    clock_state_showDateBigEndian,
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

#ifdef PARAM_CHECKS
    if(clockState->state >= countof(ClockStateFunctionMap)) {
        OriginateErrorEx(EINVAL, "%d", "clockState->state = %d is not implemented", clockState->state);
    }
#endif

    Call(ClockStateFunctionMap[clockState->state](clockState));

    return 0;
}
