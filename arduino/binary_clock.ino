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
// @brief BinaryClock Arduino implementation
//
// developed by Sergey Markelov (11/04/2013)
//

#include <clock_main.h>

//
// P1 -> 74HC595 [Q0]  \
// P2 -> 74HC595 [Q1]   \
// P3 -> 74HC595 [Q2]    \
// P4 -> 74HC595 [Q3]     \
// P5 -> 74HC595 [Q4]     /   1st Shift Register
// P6 -> 74HC595 [Q5]    /      Columns
// P7 -> 74HC595 [Q6]   /
// P8 -> 74HC595 [Q7]  /
//
// PA -> 74HC595 [Q0]  \
// PB -> 74HC595 [Q1]   \
// PC -> 74HC595 [Q2]    \
// PD -> 74HC595 [Q3]     \
// PE -> 74HC595 [Q4]     /   2st Shift Register
// PF -> 74HC595 [Q5]    /      Rows
// PG -> 74HC595 [Q6]   /
// PH -> 74HC595 [Q7]  /
//

#define DISPLAY_MILLIS 2
#define CLOCK_UPDATE_MILLIS 50

#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3
#define BUTTON_3_PIN 4
#define BUTTON_4_PIN 5

// Pin connected to ST_CP of 74HC595
#define LATCH_PIN 8
// Pin connected to SH_CP of 74HC595
#define CLOCK_PIN 12
// Pin connected to DS of 74HC595
#define DATA_PIN  11

static byte ScreenRows[CLOCK_PATTERN_SIZE] = { 0 };

static const int buttons[] = { BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN };
static ClockState clockState;
static unsigned long lastDisplayMillis     = 0;
static unsigned long lastClockUpdateMillis = 0;

static void display()
{
    //
    // A little trick:
    // Iterate beyond the end of ScreenRows, so that the last iteration
    // clears the screen:
    // (1 << 8) & 0xff = 0 -> clear screen
    // in that situation the last row won't stay lit permanently, but will
    // obey DISPLAY_MILLIS
    //
    for(size_t i = 0; i < countof(ScreenRows) + 1; ++i) {
        // Take the latchPin low so the LEDs don't change while you're sending in bits:
        digitalWrite(LATCH_PIN, LOW);

        // Shift out the bits
        // First shift rows, then columns, because shift register for rows comes after columns
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ScreenRows[i]);

        // Columns are cathodes, thus walking 0 should be used, but I account for
        // ULN2803, which inverts that
        // http://forum.arduino.cc/index.php?topic=78585.msg594284#msg594284
        // http://www.thebox.myzen.co.uk/Workshop/LED_Matrix.html
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 1 << i);

        // Take the latch pin high so the LEDs will light up:
        digitalWrite(LATCH_PIN, HIGH);
    }
}

static int arduino_setPixel(int x, int y, Bool turnOn)
{
    if(turnOn) {
        ScreenRows[x] |= 1 << y;
    } else {
        ScreenRows[x] &= ~(1 << y);
    }

    return 0;
}

static int arduino_uptimeMillis(unsigned long *milliseconds)
{
    *milliseconds = millis();
    return 0;
}

static int arduino_clearScreen()
{
    memset(&ScreenRows, 0, sizeof(ScreenRows));

    return 0;
}

static int arduino_initDateTime(DateTime *dt)
{
    memset(dt, 0, sizeof(DateTime));

    dt->year   = 2014;
    dt->month  = APRIL;
    dt->day    = 13;
    dt->hour   = 0;
    dt->minute = 10;
    dt->second = 0;

    return 0;
}

void setup()
{
    // Clock interface implementation
    clock_extern_setPixel     = arduino_setPixel;
    clock_extern_uptimeMillis = arduino_uptimeMillis;
    clock_extern_initDateTime = arduino_initDateTime;
    clock_extern_clearScreen  = arduino_clearScreen;

    // Set pins to output so you can control the shift register
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN,  OUTPUT);

    for(size_t i = 0; i < countof(buttons); ++i) {
        pinMode(buttons[i], INPUT);           // set pin to input
        digitalWrite(buttons[i], HIGH);       // turn on internal pullup resistors

        // Note that internal pull-up resistor switches the state of a button.
        // To check if a button is pressed, test digitalRead(ButtonPin) == LOW
    }

    clock_clearScreen();
    Call(clock_init(&clockState));
}

void loop()
{
    unsigned long m = millis();

    //
    // Update BinaryClock state
    // account for overflows
    //
    if(m < lastClockUpdateMillis || m - lastClockUpdateMillis > CLOCK_UPDATE_MILLIS) {
        for(size_t i = 0; i < countof(buttons); ++i) {
            Call(clock_button_press( &(clockState.buttons), i, digitalRead(buttons[i]) == LOW ? TRUE : FALSE));
        }

        Call(clock_update(&clockState));
        m = lastClockUpdateMillis = millis();
    }

    //
    // Display
    // account for overflows
    //
    if(m < lastDisplayMillis || m - lastDisplayMillis > DISPLAY_MILLIS) {
        display();
        lastDisplayMillis = millis();
    }
}
