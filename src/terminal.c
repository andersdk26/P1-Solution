#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

HANDLE hstdin;
DWORD mode;

void box_print(const char originalMessage[], const char title[]) {
    // Allocate memory for copy of original message.
    char *message = memory_allocation(NULL, strlen(originalMessage) + 1, 0);

    // Put original message into allocated memory as a string.
    strcpy(message, originalMessage);

    // Get length of message.
    const int messageLength = strlen(message);

    // Check if the box has room for the longest word in the message.
    if (BOX_WIDTH < length_of_longest_word(message)) {
        printf("ERROR: Box width is less than the length of the longest word in the message.\n");
        exit(EXIT_FAILURE);
    }

    // Get the max height of the box.
    const int boxHeight = number_of_words(message);

    // Find places in the message to create breaks.
    int previousBreakIndex = 0;
    int breakIndex = BOX_WIDTH - 1;

    for (int n = 0; n < boxHeight - 1; n++) {
        // For the max height of the box.
        if (breakIndex >= messageLength) {
            // If the index being checked is greater or equal to the length of the message, break out of the for loop.
            break;
        }
        while (message[breakIndex] != ' ') {
            // While the index is not at a space, decrease the index.
            breakIndex--;
        }
        if (breakIndex < previousBreakIndex) {
            // If the index is less than the previous break index, break out of the for loop.
            break;
        }

        // Replace the space with a symbol for breaking the line.
        message[breakIndex] = '#';

        // Update previous index.
        previousBreakIndex = breakIndex;

        // Move index to "next line".
        breakIndex += BOX_WIDTH + 1;
    }

    // Print top side of text box.
    print_top_of_box(title);

    int n = 0;

    while (n < messageLength) {
        // While the end of the message has not been reached.
        // Print a middle slice of the box.
        print_left_side_of_box();
        for (int i = 0; i < BOX_WIDTH; i++) {
            printf(" ");
        }
        print_right_side_of_box();

        // Change text color and move to the start of the slice.
        set_win_color(wc_bright_white);
        printf("\033[1A" "\033[0G" "\033[%dC", 1 + BOX_PADDING);

        // Print text in the line until a break point or the end of the slice has been reached.
        for (int i = 0; i < BOX_WIDTH; i++) {
            if (n < messageLength) {
                if (message[n] == '#') {
                    if (i == 0) {
                        printf("\033[1A");
                    }
                    n++;
                    break;
                }
                printf("%c", message[n]);
                n++;
            }
        }

        // Once the line has been printed, move the cursor down one line and all the way to the left.
        printf("\033[1E");
    }

    // Bottom side of text box.
    print_bottom_of_box(BOX_WIDTH + 2 * BOX_PADDING);

    // Free allocated memory.
    free(message);
}

char *box_read(const char title[], const route_s *routes, const int routeQuantity, const searchInColumn_e searchColumn,
               const char *firstColumn) {
    // Print the top of the box.
    print_top_of_box(title);

    // Print the left and right side of the box with spaces in between.
    printf("%c", 186);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf(" ");
    }
    printf("%c\n", 186);

    // Print the bottom of the box.
    print_bottom_of_box();

    // Move the cursor into the box.
    printf("\033[2A");
    printf("\033[%dC", 1 + BOX_PADDING);

    // Allocate memory for reading the user's input.
    char *input = memory_allocation(NULL, BOX_WIDTH + 1, 0);
    set_win_color(wc_bright_white);
    read_characters(input, routes, routeQuantity, searchColumn, firstColumn);

    // Move the cursor down one line and all the way to the left and change text color.
    printf("\033[2E");
    set_win_color(wc_gray);

    // Check if input is valid and return if so.
    check_input(input);
    return input;
}

void print_journey(const route_s journey) {
    // Allocate memory for the longest allowed box title.
    char title[BOX_WIDTH];

    // Create title of box.
    sprintf(title, "Your best journey is by %s", journey.transportType == 1 ? "airplane" : "train");
    print_top_of_box(title);

    // Print 5 "center slices" of a box.
    for (int i = 0; i < 5; i++) {
        print_left_side_of_box();
        for (int j = 0; j < BOX_WIDTH; j++) {
            printf(" ");
        }
        print_right_side_of_box();
    }

    // Change text color and go to the first "slice".
    set_win_color(wc_bright_white);
    printf("\033[5A");


    for (int i = 0; i < 5; i++) {
        // For every "slice".
        // Go the first column and add padding.
        printf("\033[0G");
        printf("\033[%dC", 1 + BOX_PADDING);
        switch (i) {
            case 0: // Print start location.
                printf("From:\t\t%s (%s)", journey.origin, journey.originName);
                break;
            case 1: // Print destination.
                printf("To:\t\t\t%s (%s)", journey.destination, journey.destinationName);
                break;
            case 2: // Print estimated travel time including downtime.
                printf("Est. travel time:\t%d minutes", journey.travelTime + journey.downtime);
                break;
            case 3: // Print price.
                printf("Price:\t\t%.2lf EUR", journey.price / 100.0);
                break;
            case 4: // Print emission.
                printf("Emission:\t\t%d kg CO2e per passenger.", journey.emission);
                break;
            default: // Error if unknown case.
                printf("Unknown 'print_journey' case!");
                exit(EXIT_FAILURE);
        }

        // Move cursor down 1 line.
        printf("\033[1B");
    }

    // Move cursor to the first column and print bottom of box.
    printf("\033[0G");
    print_bottom_of_box();
}

void get_priorities(int priorities[3]) {
    // Create array of titles.
    const char titles[3][10] = {{"Time"}, {"Price"}, {"Emission"}};

    // Print input boxes in terminal.
    print_top_of_priority_boxes(titles);
    print_middle_of_priority_boxes(titles);
    print_bottom_of_priority_boxes(titles);

    // Set cursor inside the first box.
    printf("\033[2A");
    printf("\033[3C");
    set_win_color(wc_bright_white);

    // Set terminal settings.
    set_terminal_mode(ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT
                      ,ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);

    // TODO: Gør så man kan gå tilbage i sine prioriteter.
    // TODO: Skriv kommentarer når det er gjort.

    char c = '\0';
    int n = 0;
    while (n < 3) {
        while (c < 49 || c > 51 || priorities[(int) c - '1'] != -1) {
            c = w_getchar();
        }
        priorities[(int) c - '1'] = n;
        printf("%c", c);
        c = '\0';
        n++;

        if (n == 1) {
            printf("\033[9C");
        } else if (n == 2) {
            printf("\033[10C");
        }
    }

    /*

    // Get priority for time.
    char p = '\0';
    while (p < 49 || p > 51) {
        p = getchar();
    }
    priorities[(int) p - '1'] = p_time;
    printf("%c", p);
    p = '\0';

    // Move cursor to the second box.
    printf("\033[9C");

    // Get priority for price.
    while (p < 49 || p > 51 || priorities[(int) p - '1'] != -1) {
        p = getchar();
    }
    priorities[(int) p - '1'] = p_price;
    printf("%c", p);
    p = '\0';

    // Move cursor to the third box.
    printf("\033[10C");

    // Get priority for emission.
    while (p < 49 || p > 51 || priorities[(int) p - '1'] != -1) {
        p = getchar();
    }
    priorities[(int) p - '1'] = p_emission;
    printf("%c", p);
    p = '\0';

    */

    // Move cursor out of the box and change text color.
    printf("\033[2B");
    printf("\033[0G");
    set_win_color(wc_gray);

    // Reset terminal settings.
    set_terminal_mode(0, 0);
}

void read_characters(char *input, const route_s *routes, const int routeQuantity, const searchInColumn_e searchColumn,
                     const char *firstColumn) {
    int i = 0;
    char c;
    char **strings = NULL;
    int stringsAmount = 0;
    char *autoCompleteString = NULL;
    int autoCompleteSelection = -1;
    input[i] = '\0';

    // Prepare terminal for autocomplete.
    set_terminal_mode(ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT
                      ,ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);

    // TODO: Problemer med autocomplete efter BSP

    while (1) {
        c = w_getchar();

        if (i > 0 && c == BSP) {
            // Backspace: delete last char.
            printf("\033[1D \033[1D");
            input[i] = '\0';
            i--;
            if (i == 0) {
                input[0] = '\0';
            }
        } else if (i > 0 && c == ENTER) {
            // Enter: return input.
            break;
        } else if (c == '\t' && autoCompleteString != NULL) {
            // Put the suggested autocompletion into the char pointer.
            strcpy(input, autoCompleteString);

            // Print auto-completed input.
            if (i != 0) {
                printf("\033[%dD", i);
            }
            printf(input);
            i = strlen(input);

            continue;
        } else if (c == ESC) {
            // Handel escape codes.
            if (w_getchar() != START_BRACKET) {
                continue;
            }

            const char c2 = w_getchar();
            switch (c2) {
                case 'A': // Up arrow.
                case 'D': // Left arrow.
                    autoCompleteSelection--;
                    break;
                case 'B': // Right arrow.
                case 'C': // Down arrow.
                    autoCompleteSelection++;
                    break;
                default:
                    continue;
            }
        }

        // If length of message is less than the width of the box and the user has entered a letter, print letter and update input.
        if (i < BOX_WIDTH && isalpha(c)) {
            printf("%c", c);
            input[i] = c;
            input[i + 1] = '\0';
            i++;
        }

        // Autocomplete
        if (searchColumn != sic_none) {
            set_win_color(wc_gray);

            // Reset auto-complete strings
            free_string_list(strings, stringsAmount, 1);
            strings = NULL;
            stringsAmount = 0;

            // Search in given column
            if (searchColumn == sic_second) {
                search_second_column(firstColumn, input, &strings, &stringsAmount, routes, routeQuantity);
            } else {
                search_first_column(input, &strings, &stringsAmount, routes, routeQuantity);
            }

            // Continue if 0 strings match input
            if (stringsAmount < 1) {
                set_win_color(wc_bright_white);
                continue;
            }

            // Wrap auto-complete selection var based on amount of strings
            if (autoCompleteSelection < 0) {
                autoCompleteSelection = stringsAmount - 1;
            } else {
                autoCompleteSelection %= stringsAmount;
            }

            // Save selected string
            autoCompleteString = strings[autoCompleteSelection];

            if (stringsAmount > 0 && strlen(autoCompleteString) > i) {
                // Print auto-complete string
                printf("%s", autoCompleteString + i);

                // Clear the rest of the box
                for (int j = i; j < BOX_WIDTH - BOX_PADDING * 2 - 2 - 2; ++j) {
                    putchar(' ');
                }

                // Set cursor at next char
                putchar(0x0D);
                printf("\033[%dC", i + BOX_PADDING + 1);
            }

            set_win_color(wc_bright_white);
        }
    }

    // Add NULL terminator at end of input.
    input[i] = '\0';

    // Reset terminal mode
    set_terminal_mode(0, 0);
}

void print_top_of_priority_boxes(const char titles[3][10]) {
    // Set text color.
    set_win_color(wc_gray);

    // Print top of 3 boxes including titles from the array.
    for (int i = 0; i < 3; i++) {
        printf("%c%c \033[1m", 201, 181);
        set_win_color(wc_light_blue);
        printf(titles[i]);
        printf("\033[0m");
        set_win_color(wc_gray);
        printf(" %c%c", 198, 187);
    }
    printf("\n");
}

void print_middle_of_priority_boxes(const char titles[3][10]) {
    // Print middle "slices" of priority boxes with varying widths.
    for (int i = 0; i < 3; i++) {
        printf("%c", 186);
        for (int j = 0; j < strlen(titles[i]) + 4; j++) {
            printf(" ");
        }
        printf("%c", 186);
    }
    printf("\n");
}

void print_bottom_of_priority_boxes(const char titles[3][10]) {
    // Set text color.
    set_win_color(wc_gray);

    // Print 3 bottoms of boxes with varying widths.
    for (int i = 0; i < 3; i++) {
        printf("%c", 200);
        for (int j = 0; j < strlen(titles[i]) + 4; j++) {
            printf("%c", 205);
        }
        printf("%c", 188);
    }

    printf("\n");
}

void print_top_of_box(const char title[]) {
    // Print top left corner and set font to bold for title.
    set_win_color(wc_gray);
    printf("%c%c \033[1m", 201, 181);

    // Set text color and print box title.
    set_win_color(wc_light_blue);
    int i = strlen(title);
    printf(title);

    // Set text back to non-bold and set color.
    printf("\033[0m");
    set_win_color(wc_gray);
    printf(" %c", 198);

    // Print the rest of the top part.
    while (i < BOX_WIDTH) {
        printf("%c", 205);
        i++;
    }

    // Print top right corner.
    printf("%c\n", 187);
}

void print_bottom_of_box() {
    set_win_color(wc_gray);
    printf("%c", 200);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);
}

void print_left_side_of_box() {
    set_win_color(wc_gray);
    printf("%c", 186);
    for (int k = 0; k < BOX_PADDING; k++) {
        printf(" ");
    }
}

void print_right_side_of_box() {
    set_win_color(wc_gray);
    for (int k = 0; k < BOX_PADDING; k++) {
        printf(" ");
    }
    printf("%c\n", 186);
}

int length_of_longest_word(const char message[]) {
    int lengthOfLongestWord = 0;
    int n = 0;

    // Look for spaces in message.
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == ' ') {
            if (n > lengthOfLongestWord) { // If the distance between to spaces is greater than the previous distance, then update variable.
                lengthOfLongestWord = n;
            }
            n = 0;
        } else {
            n++;
        }
    }

    return lengthOfLongestWord;
}

int number_of_words(const char message[]) {
    // If message is 0 characters long, return 0.
    if (strlen(message) == 0) {
        return 0;
    }

    int number_of_words = 1;

    // Count number of spaces in message.
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == ' ') {
            number_of_words++;
        }
    }

    return number_of_words;
}

/**
 * Changes terminal mode
 * If set = clear = 0, then initial terminal mode is set
 * @param setValues Values to set
 * @param clearValues Values to clear
 */
void set_terminal_mode(const DWORD setValues, const DWORD clearValues) {
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD newMode;
    static int firstRunFlag = 1;
    static DWORD initialMode;

    // Get the current input mode
    if (!GetConsoleMode(hstdin, &mode)) {
        perror("Error getting console mode");
        exit(EXIT_FAILURE);
    }

    // Save initial terminal mode in first run of function
    if (firstRunFlag) {
        firstRunFlag = 0;
        initialMode = mode;
    }

    if (setValues == 0 && clearValues == 0) {
        // Set initial mode
        newMode = initialMode;
    } else {
        // Clear and set values
        newMode = mode & ~clearValues;
        newMode |= setValues;
    }

    // Set the new input mode
    if (!SetConsoleMode(hstdin, newMode)) {
        perror("Error setting console mode");
        exit(EXIT_FAILURE);
    }
}

/**
 * Read raw input from windows terminal
 * @return Char as int value
 */
int w_getchar() {
    INPUT_RECORD record;
    DWORD events;
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

    while (1) {
        // Wait for and read a single input event
        if (ReadConsoleInput(hstdin, &record, 1, &events)) {
            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
                return record.Event.KeyEvent.uChar.AsciiChar;
            }
        }
    }
}

/**
 * Prints a error message
 * @param msg Error message
 */
void print_error(const char *msg) {
    set_win_color(wc_light_red_hgl);

    if (stderr == NULL) {
        printf("%s.\n", msg);
    } else {
        perror(msg);
    }

    set_win_color(wc_default);
}

/**
 * String to int with error check
 * @param start String to convert
 * @param base Base of number
 * @return Parsed integer
 */
int strtol_check(const char *start, const int base) {
    char *end;
    errno = 0; // Reset error

    // Parse int
    const int result = strtol(start, &end, base);

    // Catch error
    if (end == start) {
        print_error("Error: no digits was read");
    } else if (end == start) {
        print_error("Error: no digits was read");
    } else if (errno == ERANGE && result == LONG_MIN) {
        print_error("Error: underflow");
    } else if (errno == ERANGE) {
        print_error("Error: overflow");
    } else if (errno == ERANGE) {
        print_error("Error: invalid base");
    } else if (errno != 0) {
        print_error("Error: unknown error");
    }

    return result;
}

/**
 * String to double with error check
 * @param start String to convert
 * @return Parsed double
 */
double strtod_check(const char *start) {
    char *end;
    errno = 0; // Reset error

    // Parse int
    const double result = strtod(start, &end);

    // Catch error
    if (end == start) {
        print_error("Error: no digits was read");
    } else if (errno == ERANGE && result == LONG_MIN) {
        print_error("Error: underflow");
    } else if (errno == ERANGE) {
        print_error("Error: overflow");
    } else if (errno != 0) {
        print_error("Error: unknown error");
    }

    return result;
}
