//
// developed by Sergey Markelov (11/10/2013)
//

#include <errno.h>
#include <logger.h>
#include "clock.h"
#include "arch/clock_bits.h"

//
// @brief draws a pattern on the screen
// @param pattern should be one of defined in alphabet.h
// @see alphabet.h
//
// @returns 0 on success
// EINVAL - if _pattern_ is NULL
//
int clock_drawPattern(const unsigned char pattern[CLOCK_PATTERN_SIZE])
{
#ifdef PARAM_CHECKS
    if(pattern == NULL)
        OriginateErrorEx(EINVAL, "%d", "pattern is NULL");
#endif

    for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
    {
        const unsigned char ch = pattern[y];

        for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
            clock_setPixel(x, y, ch & 1 << (CLOCK_SCREEN_WIDTH - x - 1));
    }

    return 0;
}

//
// @brief slides pattern from right to left
// @param patternFrom right-side pattern (which initially appears on the screen)
// @param patternTo left-side pattern (which is slided to)
// @param step indicates current iteration (0 < step < CLOCK_SCREEN_WIDTH)
// @param isLastStep is output variable which if set to non zero value indicates the last iteration
// @param pattern resulting pattern will be written here. It should be of a type (unsigned char [CLOCK_SCREEN_HEIGHT])
// @returns 0 on success
// EINVAL - if _patternFrom_ is NULL or _patternTo_ is NULL
//          if _step_ > CLOCK_SCREEN_WIDTH
//          if _isLastStep_ is NULL
//
int clock_slidePattern(
        const unsigned char  patternFrom[CLOCK_PATTERN_SIZE],
        const unsigned char  patternTo[CLOCK_PATTERN_SIZE],
        unsigned char        step,
        Bool                *isLastStep,
        unsigned char        pattern[CLOCK_SCREEN_HEIGHT])
{
#ifdef PARAM_CHECKS
    if(patternFrom == NULL)
        OriginateErrorEx(EINVAL, "%d", "patternFrom is NULL");
    if(patternTo == NULL)
        OriginateErrorEx(EINVAL, "%d", "patternTo is NULL");
    if(step > CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(EINVAL, "%d", "step should be 0 < step <= CLOCK_SCREEN_WIDTH");
    if(isLastStep == NULL)
        OriginateErrorEx(EINVAL, "%d", "isLastStep is NULL");
    if(pattern == NULL)
        OriginateErrorEx(EINVAL, "%d", "pattern is NULL");
#else
    if(step > CLOCK_SCREEN_WIDTH)
    {
        *isLastStep = TRUE;
        return 0;
    }
#endif

    for(int i = 0; i < CLOCK_SCREEN_HEIGHT; ++i)
    {
        pattern[i] = patternFrom[i] << step
                   | patternTo[i]   >> (CLOCK_SCREEN_WIDTH - step);
    }

    *isLastStep = step == CLOCK_SCREEN_WIDTH;

    return 0;
}
