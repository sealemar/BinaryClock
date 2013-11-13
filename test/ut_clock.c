//
// developed by Sergey Markelov (11/10/2013)
//

#include <clock.h>
#include <logger.h>
#include "ut_clock.h"

int test_clock_slidePattern_returnsCorrectResult()
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

    int isLastStep = FALSE;
    unsigned char currentPattern[CLOCK_SCREEN_HEIGHT];
    const unsigned char *patternFrom = patterns[0];
    const unsigned char *patternTo = patterns[8];
    for(unsigned char step = 0; !isLastStep; ++step)
    {
        int res = clock_slidePattern(patternFrom, patternTo, step, &isLastStep, currentPattern);
        if(res) ContinueError(res, "%d");

        // check pattern
        for(int i = 0; i < CLOCK_SCREEN_HEIGHT; ++i)
        {
            if(currentPattern[i] != patterns[step][i])
            {
                OriginateErrorEx(-1, "%d", "currentPattern[i] != patterns[step][i]. i = %d, step = %d, "
                                           "currentPattern[i] = %d, patterns[step][i] = %d",
                                 i, step, currentPattern[i], patterns[step][i]);
            }
        }
    }

    return 0;
}

int ut_clock_tests()
{
    int errors = 0;
    int res;

    res = test_clock_slidePattern_returnsCorrectResult();
    if(res) { IgnoreError(res, "%d"); ++errors; }

    Log("1 test ran. %d tests succeded, %d failed", 1 - errors, errors);

    return 0;
}
