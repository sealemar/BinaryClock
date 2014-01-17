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
// @brief Should be included with every source code during compilation
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
