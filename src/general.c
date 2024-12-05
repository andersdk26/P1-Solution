#include "general.h"

/**
 * Sets text color in Windows terminal
 * @param color Color code
 */
void set_win_color(const winColor_e color) {
    // Only run for windows OS
#ifdef _WINDOWS_
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
#endif
}

/**
 * Merges a color and a highlighter
 * @param color Color code
 * @param highlighter Highlighter color code
 */
winColor_e merge_win_color(const winColor_e color, const winColor_e highlighter) {
    // Mask highlighter
    winColor_e result = highlighter & 0xf0;
    // Add color
    result |= color;
    return result;
}

/**
 * Clears screen
 */
void cls(void) {
    printf("\n\n\n");
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
    // wait until time has passed
    while (startTime + millis > clock()) {}
}

/**
 * Prints warning (in yellow)
 * @param msg String to print
 */
void print_warning(const char* msg) {
    set_win_color(wc_yellow);
    printf("%s", msg);
    set_win_color(wc_default);
}

void check_input(const char* input) {
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
void* memory_allocation(void* p, const size_t size, const int clear) {
    void* lastP = NULL;

    if (size < 1) {
        free(p);
        return NULL;
    }

    if (p == NULL) {
        // New memory allocation
        if (clear) {
            p = calloc(size, 1);
        }
        else {
            p = malloc(size);
        }
    }
    else {
        // Expand memory allocation
        lastP = p; // Save last memory address to prevent memory leak if expansion fails
        p = realloc(p, size);
    }

    // Check for error
    if (p == NULL) {
        perror("Memory allocation error");
        free(lastP);
        exit(EXIT_FAILURE);
    }

    return p;
}
