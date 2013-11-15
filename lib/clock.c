//
// developed by Sergey Markelov (11/10/2013)
//

#include <errno.h>
#include <string.h>

#include <logger.h>
#include "alphabet.h"
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
            clock_setPixel(x, y, ch & 1 << x);
    }

    return 0;
}

//
// @brief finds a suitable index from CLOCK_ALPHABET by a given character
// @param ch ASCII character to find an index for
// @param clockAlphabetIndex a closest suitable index from CLOCK_ALPHABET for _ch_
// @returns 0 on success
// ERANGE if an exact match for _ch_ is not found (a closest pattern will be returned, i.e.
//        if "a" is not found than an index to "A" will be returned. If a closest pattern can't be
//        identified, an index to blank patter will be returned in _clockAlphabetIndex_)
//
// EINVAL if _clockAlphabetIndex_ is NULL
//
int clock_getAlphabetIndexByCharacter(unsigned char ch, int *clockAlphabetIndex)
{
#ifdef PARAM_CHECKS
    if(clockAlphabetIndex == NULL)
        OriginateErrorEx(EINVAL, "%d", "clockAlphabetIndex is NULL");
#endif

    if(ch >= '0' && ch <= '9') {
        *clockAlphabetIndex = CLOCK_0 + (ch - '0');
        return 0;
    }
    if(ch >= 'A' && ch <= 'Z') {
        *clockAlphabetIndex = CLOCK_A + (ch - 'A');
        return 0;
    }
    if(ch >= 'a' && ch <= 'z') {
        *clockAlphabetIndex = CLOCK_A + (ch - 'a');
        return ERANGE;
    }

    switch(ch) {
        case '+': *clockAlphabetIndex = CLOCK_PLUS;             return 0;
        case '-': *clockAlphabetIndex = CLOCK_MINUS;            return 0;
        case '*': *clockAlphabetIndex = CLOCK_MULTIPLY;         return 0;
        case '/': *clockAlphabetIndex = CLOCK_SLASH;            return 0;
        case ':': *clockAlphabetIndex = CLOCK_COLON;            return 0;
        case '.': *clockAlphabetIndex = CLOCK_POINT;            return 0;
        case '!': *clockAlphabetIndex = CLOCK_EXCLAMATION_MARK; return 0;
        case ' ': *clockAlphabetIndex = CLOCK_BLANK;            return 0;
    }

    *clockAlphabetIndex = CLOCK_BLANK;
    return ERANGE;
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

#ifdef PARAM_CHECKS
    if(text == NULL)
        OriginateErrorEx(EINVAL, "%d", "text is NULL");
    if(step > lastStep)
        OriginateErrorEx(EINVAL, "%d", "step should be <= %zu for text '%s'", lastStep, text);
    if(isLastStep == NULL)
        OriginateErrorEx(EINVAL, "%d", "isLastStep is NULL");
    if(pattern == NULL)
        OriginateErrorEx(EINVAL, "%d", "pattern is NULL");
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
    clock_getAlphabetIndexByCharacter(chPos[0], &firstCharIndex);
    clock_getAlphabetIndexByCharacter(chPos[1], &secondCharIndex);

    *isLastStep = step == lastStep;

    Bool patternIsLastStep;

#ifdef PARAM_CHECKS
    int ret =
#else
    return
#endif
    clock_slidePattern(
            CLOCK_ALPHABET[firstCharIndex],
            CLOCK_ALPHABET[secondCharIndex],
            patternStep,
           &patternIsLastStep,
            pattern);
#ifdef PARAM_CHECKS
    if(ret) ContinueError(EINVAL, "%d");

    return ret;
#endif
}
