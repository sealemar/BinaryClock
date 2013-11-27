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
    (void)clockState;
    return 0;
}

static int clock_state_showTime(ClockState *clockState)
{
    (void)clockState;
    return 0;
}

static int clock_state_showDate(ClockState *clockState)
{
    (void)clockState;
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
// @brief Call this function from the main loop
// @param clockState a structure which holds the entire state of the clock
// @returns 0 on success
// EINVAL - if clockState is NULL
//
int clock_update(ClockState *clockState)
{
#ifdef PARAM_CHECKS
    if(clockState == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockState is NULL");
#endif

    unsigned long delta;

    Call(clock_updateUptimeMillis(0, &(clockState->lastUptime), &delta));
    Call(dateTime_addMillis(&(clockState->dateTime), delta));

#ifdef PARAM_CHECKS
    if(clockState->state >= countof(ClockStateFunctionMap)) {
        OriginateErrorEx(EINVAL, "%d", "clockState->state = %d is not implemented", clockState->state);
    }
#endif

    Call(ClockStateFunctionMap[clockState->state](clockState));

    return 0;
}
