//
// developed by Sergey Markelov (11/10/2013)
//

#ifdef PARAM_CHECKS
#include <errno.h>
#include <string.h>

#include <logger.h>
#endif

#include "clock.h"
#include "clock_state.h" // for MIN_YEAR

#define DATE_TIME_BINARY_WIDTH 2

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
    NullCheck(pattern);

    for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
    {
        const unsigned char ch = pattern[y];

        for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
            clock_extern_setPixel(x, y, ch & 1 << (CLOCK_SCREEN_WIDTH - x - 1));
    }

    return 0;
}

//
// @brief displays a given number in a binary format on the clock screen
// @param number a number to display
// @param width a width of a binary bar [ 0 < width < CLOCK_MAX_BINARY_WIDTH ]
// @param pos an offset on x axis of where to start displaying the _number_ [ 0 <= pos < CLOCK_SCREEN_WIDTH - width ]
// @returns 0 on success
// EINVAL - if _number_ in binary format is taller than CLOCK_SCREEN_HEIGH bits
//          if _width_ > CLOCK_MAX_BINARY_WIDTH || width == 0
//          if _pos_ >= CLOCK_SCREEN_WIDTH - width
//
int clock_displayBinaryNumber(unsigned int number, unsigned int width, unsigned int pos)
{
#ifdef PARAM_CHECKS
    if(number >> CLOCK_SCREEN_HEIGHT)
        OriginateErrorEx(EINVAL, "%d", "number[%u] should be < %u", number, (1 << CLOCK_SCREEN_HEIGHT));
    if(width > CLOCK_MAX_BINARY_WIDTH || width == 0)
        OriginateErrorEx(EINVAL, "%d", "width[%u] should be 0 < width < %u", width, CLOCK_MAX_BINARY_WIDTH);
    if(pos > CLOCK_SCREEN_WIDTH - width)
        OriginateErrorEx(EINVAL, "%d", "pos[%u] should be < %u", pos, CLOCK_SCREEN_WIDTH - width);
#endif

    for(unsigned int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y) {
        for(unsigned int x = pos, xLast = pos + width; x < xLast; ++x) {
            Call(clock_extern_setPixel(x, CLOCK_SCREEN_HEIGHT - y - 1, number & 1U << y));
        }
    }

    return 0;
}

//
// @brief displays time from a given DateTime
// @param dt a pointer to DateTime which time should be displayed
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_displayTime(const DateTime *dt)
{
    NullCheck(dt);

    Call(clock_displayBinaryNumber(dt->hour,   DATE_TIME_BINARY_WIDTH, 0));
    Call(clock_displayBinaryNumber(dt->minute, DATE_TIME_BINARY_WIDTH, DATE_TIME_BINARY_WIDTH + 1));
    Call(clock_displayBinaryNumber(dt->second, DATE_TIME_BINARY_WIDTH, ((DATE_TIME_BINARY_WIDTH) * 2) + 2));

    return 0;
}

//
// @brief displays date from a given DateTime
// @param dt a pointer to DateTime which date should be displayed
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_displayDate(const DateTime *dt)
{
    NullCheck(dt);

    Call(clock_displayBinaryNumber(dt->month + 1, DATE_TIME_BINARY_WIDTH, 0));
    Call(clock_displayBinaryNumber(dt->day, DATE_TIME_BINARY_WIDTH, DATE_TIME_BINARY_WIDTH + 1));
    Call(clock_displayBinaryNumber(dt->year - MIN_YEAR, DATE_TIME_BINARY_WIDTH, ((DATE_TIME_BINARY_WIDTH) * 2) + 2));

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
        unsigned char        pattern[CLOCK_PATTERN_SIZE])
{
    NullCheck(patternFrom);
    NullCheck(patternTo);
    NullCheck(isLastStep);
    NullCheck(pattern);
#ifdef PARAM_CHECKS
    if(step > CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(EINVAL, "%d", "step should be 0 < step[%u] <= CLOCK_SCREEN_WIDTH[%d]", step, CLOCK_SCREEN_WIDTH);
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

//
// @brief slides _text_ from right to left the same way that clock_slidePattern
//        slides a pattern. In fact, this function uses clock_slidePattern
//        internally.
//
// @param text a text to slide
// @param step indicates current iteration [ step <= CLOCK_SCREEN_WIDTH * (strlen(text) - 1) ]
// @param isLastStep is an output variable which if set to non zero value indicates the last iteration
// @param pattern resulting pattern will be written here.
// @returns 0 on success
// EINVAL - if _text_ is NULL
//          if _step_ > CLOCK_SCREEN_WIDTH * (strlen(text) - 1)
//          if _isLastStep_ is NULL
//
int clock_slideText(
        const char    *text,
        size_t         step,
        Bool          *isLastStep,
        unsigned char  pattern[CLOCK_PATTERN_SIZE])
{
    size_t lastStep = CLOCK_SCREEN_WIDTH * (strlen(text) - 1);

    NullCheck(text);
    NullCheck(isLastStep);
    NullCheck(pattern);
#ifdef PARAM_CHECKS
    if(step > lastStep)
        OriginateErrorEx(EINVAL, "%d", "step[%zu] should be <= %zu for text '%s'", step, lastStep, text);
#else
    if(step > lastStep) {
        *isLastStep = TRUE;
        return 0;
    }
#endif

    const char *chPos = text + (step / CLOCK_SCREEN_WIDTH);
    const unsigned char patternStep = step % CLOCK_SCREEN_WIDTH;

    int firstCharIndex  = CLOCK_BLANK;
    int secondCharIndex = CLOCK_BLANK;
    clock_alphabet_getIndexByCharacter(chPos[0], &firstCharIndex);
    clock_alphabet_getIndexByCharacter(chPos[1], &secondCharIndex);

    *isLastStep = step == lastStep;

    Bool patternIsLastStep;

    Call(clock_slidePattern(
            ClockAlphabet[firstCharIndex],
            ClockAlphabet[secondCharIndex],
            patternStep,
           &patternIsLastStep,
            pattern));

    return 0;
}
