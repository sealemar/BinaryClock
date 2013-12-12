/**
 * Binary Clock
 *
 * developed by Sergey Markelov (11/04/2013)
 */

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

#define DUTY_CYCLE_MILLIS 15

#define BUTTON_1_PIN 5
#define BUTTON_2_PIN 4
#define BUTTON_3_PIN 3
#define BUTTON_4_PIN 2

// Pin connected to ST_CP of 74HC595
#define LATCH_PIN 8
// Pin connected to SH_CP of 74HC595
#define CLOCK_PIN 12
// Pin connected to DS of 74HC595
#define DATA_PIN  11

static byte ScreenColumns[CLOCK_PATTERN_SIZE] = { ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0 };

// static const int cols[] = { P1, P2, P3, P4, P5, P6, P7, P8 };
// static const int rows[] = { PA, PB, PC, PD, PE, PF, PG, PH };
static const int buttons[] = { BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN };
static ClockState clockState;

static void display()
{
    for(size_t i = 0; i < countof(ScreenColumns); ++i) {
        // Take the latchPin low so the LEDs don't change while you're sending in bits:
        digitalWrite(LATCH_PIN, LOW);

        // Shift out the bits
        // First shift rows, then columns, because shift register for rows comes after columns
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 1 << i);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ScreenColumns[i]);

        // Take the latch pin high so the LEDs will light up:
        digitalWrite(LATCH_PIN, HIGH);
    }
}

static int arduino_setPixel(int x, int y, Bool turnOn)
{
    if(turnOn) {
        ScreenColumns[y] &= ~(1 << x);
    } else {
        ScreenColumns[y] |= 1 << x;
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
    memset(&ScreenColumns, 0xFF, sizeof(ScreenColumns));

    return 0;
}

static int arduino_initDateTime(DateTime *dt)
{
    memset(dt, 0, sizeof(DateTime));

    dt->year   = 2013;
    dt->month  = DECEMBER;
    dt->day    = 10;
    dt->hour   = 9;
    dt->minute = 17;
    dt->second = 41;

    return 0;
}

void setup()
{
    clock_extern_setPixel     = arduino_setPixel;
    clock_extern_uptimeMillis = arduino_uptimeMillis;
    clock_extern_initDateTime = arduino_initDateTime;
    clock_extern_clearScreen  = arduino_clearScreen;

    // Set pins to output so you can control the shift register
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN,  OUTPUT);

    for(size_t i = 0; i < countof(buttons); ++i) {
        pinMode(buttons[i], INPUT);
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

    display();
    delay(DUTY_CYCLE_MILLIS);
}
