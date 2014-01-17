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
// @brief BinaryClock tests main functionality
//
// developed by Sergey Markelov (11/15/2013)
//

#ifndef BINARY_CLOCK_TEST_ARCH_CLOCK_BITS_EXTRA_H
#define BINARY_CLOCK_TEST_ARCH_CLOCK_BITS_EXTRA_H

#include <clock.h>
#include <clock_time.h>

int test_setPixel(int x, int y, Bool turnOn);
int test_clearScreen();

//
// @brief compares the test screen to a given pattern
// @returns 0 if the screen displays the pattern exactly
//
int test_compareScreenPattern(const unsigned char pattern[CLOCK_PATTERN_SIZE]);

//
// @brief compares bits on the test screen
// @param index y pos of the screen where the row should be compared
// @param bits the actual bits to compare
// @returns 0 if bits are equal
//
int test_compareScreenBits(unsigned int index, unsigned char bits);

//
// @brief get screen bits at y pos of index
// @warning index is not checked for overflow
//
unsigned char test_getScreenBits(unsigned int index);

//
// @brief dumps the test screen bits to stdout
//
void test_dumpScreenBits();

#endif
