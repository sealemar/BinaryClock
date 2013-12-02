//
// developed by Sergey Markelov (12/02/2013)
//

#include <clock_alphabet.h>
#include "ut_clock_alphabet.h"

static int validate_getAlphabetIndexByCharacter_ERANGE(unsigned char ch, int expectedAlphabetIndex)
{
    int index = 0;
    assert_function(clock_alphabet_getIndexByCharacter(ch, &index), ERANGE);
    if(index != expectedAlphabetIndex) {
        OriginateErrorEx(EFAULT, "%d", "Unexpected index from clock_getAlphabetIndexByCharacter('%c', &index). "
                                       "index = %d, expected = %d", ch, index, expectedAlphabetIndex);
    }

    return 0;
}

static int test_clock_alphabet_getIndexByCharacter_correct()
{
    int index;

    Call(clock_alphabet_getIndexByCharacter('\001', &index));
    assert_number(index, CLOCK_SMILEY_FACE_SMILE, "%d", "%d");

    return 0;
}

static int test_clock_alphabet_getIndexByCharacter_returns_ERANGE_andCorrectIndex()
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
    { test_clock_alphabet_getIndexByCharacter_correct, "clock_alphabet_getIndexByCharacter() correct", FALSE },
    { test_clock_alphabet_getIndexByCharacter_returns_ERANGE_andCorrectIndex, "clock_alphabet_getIndexByCharacter() returns ERANGE and correct index", FALSE },
};

int ut_clock_alphabet()
{
    return runTestSuite(testSuite);
}
