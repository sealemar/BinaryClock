//
// developed by Sergey Markelov (11/30/2013)
//

#include "clock_extern_functions.h"

int (* clock_setPixel)(int x, int y, Bool turnOn) = NULL;
int (* clock_uptimeMillis)(unsigned long *millis) = NULL;
int (* clock_initDateTime)(DateTime *dt) = NULL;
