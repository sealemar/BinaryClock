//
// developed by Sergey Markelov (11/28/2013)
//

#define _destroyWindow(wnd) \
    if(wnd) { \
        int _res = delwin(wnd); \
        if(_res) OriginateErrorEx(_res, "%d", "delwnd(" TOSTRING(wnd) ") returned %d", _res); \
        wnd = NULL; \
    }

#define _newwin(wnd, height, width, beginY, beginX) { \
    wnd = newwin(height, width, beginY, beginX); \
    if(wnd == NULL) { \
        OriginateErrorEx(ENOMEM, "%d", "newwin(%d, %d, %d, %d) failed", height, width, beginY, beginX); \
    } \
}

#define _mvwprintw(wnd, y, x, format, ...) { \
    int _res = mvwprintw(wnd, y, x, format, ##__VA_ARGS__); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "mvwprintw(" TOSTRING(wnd) ", %d, %d, ...) failed with %d", \
                                     y, x, _res); \
    } \
}

#define _mvwaddch(wnd, y, x, ch) { \
    int _res = mvwaddch(wnd, y, x, ch); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "mvwaddch(" TOSTRING(wnd) ", %d, %d, " TOSTRING(ch) ") failed with %d", \
                                    y, x, _res); \
    } \
}

#define _wrefresh(wnd) { \
    int _res = wrefresh(wnd); \
    if(_res == ERR) { \
        OriginateErrorEx(_res, "%d", "wrefresh(" TOSTRING(wnd) ") failed with %d", _res); \
    } \
}
