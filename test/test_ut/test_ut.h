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


//
// @brief asserts if _str_ != _expected_
//
#define assert_str(str, expected) { \
    if(strcmp(str, expected)) { \
        OriginateErrorEx(EFAULT, "%d", "'" TOSTRING(str) "' [%s] != '" TOSTRING(expected) "' [%s]", str, expected); \
    } \
}

//
// @brief asserts if _number_ != _expected_
//
#define assert_number(number, expected, numberFormatSpec, expectedFormatSpec) { \
    if(number != expected) { \
        OriginateErrorEx(EFAULT, "%d", "'" TOSTRING(number) "' [" numberFormatSpec "] != '" TOSTRING(expected) \
                                       "' [" expectedFormatSpec "]", number, expected ); \
    } \
}

#define assert_int(number, expected) { assert_number(number, expected, "%d", "%d"); }

//
// @brief asserts if the _value_ is true
//
#define assert_true(value) { \
    if(!value) { \
        OriginateErrorEx(EFAULT, "%d", "'" TOSTRING(value) "' is false. Expected true" ); \
    } \
}

#define assert_false(value) { \
    if(value) { \
        OriginateErrorEx(EFAULT, "%d", "'" TOSTRING(value) "' is true. Expected false" ); \
    } \
}


//
// @brief asserts if _function_ returns _expectedErrorCode_
// @param function is the function itself, not an error code which it returns.
//        This macro will make the function call. The function must return int
// @param expectedErrorCode an expected error code fromt the _function_
//
#define assert_function(function, expectedErrorCode) { \
    int _res = function; \
    if(_res != (expectedErrorCode) ) { \
        OriginateErrorEx(EFAULT, "%d", "Unexpected error code [%d], %d was expected. " \
                                       "From '%s'", \
                                       _res, (expectedErrorCode), TOSTRING(function)); \
    } \
}

#endif
