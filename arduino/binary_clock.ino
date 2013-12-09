/**
 * Binary Clock
 *
 * developed by Sergey Markelov (11/04/2013)
 */

#include <clock.h>
#include <clock_main.h>

#define P1    2
#define P2    3
#define P3    4
#define P4    5
#define P5    6
#define P6    7
#define P7    8
#define P8    9
#define PA   10
#define PB   11
#define PC   12
#define PD   13
#define PE   A0
#define PF   A1
#define PG   A2
#define PH   A3

#define BUTTON_1_PIN 1
#define BUTTON_2_PIN 2
#define BUTTON_3_PIN 3
#define BUTTON_4_PIN 4

static const int cols[] = { P1, P2, P3, P4, P5, P6, P7, P8 };
static const int rows[] = { PA, PB, PC, PD, PE, PF, PG, PH };
static const int buttons[] = { BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN };
static ClockState clockState;

static int arduino_setPixel(int x, int y, Bool turnOn)
{
    if(turnOn) {
        digitalWrite(cols[x % countof(cols)], LOW);
        digitalWrite(rows[y / countof(rows)], HIGH);
    } else {
        digitalWrite(cols[x % countof(cols)], HIGH);
        digitalWrite(rows[y / countof(rows)], LOW);
    }

    return 0;
}

static int arduino_uptimeMillis(unsigned long *milliseconds)
{
    *milliseconds = millis();
    return 0;
}

void setup()
{
    clock_extern_setPixel     = arduino_setPixel;
    clock_extern_uptimeMillis = arduino_uptimeMillis;
//     clock_extern_initDateTime = emulator_initDateTime;
//     clock_extern_clearScreen  = emulator_clearScreen;

    for(size_t i = 0; i < countof(buttons); ++i) {
        pinMode(buttons[i], INPUT);
    }
    for(size_t i = 0; i < countof(cols); ++i) {
        pinMode(cols[i], OUTPUT);
    }
    for(size_t i = 0; i < countof(rows); ++i) {
        pinMode(rows[i], OUTPUT);
    }
    clock_clearScreen();

    Call(clock_init(&clockState));
}

void loop()
{
    for(size_t i = 0; i < countof(buttons); ++i) {
        Call(clock_button_press( &(clockState.buttons), i, digitalRead(buttons[i]) == HIGH ? TRUE : FALSE));
    }
    Call(clock_update(&clockState));
}
