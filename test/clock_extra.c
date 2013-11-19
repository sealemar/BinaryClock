//
// developed by Sergey Markelov (11/15/2013)
//
#include <memory.h>
#include "clock_extra.h"

static unsigned char Screen[CLOCK_PATTERN_SIZE] = { 0 };

int test_setPixel(int x, int y, Bool turnOn)
{
    if(x < 0 || x >= CLOCK_SCREEN_WIDTH)
        OriginateErrorEx(EINVAL, "%d", "x[%d] should be 0 < x < %d", x, CLOCK_SCREEN_WIDTH);
    if(y < 0 || y >= CLOCK_SCREEN_HEIGHT)
        OriginateErrorEx(EINVAL, "%d", "y[%d] should be 0 < y < %d", y, CLOCK_SCREEN_HEIGHT);

    if(turnOn) {
        Screen[y] |= 1 << (CLOCK_SCREEN_WIDTH - x - 1);
    } else {
        Screen[y] &= ~(1 << (CLOCK_SCREEN_WIDTH - x - 1));
    }

    return 0;
}

void test_clearScreen()
{
    memset(Screen, 0, sizeof(Screen));
}

int test_compareScreenPattern(const unsigned char pattern[CLOCK_PATTERN_SIZE])
{
    if(pattern == NULL)
        OriginateErrorEx(EINVAL, "%d", "pattern is NULL");

    return memcmp(Screen, pattern, sizeof(Screen));
}

int test_compareScreenBits(unsigned int index, unsigned char bits)
{
    if(index > CLOCK_PATTERN_SIZE)
        OriginateErrorEx(EINVAL, "%d", "index[%u] should be < %d", index, CLOCK_PATTERN_SIZE);

    return (Screen[index] == bits) ? 0 : -1;
}

unsigned char test_getScreenBits(unsigned int index)
{
    return Screen[index];
}

void test_dumpScreenBits()
{
    LogRaw(OUT_STREAM, "screen bits = { ");

    for(unsigned int i = 0; i < countof(Screen); ++i) {
        LogRaw(OUT_STREAM, "0x%02X, ", Screen[i]);
    }

    LogLnRaw(OUT_STREAM, "}");
}
