//
// developed by Sergey Markelov (11/10/2013)
//

#include <string.h>

#include <alphabet.h>
#include "test.h"
#include "ut_clock.h"

static int validatePattern(
    const unsigned char expected[CLOCK_PATTERN_SIZE],
    const unsigned char actual[CLOCK_PATTERN_SIZE])
{
    for(int i = 0; i < CLOCK_SCREEN_HEIGHT; ++i) {
        if(expected[i] != actual[i]) {
            OriginateErrorEx(-1, "%d", "expected[%d] (%02x) != actual[%d] (%02x)", i, expected[i], i, actual[i]);
        }
    }

    return 0;
}

static int test_clock_slidePattern_returnsCorrectResult()
{
// step 0
//
// - - o o o o - -      0x3c
// - o - - - - o -      0x42
// o - o - - o - o      0xa5
// o - - - - - - o      0x81
// o - - o o - - o      0x99
// o - o - - o - o      0xa5
// - o - - - - o -      0x42
// - - o o o o - -      0x3c
//

// step 1
//
// - o o o o - - -      0x78
// o - - - - o - -      0x84
// - o - - o - o -      0x4a
// - - - - - - o -      0x02
// - - o o - - o -      0x32
// - o - - o - o -      0x4a
// o - - - - o - -      0x84
// - o o o o - - -      0x78
//

// step 2
//
// o o o o - - - -      0xf0
// - - - - o - - o      0x09
// o - - o - o - -      0x94
// - - - - - o - -      0x04
// - o o - - o - -      0x64
// o - - o - o - -      0x94
// - - - - o - - o      0x09
// o o o o - - - -      0xf0
//

// step 3
//
// o o o - - - - o      0xe1
// - - - o - - o -      0x12
// - - o - o - - -      0x28
// - - - - o - - -      0x08
// o o - - o - - -      0xc8
// - - o - o - - o      0x29
// - - - o - - o o      0x13
// o o o - - - - -      0xe0
//

// step 4
//
// o o - - - - o o      0xc3
// - - o - - o - -      0x24
// - o - o - - - -      0x50
// - - - o - - - -      0x10
// o - - o - - - o      0x91
// - o - o - - o -      0x52
// - - o - - o o o      0x27
// o o - - - - - -      0xc0
//

// step 5
//
// o - - - - o o o      0x87
// - o - - o - - -      0x48
// o - o - - - - -      0xa0
// - - o - - - - o      0x21
// - - o - - - o -      0x22
// o - o - - o - -      0xa4
// - o - - o o o o      0x4f
// o - - - - - - -      0x80
//

// step 6
//
// - - - - o o o -      0x0e
// o - - o - - - o      0x91
// - o - - - - - o      0x41
// - o - - - - o -      0x42
// - o - - - o - -      0x44
// - o - - o - - -      0x48
// o - - o o o o o      0x9f
// - - - - - - - -      0x00
//

// step 7
//
// - - - o o o - -      0x1c
// - - o - - - o -      0x22
// o - - - - - o -      0x82
// o - - - - o - -      0x84
// o - - - o - - -      0x88
// o - - o - - - -      0x90
// - - o o o o o -      0x3e
// - - - - - - - -      0x00
//

// step 8
//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - - - - - o - -      0x04
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - o - - - - -      0x20
// - o o o o o - -      0x7c
// - - - - - - - -      0x00
//


    const unsigned char patterns[][CLOCK_SCREEN_HEIGHT] = {
        { 0x3c, 0x42, 0xa5, 0x81, 0x99, 0xa5, 0x42, 0x3c },
        { 0x78, 0x84, 0x4a, 0x02, 0x32, 0x4a, 0x84, 0x78 },
        { 0xf0, 0x09, 0x94, 0x04, 0x64, 0x94, 0x09, 0xf0 },
        { 0xe1, 0x12, 0x28, 0x08, 0xc8, 0x29, 0x13, 0xe0 },
        { 0xc3, 0x24, 0x50, 0x10, 0x91, 0x52, 0x27, 0xc0 },
        { 0x87, 0x48, 0xa0, 0x21, 0x22, 0xa4, 0x4f, 0x80 },
        { 0x0e, 0x91, 0x41, 0x42, 0x44, 0x48, 0x9f, 0x00 },
        { 0x1c, 0x22, 0x82, 0x84, 0x88, 0x90, 0x3e, 0x00 },
        { 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c, 0x00 }
    };

    Bool isLastStep = FALSE;
    unsigned char currentPattern[CLOCK_SCREEN_HEIGHT];
    const unsigned char *patternFrom = patterns[0];
    const unsigned char *patternTo = patterns[8];
    for(unsigned char step = 0; !isLastStep; ++step)
    {
        Call(clock_slidePattern(patternFrom, patternTo, step, &isLastStep, currentPattern));

        int res = validatePattern(patterns[step], currentPattern);
        if(res) ContinueErrorEx(res, "%d", "step = %d", step);
    }

    return 0;
}

static int test_clock_drawPattern_correct()
{
    Call(test_clearScreen());

    Call(clock_drawPattern(ClockAlphabet[CLOCK_1]));

    int res = test_compareScreenPattern(ClockAlphabet[CLOCK_1]);
    if(res) OriginateErrorEx(res, "%d", "pattern ClockAlphabet[CLOCK_1] doesn't match the screen");

    return 0;
}

static int test_clock_displayBinaryNumber_correct()
{
    unsigned char pat = 3 << 4;

    Call(test_clearScreen());

    for(unsigned int i = 0, last = (1 << CLOCK_SCREEN_HEIGHT); i < last; ++i) {
        int res = clock_displayBinaryNumber(i, 2, 2);
        if(res) ContinueError(res, "%d");

        for(unsigned int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y) {
            unsigned char expected = (i & 1U << y) ? pat : 0;
            unsigned int yReverted = CLOCK_SCREEN_HEIGHT - y - 1;
            res = test_compareScreenBits(yReverted, expected);
            if(res) {
                test_dumpScreenBits();
                OriginateErrorEx(res, "%d", "failed testing %u. Bits %u. Expected = 0x%02X. Actual = 0x%02X",
                                            i, y, expected, test_getScreenBits(yReverted));
            }
        }
    }

    return 0;
}

static int test_clock_slideText_correct()
{
    const char text[] = "TH1S TEXT2TEXT:SHOULD.BE-CORRECTLY*PROCESSED!!!";
    int res;

    for(size_t i = 0, len = strlen(text); i < len; ++i) {
        Bool isLastStep;
        unsigned char pattern[CLOCK_PATTERN_SIZE];

        res = clock_slideText(text, i * CLOCK_SCREEN_WIDTH, &isLastStep, pattern);
        if(res) ContinueError(res, "%d");

        if((i == len -1 && !isLastStep) || (isLastStep && i != len -1)) {
            OriginateErrorEx(-1, "%d", "unexpected isLastStep = %s. i = %zu, len = %zu, text = '%s'",
                                       isLastStep ? "true" : "false", i, len, text);
        }

        int index;
        res = clock_getAlphabetIndexByCharacter(text[i], &index);
        if(res) ContinueError(res, "%d");

        res = validatePattern(ClockAlphabet[index], pattern);
        if(res) ContinueErrorEx(res, "%d", "pattern is not valid. i = %zu, text = '%s', index = %d", i, text, index);
    }

    return 0;
}

int validate_getAlphabetIndexByCharacter_ERANGE(unsigned char ch, int expectedAlphabetIndex)
{
    int index = 0;
    assert_function(clock_getAlphabetIndexByCharacter(ch, &index), ERANGE);
    if(index != expectedAlphabetIndex) {
        OriginateErrorEx(EFAULT, "%d", "Unexpected index from clock_getAlphabetIndexByCharacter('%c', &index). "
                                       "index = %d, expected = %d", ch, index, expectedAlphabetIndex);
    }

    return 0;
}

static int test_clock_getAlphabetIndexByCharacter_returns_ERANGE_andCorrectIndex()
{
    for(unsigned char ch = 'a'; ch <= 'z'; ++ch) {
        Call(validate_getAlphabetIndexByCharacter_ERANGE(ch, CLOCK_A + (ch - 'a')));
    }

    //
    // Test a random character which is not defined in the alphabet.
    // Expected resulting index is CLOCK_BLANK.
    //
    Call(validate_getAlphabetIndexByCharacter_ERANGE('~', CLOCK_BLANK));

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_slidePattern_returnsCorrectResult, "clock_slidePattern() returns correct result", FALSE },
    { test_clock_drawPattern_correct, "clock_drawPattern() correct", FALSE },
    { test_clock_displayBinaryNumber_correct, "clock_displayBinaryNumber() correct", FALSE },
    { test_clock_slideText_correct, "clock_slideText() correct", FALSE },
    { test_clock_getAlphabetIndexByCharacter_returns_ERANGE_andCorrectIndex, "clock_getAlphabetIndexByCharacter() returns ERANGE and correct index", FALSE },
};

int ut_clock()
{
    return runTestSuite(testSuite);
}
