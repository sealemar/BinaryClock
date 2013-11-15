//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_H
#define BINARY_CLOCK_LIB_CLOCK_H

#include "arch/clock_bits.h"

//
// @brief draws a pattern on the screen
// @param pattern should be one of defined in alphabet.h
// @see alphabet.h
//
// @returns 0 on success
// EINVAL - if _pattern_ is NULL
//
int clock_drawPattern(const unsigned char pattern[CLOCK_PATTERN_SIZE]);

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
int clock_getAlphabetIndexByCharacter(unsigned char ch, int *clockAlphabetIndex);

//
// @brief slides pattern from right to left
// @param patternFrom right-side pattern (which initially appears on the screen)
// @param patternTo left-side pattern (which is slided to)
// @param step indicates current iteration (0 < step < CLOCK_SCREEN_WIDTH)
// @param isLastStep is an output variable which if set to non zero value indicates the last iteration
// @param pattern resulting pattern will be written here.
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
        unsigned char        pattern[CLOCK_PATTERN_SIZE]);

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
        unsigned char  pattern[CLOCK_PATTERN_SIZE]);
#endif
