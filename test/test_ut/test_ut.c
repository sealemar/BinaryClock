// Copyright [2013] [Sergey Markelov]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// @brief BinaryClock TestUT framework
//
// developed by Sergey Markelov (11/15/2013)
//

#include "test_ut.h"

int _runTestSuite(const char *fileName, unsigned long line, const char *func, TestUnit *testSuite, size_t testsCount)
{
    NullCheck(fileName);
    NullCheck(func);
    NullCheck(testSuite);
    if(testsCount == 0)
        OriginateErrorEx(EINVAL, "%d", "testsCount is 0");

    LogLnRaw(OUT_STREAM, "--- %s:%lu (%s) - a test suite started ---", fileName, line, func);

    int errors = 0;

    for(size_t i = 0; i < testsCount; ++i) {
        TestUnit *unit = &testSuite[i];
        LogLnRaw(OUT_STREAM, "Starting %s:%s --- [%s]", fileName, func, unit->functionName);
        int res = unit->func();
        if(res) {
            IgnoreError(res, "%d");
            ++errors;
            unit->isFailed = TRUE;
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
