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

//
// @brief CallOriginateErrno(function) makes a function call taking PARAM_CHECKS macro into
//        consideration. If the macro is defined, OriginateErrorEx(errno, "%d", description);
//        will be called if function fails.
//        If the macro is not defined, the result will be ignored
//
#ifdef PARAM_CHECKS
#define CallOriginateErrno(function) { \
    int _res = function; \
    if(_res) OriginateErrorEx(errno, "%d", "'%s' failed with %d", TOSTRING(function), errno); \
}
#else
#define CallOriginateErrno(function) { (void)function; }
#endif

//
// @brief Performs a NULL check on _arg_. OriginateErrorEx(EINVAL, arg + " is NULL") if it is NULL
//
#ifdef PARAM_CHECKS
#define NullCheck(arg) { \
    if(arg == NULL) \
        OriginateErrorEx(EINVAL, "%d", "%s is NULL", TOSTRING(arg)); \
}
#else
#define NullCheck(arg) ((void)(arg))
#endif

#endif
