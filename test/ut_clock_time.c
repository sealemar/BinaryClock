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
// @brief BinaryClock lib/clock_time unit tests
//
// developed by Sergey Markelov (11/18/2013)
//

#include <clock_time.h>
#include "ut_clock_time.h"

static int assert_clock_updateUptimeMillis_delta(
        unsigned long updateMillis,
        unsigned long expectedDelta,
        unsigned long *lastMillis)
{
    NullCheck(lastMillis);

    unsigned long delta;
    Call(clock_updateUptimeMillis(updateMillis, lastMillis, &delta));
    assert_number(delta, expectedDelta, "%lu", "%lu");

    return 0;
}


static int test_clock_updateUptimeMillis_returnsCorrectDelta()
{
    unsigned long lastMillis;
    unsigned long delta;

    Call(clock_updateUptimeMillis(0, &lastMillis, &delta));

    Call(assert_clock_updateUptimeMillis_delta(0, 0, &lastMillis));
    Call(assert_clock_updateUptimeMillis_delta(100, 100, &lastMillis));
    Call(assert_clock_updateUptimeMillis_delta(5000, 4900, &lastMillis));

    Call(clock_updateUptimeMillis(MAX_UPTIME_MILLIS - 5, &lastMillis, &delta));
    Call(assert_clock_updateUptimeMillis_delta(MAX_UPTIME_MILLIS, 5, &lastMillis));

    return 0;
}

static int test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly()
{
    unsigned long lastMillis;
    unsigned long delta;

    Call(clock_updateUptimeMillis(MAX_UPTIME_MILLIS, &lastMillis, &delta));
    Call(assert_clock_updateUptimeMillis_delta(0, 1, &lastMillis));

    Call(clock_updateUptimeMillis(5, &lastMillis, &delta));
    Call(assert_clock_updateUptimeMillis_delta(4, MAX_UPTIME_MILLIS, &lastMillis));

    return 0;
}

static TestUnit testSuite[] = {
    { test_clock_updateUptimeMillis_returnsCorrectDelta, "clock_updateUptimeMillis() returns correct delta", FALSE },
    { test_clock_updateUptimeMillis_handlesIntegersOverflowsCorrectly, "clock_updateUptimeMillis() handles integers overflows correctly", FALSE },
};

int ut_clock_time()
{
    return runTestSuite(testSuite);
}
