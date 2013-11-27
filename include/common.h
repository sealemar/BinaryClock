//
// developed by Sergey Markelov (11/10/2013)
//

#ifndef BINARY_CLOCK_INCLUDE_COMMON_H
#define BINARY_CLOCK_INCLUDE_COMMON_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

typedef enum { FALSE = 0, TRUE } Bool;

#define countof(x) (sizeof(x) / sizeof(*x))

//
// @brief Call(function) makes a function call taking PARAM_CHECKS macro into
//        consideration. If the macro is defined, ContinueError(functionResult, "%d");
//        will be called if function fails.
//        If the macro is not defined, the result will be ignored
//
#ifdef PARAM_CHECKS
#define Call(function) { \
    int _res = function; \
    if(_res) ContinueError(_res, "%d"); \
}
#else
#define Call(function) { (void)function; }
#endif

#endif
