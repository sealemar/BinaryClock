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
// developed by Sergey Markelov (11/10/2013)
//
// @brief BinaryClock alphabet

#ifdef PARAM_CHECKS
#include <logger.h>
#endif

#include <errno.h>

#include "clock_alphabet.h"

//
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - o o - -      0x4c
// - o - o - o - -      0x54
// - o o - - o - -      0x64
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - - - - o - - -      0x08
// - - - o o - - -      0x18
// - - o - o - - -      0x28
// - - - - o - - -      0x08
// - - - - o - - -      0x08
// - - - - o - - -      0x08
// - - - - o - - -      0x08
// - - - - - - - -      0x00
//

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

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - - - - - o - -      0x04
// - - - o o - - -      0x18
// - - - - - o - -      0x04
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o o - -      0x3c
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - o - - - - - -      0x40
// - o o o o - - -      0x78
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o o o o - - -      0x78
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o o - -      0x3c
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - - - o - - - -      0x10
// - - o - o - - -      0x28
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o o o o o - -      0x7c
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - o o o - - - -      0x70
// - o - - o - - -      0x48
// - o - - o - - -      0x48
// - o o o o - - -      0x78
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o o o o o - -      0x7c
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o o o - - - -      0x70
// - o - - o - - -      0x48
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - o - - -      0x48
// - o o o - - - -      0x70
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o o o o - - -      0x78
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o o o o o - -      0x7c
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o o o o - - -      0x78
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - - - - - - - -      0x00
//

//
// - - o o o o - -      0x3c
// - o - - - - - -      0x44
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - o o - -      0x4c
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o o o o o - -      0x7c
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - - - - o o - -      0x0c
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - - - - o - -      0x04
// - - o - - o - -      0x24
// - - - o o - - -      0x18
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - o - - -      0x48
// - o - o - - - -      0x50
// - o o - - - - -      0x60
// - o - o - - - -      0x50
// - o - - o - - -      0x48
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o o o o o - -      0x7c
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o o - o o - -      0x6c
// - o - o - o - -      0x54
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o o - - o - -      0x64
// - o o - - o - -      0x64
// - o - o - o - -      0x54
// - o - - o o - -      0x4c
// - o - - o o - -      0x4c
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o o o o - - -      0x78
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o o o o - - -      0x78
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - o - - - - - -      0x40
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - o - o - -      0x54
// - o - o - o - -      0x54
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o o o o - - -      0x78
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o o o o - - -      0x78
// - o o - - - - -      0x60
// - o - o - - - -      0x50
// - o - - o o - -      0x4c
// - - - - - - - -      0x00
//

//
// - - o o o - - -      0x38
// - o - - - o - -      0x44
// - o - - - - - -      0x40
// - - o o o - - -      0x38
// - - - - - o - -      0x04
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o o o - - -      0x38
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o - o - - -      0x28
// - - - o - - - -      0x10
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - o - o - -      0x54
// - o - o - o - -      0x54
// - o - o - o - -      0x54
// - - o - o - - -      0x28
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o - o - - -      0x28
// - - - o - - - -      0x10
// - - o - o - - -      0x28
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - - - - - - -      0x00
//

//
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - o - - - o - -      0x44
// - - o - o - - -      0x28
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - - - - - -      0x00
//

//
// - o o o o o - -      0x7c
// - - - - - o - -      0x04
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - o - - - - -      0x20
// - o - - - - - -      0x40
// - o o o o o - -      0x7c
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - o o o o o - -      0x7c
// - - - o - - - -      0x10
// - - - o - - - -      0x10
// - - - - - - - -      0x00
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - o o o o o o -      0x7e
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - o - - - o - -      0x44
// - - o - o - - -      0x28
// - - - o - - - -      0x10
// - - o - o - - -      0x28
// - o - - - o - -      0x44
// - - - - - - - -      0x00
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - - - - - - o -      0x02
// - - - - - o - -      0x04
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - o - - - - -      0x20
// - o - - - - - -      0x40
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - - - - -      0x00
//

//
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - - - - -      0x00
//

//
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - - - - -      0x00
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - - - - -      0x00
//

//
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - o - - - - -      0x20
// - - o - - - - -      0x20
// - - o - - - - -      0x20
// - - - o - - - -      0x10
// - - - - o - - -      0x08
// - - - - - - - -      0x00
//

//
// - - o - - - - -      0x20
// - - - o - - - -      0x10
// - - - - o - - -      0x08
// - - - - o - - -      0x08
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - o - - - - -      0x20
// - - - - - - - -      0x00
//

//
// - - - o o - - -      0x18
// - - - o o - - -      0x18
// - - - - o - - -      0x08
// - - - o - - - -      0x10
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
// - - - - - - - -      0x00
//

//
// - - o o o o - -      0x3c
// - o - - - - o -      0x42
// o - o - - o - o      0xa5
// o - - - - - - o      0x81
// o - o - - o - o      0xa5
// o - - o o - - o      0x99
// - o - - - - o -      0x42
// - - o o o o - -      0x3c
//

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

const unsigned char ClockAlphabet[CLOCK_ALPHABET_SIZE][CLOCK_PATTERN_SIZE] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },     // blank
    { 0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38, 0x00 },     // 0
    { 0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x00 },     // 1
    { 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c, 0x00 },     // 2
    { 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38, 0x00 },     // 3
    { 0x44, 0x44, 0x44, 0x3c, 0x04, 0x04, 0x04, 0x00 },     // 4
    { 0x7c, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38, 0x00 },     // 5
    { 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00 },     // 6
    { 0x7c, 0x04, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00 },     // 7
    { 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00 },     // 8
    { 0x38, 0x44, 0x44, 0x3c, 0x04, 0x04, 0x38, 0x00 },     // 9
    { 0x10, 0x28, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x00 },     //  A
    { 0x70, 0x48, 0x48, 0x78, 0x44, 0x44, 0x7c, 0x00 },     //  B
    { 0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00 },     //  C
    { 0x70, 0x48, 0x44, 0x44, 0x44, 0x48, 0x70, 0x00 },     //  D
    { 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c, 0x00 },     //  E
    { 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x00 },     //  F
    { 0x3c, 0x44, 0x40, 0x40, 0x4c, 0x44, 0x38, 0x00 },     //  G
    { 0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, 0x00 },     //  H
    { 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00 },     //  I
    { 0x0c, 0x04, 0x04, 0x04, 0x04, 0x24, 0x18, 0x00 },     //  J
    { 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00 },     //  K
    { 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x00 },     //  L
    { 0x44, 0x6c, 0x54, 0x44, 0x44, 0x44, 0x44, 0x00 },     //  M
    { 0x44, 0x64, 0x64, 0x54, 0x4c, 0x4c, 0x44, 0x00 },     //  N
    { 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00 },     //  O
    { 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00 },     //  P
    { 0x38, 0x44, 0x44, 0x44, 0x54, 0x54, 0x38, 0x00 },     //  Q
    { 0x78, 0x44, 0x44, 0x78, 0x60, 0x50, 0x4c, 0x00 },     //  R
    { 0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x00 },     //  S
    { 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00 },     //  T
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00 },     //  U
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00 },     //  V
    { 0x44, 0x44, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00 },     //  W
    { 0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00 },     //  X
    { 0x44, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x00 },     //  Y
    { 0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00 },     //  Z
    { 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00 },     // +
    { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00 },     // -
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00 },     // *
    { 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00 },     // /
    { 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00 },     // :
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 },     // .
    { 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00 },     // !
    { 0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00 },     // (
    { 0x20, 0x10, 0x08, 0x08, 0x08, 0x10, 0x20, 0x00 },     // )
    { 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00 },     // '
    { 0x3c, 0x42, 0xa5, 0x81, 0xa5, 0x99, 0x42, 0x3c },     // smiley face smile
    { 0x3c, 0x42, 0xa5, 0x81, 0x99, 0xa5, 0x42, 0x3c },     // smiley face sad
};

//
// @brief finds a suitable index from ClockAlphabet by a given character
// @param ch ASCII character to find an index for
// @param clockAlphabetIndex a closest suitable index from ClockAlphabet for _ch_
// @returns 0 on success
// ERANGE if an exact match for _ch_ is not found (a closest pattern will be returned, i.e.
//        if "a" is not found than an index to "A" will be returned. If a closest pattern can't be
//        identified, an index to blank patter will be returned in _clockAlphabetIndex_)
//
// EINVAL if _clockAlphabetIndex_ is NULL
//
int clock_alphabet_getIndexByCharacter(unsigned char ch, int *clockAlphabetIndex)
{
    NullCheck(clockAlphabetIndex);

    if(ch >= '0' && ch <= '9') {
        *clockAlphabetIndex = CLOCK_0 + (ch - '0');
        return 0;
    }
    if(ch >= 'A' && ch <= 'Z') {
        *clockAlphabetIndex = CLOCK_A + (ch - 'A');
        return 0;
    }
    if(ch >= 'a' && ch <= 'z') {
        *clockAlphabetIndex = CLOCK_A + (ch - 'a');
        return ERANGE;
    }

    switch(ch) {
        case '+': *clockAlphabetIndex = CLOCK_PLUS;                return 0;
        case '-': *clockAlphabetIndex = CLOCK_MINUS;               return 0;
        case '*': *clockAlphabetIndex = CLOCK_MULTIPLY;            return 0;
        case '/': *clockAlphabetIndex = CLOCK_SLASH;               return 0;
        case ':': *clockAlphabetIndex = CLOCK_COLON;               return 0;
        case '.': *clockAlphabetIndex = CLOCK_POINT;               return 0;
        case '!': *clockAlphabetIndex = CLOCK_EXCLAMATION_MARK;    return 0;
        case '(': *clockAlphabetIndex = CLOCK_OPENING_PARENTHESES; return 0;
        case ')': *clockAlphabetIndex = CLOCK_CLOSING_PARENTHESES; return 0;
        case '\'': *clockAlphabetIndex = CLOCK_TICK;               return 0;
        case ' ': *clockAlphabetIndex = CLOCK_BLANK;               return 0;
        case 001: *clockAlphabetIndex = CLOCK_SMILEY_FACE_SMILE;   return 0;
        case 002: *clockAlphabetIndex = CLOCK_SMILEY_FACE_SAD;     return 0;
    }

    *clockAlphabetIndex = CLOCK_BLANK;
    return ERANGE;
}
