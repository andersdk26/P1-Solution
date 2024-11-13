#include "general.h"

/**
 * Sets text color in Windows terminal
 * @param color Color code
 */
void set_win_color(const winColor_t color) {
#ifdef _WINDOWS_
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //static const WORD standardColor = 0x07;

    SetConsoleTextAttribute(hConsole, color);
#endif
}

/**
 * Merges a color and a highlighter
 * @param color Color code
 * @param highlighter Highlighter color code
 */
winColor_t merge_win_color(const winColor_t color, const winColor_t highlighter) {
    winColor_t result = highlighter & 0xf0;
    result |= color;
    return result;
}

/**
 * Clears screen
 */
void cls(void) {
    printf("\n\n\n\n\n\n\n\n\n\n");
    system("cls");
}

/**
 * Delays program for x ms
 * @param millis Delay in ms
 */
void delay(const clock_t millis) {
    if (millis == 0) {
        return;
    }
    const clock_t startTime = clock();
    while (startTime + millis > clock()) {}
}

void swap_int(int *p1, int *p2) {
    *p1 = *p1 ^ *p2;
    *p2 = *p1 ^ *p2;
    *p1 = *p1 ^ *p2;
}