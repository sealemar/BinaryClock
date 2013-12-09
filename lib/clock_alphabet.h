//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_ALPHABET_H
#define BINARY_CLOCK_LIB_CLOCK_ALPHABET_H

#include "clock.h"

#define CLOCK_BLANK              0
#define CLOCK_0                  1
#define CLOCK_1                  2
#define CLOCK_2                  3
#define CLOCK_3                  4
#define CLOCK_4                  5
#define CLOCK_5                  6
#define CLOCK_6                  7
#define CLOCK_7                  8
#define CLOCK_8                  9
#define CLOCK_9                 10
#define CLOCK_A                 11
#define CLOCK_B                 12
#define CLOCK_C                 13
#define CLOCK_D                 14
#define CLOCK_E                 15
#define CLOCK_F                 16
#define CLOCK_G                 17
#define CLOCK_H                 18
#define CLOCK_I                 19
#define CLOCK_J                 20
#define CLOCK_K                 21
#define CLOCK_L                 22
#define CLOCK_M                 23
#define CLOCK_N                 24
#define CLOCK_O                 25
#define CLOCK_P                 26
#define CLOCK_Q                 27
#define CLOCK_R                 28
#define CLOCK_S                 29
#define CLOCK_T                 30
#define CLOCK_U                 31
#define CLOCK_V                 32
#define CLOCK_W                 33
#define CLOCK_X                 34
#define CLOCK_Y                 35
#define CLOCK_Z                 36
#define CLOCK_PLUS              37
#define CLOCK_MINUS             38
#define CLOCK_MULTIPLY          39
#define CLOCK_SLASH             40
#define CLOCK_COLON             41
#define CLOCK_POINT             42
#define CLOCK_EXCLAMATION_MARK  43
#define CLOCK_OPENING_PARENTHESES 44
#define CLOCK_CLOSING_PARENTHESES 45
#define CLOCK_TICK              46
#define CLOCK_SMILEY_FACE_SMILE 47
#define CLOCK_SMILEY_FACE_SAD   48
#define CLOCK_ALPHABET_LAST     CLOCK_SMILEY_FACE_SAD
#define CLOCK_ALPHABET_SIZE     (CLOCK_ALPHABET_LAST + 1)

extern const unsigned char ClockAlphabet[CLOCK_ALPHABET_SIZE][CLOCK_PATTERN_SIZE];

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
// @note CLOCK_SMILEY_FACE_SMILE is '\001'
//       CLOCK_SMILEY_FACE_SAD is '\002'
//
int clock_alphabet_getIndexByCharacter(unsigned char ch, int *clockAlphabetIndex);

#endif
