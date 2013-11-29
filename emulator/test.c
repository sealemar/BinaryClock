// int checkClockScreen()
// {
//     for(int y = 0; y < CLOCK_SCREEN_HEIGHT; ++y)
//     {
//         for(int x = 0; x < CLOCK_SCREEN_WIDTH; ++x)
//         {
//             int res = clock_setPixel(x, y, 1);
//             if(res) ContinueError(res, "%d");
//             clock_setPixel(x, y, 0);
//             if(getch() == 27) return 0;
//         }
//     }
//
//     return 0;
// }
//
// int traverseAlphabet()
// {
//     int i = 0;
//     int ch;
//     do {
//         int res = clock_drawPattern(ClockAlphabet[i]);
//         if(res) ContinueError(res, "%d");
//
//         ch = getch();
//         if(ch == 'h' || ch == 'k') {
//             --i;
//             if(i == -1) {
//                 i = CLOCK_ALPHABET_SIZE - 1;
//             }
//         } else {
//             ++i;
//             i %= CLOCK_ALPHABET_SIZE;
//         }
//     } while (ch != 27);
//
//     return 0;
// }
//
// int slideText(const char* text)
// {
//     Bool isLastStep = FALSE;
//
//     for(size_t step = 0; !isLastStep; ++step) {
//         unsigned char pattern[CLOCK_PATTERN_SIZE];
//
//         int res = clock_slideText(text, step, &isLastStep, pattern);
//         //int res = clock_slidePattern(ClockAlphabet[CLOCK_SMILEY_FACE_SAD], ClockAlphabet[CLOCK_SMILEY_FACE_SMILE], step, &isLastStep, pattern);
//         if(res) ContinueError(res, "%d");
//
//         res = clock_drawPattern(pattern);
//         if(res) ContinueError(res, "%d");
//
//         if(getch() == 27) return 0;
//     }
//
//     return 0;
// }
//
// int checkDisplayBinaryNumber()
// {
//     clock_clearScreen();
//
//     for(unsigned int i = 0, last = (1 << CLOCK_SCREEN_HEIGHT); i < last; ++i) {
//         int res = clock_displayBinaryNumber(i, 2, 2);
//         if(res) ContinueError(res, "%d");
//         if(getch() == 27) return 0;
//     }
//
//     return 0;
// }
//
// int displayDateTime()
// {
//     DateTime dt;
//
//     dt.year = 2013;
//     dt.month = NOVEMBER;
//     dt.day = 22;
//     dt.hour = 23;
//     dt.minute = 27;
//     dt.second = 17;
//
//     int res = clock_displayDate(&dt);
//     if(res) ContinueError(res, "%d");
//
//     getch();
//
//     res = clock_displayTime(&dt);
//     if(res) ContinueError(res, "%d");
//
//     return 0;
// }
//
