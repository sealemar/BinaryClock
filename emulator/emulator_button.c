//
// developed by Sergey Markelov (11/28/2013)
//

#include <ncurses.h>
#include <errno.h>

#include <logger.h>
#include <clock.h>
#include <clock_state.h>

#include "emulator.h"
#include "emulator_button.h"
#include "include.h"

#define BUTTONS_INFO_WND_X 5

static WINDOW *WndButtons = NULL;

typedef struct {
    const int x;
    const int y;
    const char *title;

    // An array of characters which control the button. Is compared against getch() result.
    // The comparison stops when ERR is reached
    const int chs[CLOCK_BUTTON_MAX_COUNT];
} Button;

static const Button Buttons[] = {
    { 0, 0, "1", { '1', ERR, ERR, ERR } },
    { 0, 2, "2", { '2', '6', ERR, ERR } },
    { 0, 4, "3", { '3', '6', ERR, ERR } },
    { 0, 6, "4", { '4', ERR, ERR, ERR } },
};

static const char *ButtonsDescription[CLOCK_STATE_COUNT][CLOCK_BUTTON_COUNT] = {
    // CLOCK_STATE_HELLO
    {
        "", "Skip", "", "",
    },

    // CLOCK_STATE_SHOW_TIME
    {
        "Show time in text",
        "Set time",
        "Show date",
        "",
    },

    // CLOCK_STATE_SHOW_DATE
    {
        "Show date in text",
        "Set date",
        "Show time",
        "",
    },

    // TODO: think about setting date and time in big endian mode

    // CLOCK_STATE_SHOW_TIME_BIG_ENDIAN
    {
        "Show time in binary",
        "",
        "Show date",
        "",
    },

    // CLOCK_STATE_SHOW_DATE_BIG_ENDIAN
    {
        "Show date in binary",
        "",
        "Show time",
        "",
    },

    // CLOCK_STATE_SET_TIME
    {
        "",
        "Set next",
        "Decrease",
        "Increase",
    },

    // CLOCK_STATE_SET_DATE
    {
        "",
        "Set next",
        "Decrease",
        "Increase",
    },
};

inline static int pressButton(const Button *btn, Bool isPressed)
{
    const int attr = has_colors()
                   ? isPressed ? COLOR_PAIR(COLOR_ON) : COLOR_PAIR(COLOR_OFF)
                   : 0;

    CallNcurses( wattrset(WndButtons, attr) );
    CallNcurses( mvwprintw(WndButtons, btn->y, btn->x, "|%s|", btn->title) );

    return 0;
}

int emulator_button_init()
{
    CallMalloc( WndButtons, newwin(CLOCK_SCREEN_HEIGHT, 0, 2, (CLOCK_SCREEN_WIDTH << 1) + 3) );

    const Button *btn = Buttons;

    for(size_t i = 0; i < countof(Buttons); ++i, ++btn) {
        Call(pressButton(btn, FALSE));
    }

    CallNcurses( wrefresh(WndButtons) );

    return 0;
}

int emulator_button_deinit()
{
    _destroyWindow(WndButtons);
    return 0;
}

//
// @brief Checks the emulator buttons and updates _clockButtons_ if
//        a matching controling _ch_ is found for a button.
//
// @param clockButtons - a pointer to a structure to update
// @param ch that is what getch() returned
// @param matched if not NULL will be set to TRUE if ch was matched at least once
//
// @returns 0 on ok
//          EINVAL if _clockButtons_ is NULL
//
int emulator_button_press(ClockButtons *clockButtons, int ch, Bool *matched)
{
    NullCheck(clockButtons);

    if(matched != NULL) {
        *matched = FALSE;
    }

    const Button *btn = Buttons;
    for(size_t i = 0; i < countof(Buttons); ++i, ++btn) {
        Bool isPressed = FALSE;

        for(size_t c = 0; c < countof(btn->chs); ++c) {
            if(btn->chs[c] == ERR) break;
            if(btn->chs[c] == ch) {
                isPressed = TRUE;
                if(matched != NULL) {
                    *matched = TRUE;
                }
                break;
            }
        }

        Call(clock_button_press(clockButtons, i, isPressed));
        Call(pressButton(btn, isPressed));
    }

    CallNcurses( wrefresh(WndButtons) );

    return 0;
}

//
// @brief Updates the visual state of emulator's buttons
// @param clockState - the clock state object
// @returns 0 on ok
//          EINVAL if _clockButtons_ is NULL
//
int emulator_button_update(const ClockState *clockState)
{
    NullCheck(clockState);

    CallNcurses( wattrset(WndButtons, A_NORMAL) );

    const Button *btn = Buttons;
    for(size_t i = 0; i < countof(Buttons); ++i, ++btn) {
        CallNcurses( mvwprintw(WndButtons, btn->y, BUTTONS_INFO_WND_X, "%-30s", ButtonsDescription[clockState->state][i]) );
    }

    CallNcurses( wrefresh(WndButtons) );

    return 0;
}
