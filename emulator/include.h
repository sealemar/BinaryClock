//
// developed by Sergey Markelov (11/28/2013)
//

#define CallNcurses(function) { \
    int _res = function; \
    if(_res == ERR) { OriginateErrorEx(_res, "%d", "'%s' failed with %d", TOSTRING(function), _res); } \
}


#define _destroyWindow(wnd) \
    if(wnd) { \
        int _res = delwin(wnd); \
        if(_res == ERR) OriginateErrorEx(_res, "%d", "delwnd(" TOSTRING(wnd) ") failed with %d", _res); \
        wnd = NULL; \
    }
