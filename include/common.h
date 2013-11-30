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
// @brief Calls any malloc related function and checks if the result is NULL
// @param var a variable which will get the result
// @param function a function which allocates memory and returns NULL on failure
//
#ifdef PARAM_CHECKS
#define CallMalloc(var, function) { \
    var = function; \
    if(var == NULL) { \
        OriginateErrorEx(ENOMEM, "%d", "'%s' failed - no memory", TOSTRING(function)); \
    } \
}
#else
#define CallMalloc(var, function) { var = function; }
#endif

//
// @brief Calls any malloc related function and checks if the result is NULL
// @param function a function which allocates memory and returns NULL on failure
//
// @note This macro doesn't return the result.
//       Example of usage -- initscr() from curses library
//
#ifdef PARAM_CHECKS
#define CallMallocQuiet(function) { \
    void *_var = function; \
    if(_var == NULL) { \
        OriginateErrorEx(ENOMEM, "%d", "'%s' failed - no memory", TOSTRING(function)); \
    } \
}
#else
#define CallMallocQuiet(function) { function; }
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
