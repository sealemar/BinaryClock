//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_H
#define BINARY_CLOCK_LIB_CLOCK_H

#define CLOCK_SCREEN_WIDTH     8
#define CLOCK_SCREEN_HEIGHT    8

//
// @brief draws a pattern on the screen
// @param pattern is one of CLOCK_ALPHABET[CLOCK_*] from alphabet.h
// @returns 0 on success
// EINVAL - if _pattern_ is NULL
//
int drawPattern(const unsigned char* pattern);

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
int slidePattern(
        const unsigned char *patternFrom,
        const unsigned char *patternTo,
        unsigned char        step,
        int                 *isLastStep,
        unsigned char        pattern[CLOCK_SCREEN_HEIGHT]);

#endif
