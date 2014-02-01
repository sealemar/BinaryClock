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
// @brief BinaryClock logger
//

#ifndef GENERIC_LOGGER
#define GENERIC_LOGGER

#include <stdio.h>

// TODO: think about initializing these variables, so that I don't have to add default
//       initialization section everywhere
extern FILE *errStream;
extern FILE *outStream;

#define ERR_STREAM              errStream
#define OUT_STREAM              outStream

#define ERROR_PREFIX            "Error: "
#define STANDARD_PREFIX         ""
#define ERROR_CONTINUE_PREFIX   " ----> "

#define LogRaw(stream, format, ...) \
    { fprintf(stream, format, ##__VA_ARGS__); }

#define LogLnRaw(stream, format, ...) \
    { fprintf(stream, format "\n", ##__VA_ARGS__); }

#define _LOG(stream, prefix, format, ...) \
    { fprintf(stream, "%s (%s) - " format "\n", prefix __FILE__ ":" TOSTRING(__LINE__), __func__, ##__VA_ARGS__); }

#define LogError(format, ...) \
    { _LOG(ERR_STREAM, ERROR_PREFIX, format, ##__VA_ARGS__); }

#define Log(format, ...) \
    { _LOG(OUT_STREAM, STANDARD_PREFIX, format, ##__VA_ARGS__); }

//
// @brief this macro originates the error. Functions down the stack should use
//        ContinueErrorEx() or ContinueError() to propagate the error behavior.
//
//        In the log it will look like:
//
//        Error: example.c:35 (my_function) - [0000001b] custom error
//         ----> someFile.c:50 (process) - [0000001b]
//         ----> main.c:163 (main) - [00000001]
//
// @param result the value which needs to be logged
// @param resultSpecifier printf() specifier for the result. Ex. "%d" means @c result is of type @c int
//        you can also use "0x%08x" to make it Hex
// @param format, ... - custom formatted message
//
#define OriginateErrorEx(result, resultSpecifier, format, ...) \
    { LogError("[" resultSpecifier "] " format, result, ##__VA_ARGS__); return result; }

//
// @see OriginateErrorEx
//
#define OriginateError(result, resultSpecifier) \
    { LogError("[" resultSpecifier "] ", result); return result; }

//
// @see OriginateErrorEx
//
#define ContinueErrorEx(result, resultSpecifier, format, ...) \
    { _LOG(ERR_STREAM, ERROR_CONTINUE_PREFIX, \
           "[" resultSpecifier "] " format, result, ##__VA_ARGS__); return result; }

//
// @see OriginateErrorEx
//
#define ContinueError(result, resultSpecifier) \
    { _LOG(ERR_STREAM, ERROR_CONTINUE_PREFIX, \
           "[" resultSpecifier "] ", result); return result; }

//
// @brief operates the same to ContinueErrorEx, but doesn't return
//
#define IgnoreErrorEx(result, resultSpecifier, format, ...) \
    { _LOG(ERR_STREAM, ERROR_CONTINUE_PREFIX, \
           "[" resultSpecifier "] " format, result, ##__VA_ARGS__); }

//
// @brief operates the same to ContinueError, but doesn't return
//
#define IgnoreError(result, resultSpecifier) \
    { _LOG(ERR_STREAM, ERROR_CONTINUE_PREFIX, \
           "[" resultSpecifier "] ", result); }

#endif
