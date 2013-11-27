
//
// developed by Sergey Markelov (11/25/2013)
//

#ifndef BINARY_CLOCK_LIB_CLOCK_STATE_H
#define BINARY_CLOCK_LIB_CLOCK_STATE_H

#include "date_time.h"

typedef struct {
    unsigned int state;
    int step;
    unsigned long lastUptime;
    DateTime dateTime;
} ClockState;

#endif
