#include "general.h"
#include "terminal.h"

/**
 * Sets text color in Windows terminal
 * @param color Color code
 */
void set_win_color(const winColor_e color) {
    // Only run for Windows OS
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
        print_error("Input error");
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
        print_error("Memory allocation failed");
        free(lastP);
        exit(EXIT_FAILURE);
    }

    return p;
}

/**
 * String to int with error check
 * @param start String to convert
 * @param base Base of number
 * @return Parsed integer
 */
int strtol_check(const char *start, const int base, int *errorFlag) {
    char *end;
    errno = 0; // Reset error

    // Parse int
    const int result = strtol(start, &end, base);

    // Catch error
    if (end == start) {
        print_error("Error: no digits was read");
        *errorFlag = 1;
    } else if (errno == ERANGE && result == LONG_MIN) {
        print_error("Underflow");
        *errorFlag = 1;
    } else if (errno == ERANGE) {
        print_error("Overflow");
        *errorFlag = 1;
    } else if (errno == EINVAL) {
        print_error("Invalid base");
        *errorFlag = 1;
    } else if (errno != 0) {
        print_error("Unknown error");
        *errorFlag = 1;
    }

    return result;
}

/**
 * String to double with error check
 * @param start String to convert
 * @return Parsed double
 */
double strtod_check(const char *start, int *errorFlag) {
    char *end;
    errno = 0; // Reset error

    // Parse int
    const double result = strtod(start, &end);

    // Catch error
    if (end == start) {
        print_error("No digits was read");
        *errorFlag = 1;
    } else if (errno == ERANGE && result == LONG_MIN) {
        print_error("Underflow");
        *errorFlag = 1;
    } else if (errno == ERANGE) {
        print_error("Overflow");
        *errorFlag = 1;
    } else if (errno != 0) {
        print_error("Unknown error");
        *errorFlag = 1;
    }

    return result;
}
