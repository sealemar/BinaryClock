#include "test_ut.h"

int _runTestSuite(const char *fileName, unsigned long line, const char *func, TestUnit *testSuite, size_t testsCount)
{
    if(fileName == NULL)
        OriginateErrorEx(EINVAL, "%d", "fileName is NULL");
    if(func == NULL)
        OriginateErrorEx(EINVAL, "%d", "func is NULL");
    if(testSuite == NULL)
        OriginateErrorEx(EINVAL, "%d", "testSuite is NULL");
    if(testsCount == 0)
        OriginateErrorEx(EINVAL, "%d", "testsCount is 0");

    LogLnRaw(OUT_STREAM, "--- %s:%lu (%s) - a test suite started ---", fileName, line, func);

    int errors = 0;

    for(size_t i = 0; i < testsCount; ++i) {
        int res = testSuite[i].func();
        if(res) {
            IgnoreError(res, "%d");
            ++errors;
            testSuite[i].isFailed = TRUE;
            LogLnRaw(ERR_STREAM, "");
        }
    }

    LogLnRaw(OUT_STREAM,
             "--- %s:%lu (%s) - a test suite completed --- "
             "%zu test(s) ran. %zu succeded, %d failed",
             fileName, line, func,
             testsCount, testsCount - errors, errors);

    return errors;
}
