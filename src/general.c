#include "general.h"

/**
 * Sets text color in Windows terminal
 * @param color Color code
 */
void set_win_color(const winColor_e color) {
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
winColor_e merge_win_color(const winColor_e color, const winColor_e highlighter) {
    winColor_e result = highlighter & 0xf0;
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

/**
 * Prints warning (in yellow)
 * @param msg String to print
 */
void print_warning(const char *msg) {
    set_win_color(wc_yellow);
    printf("%s", msg);
    set_win_color(wc_default);
}

void check_input(const char *input) {
    if (input == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
}

/**
 * Allocates specific memory size
 * @param p pointer
 * @param size Size of needed memory bytes
 * @param clear True if all values should be set to 0
 * @return Memory pointer
 */
void* memory_allocation(void *p, const size_t size, const int clear) {
    void *lastP = NULL;
    if (p == NULL) {
        if (clear) {
            p = calloc(size, 1);
        } else {
            p = malloc(size);
        }
    } else {
        lastP = p;
        p = realloc(p, size);
    }

    if (p == NULL) {
        free(lastP);
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    return p;
}