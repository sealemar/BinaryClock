//
// developed by Sergey Markelov (11/15/2013)
//

#ifndef BINARY_CLOCK_TEST_UT_H
#define BINARY_CLOCK_TEST_UT_H

//
// @brief TestUnit stucture describes a test unit, a test function in other words
// @example
// TestUnit testSuite[] = {
//     { test_clock_slidePattern_returnsCorrectResult, "test_clock_slidePattern_returnsCorrectResult", FALSE },
//     { test_clock_drawPattern_returnsCorrectResult, "test_clock_drawPattern_returnsCorrectResult", FALSE },
// };
typedef struct {
    int (*func)();
    const char *functionName;
    Bool isFailed;
} TestUnit;

//
// @brief an internal function, it's better to use runTestSuite macro
//
int _runTestSuite(const char *fileName, unsigned long line, const char *func, TestUnit *testSuite, size_t testsCount);

//
// @brief run a testSuite
// @param testSuite a locally defined array of TestUnit elements.
//        Must be defined in static or automatic memory for countof(testSuite)
//        to work correctly
//
#define runTestSuite(testSuite) \
    _runTestSuite(__FILE__, __LINE__, __func__, testSuite, countof(testSuite))

#endif
