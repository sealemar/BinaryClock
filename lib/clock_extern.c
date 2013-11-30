//
// developed by Sergey Markelov (11/30/2013)
//

#include "clock_extern.h"

int (* clock_extern_setPixel)(int x, int y, Bool turnOn) = NULL;
int (* clock_extern_uptimeMillis)(unsigned long *millis) = NULL;
int (* clock_extern_clearScreen)(void) = NULL;
int (* clock_extern_initDateTime)(DateTime *dt) = NULL;
