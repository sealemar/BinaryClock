/**
 * Binary Clock
 *
 * developed by Sergey Markelov (11/04/2013)
 */

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

#define countof(array) (sizeof(array) / sizeof(*array))

const int cols[] = { P1, P2, P3, P4, P5, P6, P7, P8 };
const int rows[] = { PA, PB, PC, PD, PE, PF, PG, PH };

int i = 0;

void setup()
{
    for(size_t i = 0; i < countof(cols); ++i) {
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], HIGH);
    }
    for(size_t i = 0; i < countof(cols); ++i) {
        pinMode(rows[i], OUTPUT);
        digitalWrite(rows[i], LOW);
    }
}

void loop()
{
    i %= countof(cols) * countof(rows);

    digitalWrite(cols[i % countof(cols)], LOW);
    digitalWrite(rows[i / countof(rows)], HIGH);

    delay(100);

    digitalWrite(cols[i % countof(cols)], HIGH);
    digitalWrite(rows[i / countof(rows)], LOW);

    ++i;
}
