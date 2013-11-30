//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_H
#define BINARY_CLOCK_LIB_CLOCK_H

#include "date_time.h"

//
// @description Architecture dependant clock bits
// Third-party should implement these functions
//

#define ON  1
#define OFF 0

#define CLOCK_SCREEN_WIDTH     8
#define CLOCK_SCREEN_HEIGHT    8
#define CLOCK_MAX_BINARY_WIDTH 2U
#define CLOCK_MAX_BINARY_NUMBER ( (1 << CLOCK_SCREEN_HEIGHT) - 1 )

#define CLOCK_PATTERN_SIZE      (CLOCK_SCREEN_HEIGHT)

#include "alphabet.h"
#include "clock_extern.h"

//
// @brief Clears the clock screen.
//        If the implementation provides clock_extern_clearScreen(),
//        that function will be used.
//        clock_drawPattern(ClockAlphabet[CLOCK_BLANK])
//        will be used otherwise
//
#define clock_clearScreen() { \
    if(clock_extern_clearScreen != NULL) { \
        Call( clock_extern_clearScreen() ); \
    } else { \
        Call( clock_drawPattern(ClockAlphabet[CLOCK_BLANK]) ); \
    } \
}

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
// @brief displays a given number in a binary format on the clock screen
// @param number a number to display
// @param width a width of a binary bar [ 0 < width < CLOCK_MAX_BINARY_WIDTH ]
// @param pos an offset on x axis of where to start displaying the _number_ [ 0 <= pos < CLOCK_SCREEN_WIDTH - width ]
// @returns 0 on success
// EINVAL - if _number_ in binary format is taller than CLOCK_SCREEN_HEIGH bits
//          if _width_ > CLOCK_MAX_BINARY_WIDTH || width == 0
//          if _pos_ >= CLOCK_SCREEN_WIDTH - width
//
int clock_displayBinaryNumber(unsigned int number, unsigned int width, unsigned int pos);

//
// @brief displays time from a given DateTime
// @param dt a pointer to DateTime which time should be displayed
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_displayTime(const DateTime *dt);

//
// @brief displays date from a given DateTime
//        MM DD YY
//
// @param dt a pointer to DateTime which date should be displayed
// @returns 0 on success
// EINVAL - if _dt_ is NULL
//
int clock_displayDate(const DateTime *dt);

//
// @brief finds a suitable index from ClockAlphabet by a given character
// @param ch ASCII character to find an index for
// @param clockAlphabetIndex a closest suitable index from ClockAlphabet for _ch_
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
