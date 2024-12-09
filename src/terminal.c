#include "terminal.h"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>


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
        print_error("Box width is less than the length of the longest word in the message.\n");
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

        // Move index to "next line" (and skip the space).
        breakIndex += BOX_WIDTH + 1;
    }

    // Print top side of text box.
    print_top_of_box(title, wc_light_blue);

    int n = 0;

    while (n < messageLength) {
        // While the end of the message has not been reached.
        // Print a middle slice of the box.
        print_middle_of_box();

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
    print_top_of_box(title, wc_light_blue);

    // Print the left and right side of the box with spaces in between.
    print_middle_of_box();

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

    // // Check if input is valid and return if so.
    // check_input(input);
    return input;
}

void print_best_journey(const route_s journey) {
    // Allocate memory for the longest allowed box title.
    char title[BOX_WIDTH];

    // Create title of box.
    sprintf(title, "Your best journey is by %s", journey.transportType == 1 ? "airplane" : "train");
    print_top_of_box(title, wc_light_blue);

    // Print 5 "center slices" of a box.
    for (int i = 0; i < 5; i++) {
        print_middle_of_box();
    }

    // Change text color and go to the first "slice".
    set_win_color(wc_bright_white);
    printf("\033[5A");

    // For every "slice".
    for (int i = 0; i < 5; i++) {
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
                if (journey.travelTime + journey.downtime >= 60) {
                    printf("Est. travel time:\t%d hour(s) and %d minutes", (journey.travelTime + journey.downtime) / 60, (journey.travelTime + journey.downtime) % 60);
                } else {
                    printf("Est. travel time:\t%d minutes", journey.travelTime + journey.downtime);
                }
                break;
            case 3: // Print price.
                printf("Price:\t\t%.2lf EUR", journey.price / 100.0);
                break;
            case 4: // Print emission.
                printf("Emission:\t\t%d kg CO2e per passenger", journey.emission);
                break;
            default: // Error if unknown case.
                print_error("Unknown 'print_journey' case!");
                exit(EXIT_FAILURE);
        }

        // Move cursor down 1 line.
        printf("\033[1B");
    }

    // Move cursor to the first column and print bottom of box.
    printf("\033[0G");
    print_bottom_of_box();
}

void print_alternative_journeys(route_s journeys[], const int numberOfJourneys) {
    // Print top of box with title.
    print_top_of_box("Alternative journeys", wc_light_blue);

    // Print table "categories".
    print_middle_of_box();
    set_win_color(wc_bright_white);
    printf("\033[1A\033[0G\033[%dC", 1 + BOX_PADDING);
    printf("Vehicle\tTime\tPrice\t\tEmission\n");

    // Print alternative journeys.
    for (int i = 0; i < numberOfJourneys; i++) {
        char journey[BOX_WIDTH];
        print_middle_of_box();
        set_win_color(wc_bright_white);
        printf("\033[1A\033[0G\033[%dC", 1 + BOX_PADDING);
        sprintf(journey, "%s\t%02d:%02d\t%.2lf EUR\t%d kg\n", journeys[i].transportType == 1 ? "Airplane" : "Train   ", (journeys[i].travelTime + journeys[i].downtime) / 60, (journeys[i].travelTime + journeys[i].downtime) % 60, journeys[i].price / 100.00,
                journeys[i].emission);
        printf(journey);
    }

    // Print the bottom of the box.
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

    char c = '\0';
    int n = 0;
    while (1) {
        c = w_getchar();

        // If a priority is "empty" and the user types a valid number.
        if (c >= 49 && c <= 51 && n < 3 && priorities[(int) c - '1'] == -1) {
            // Set the priority to a number corresponding to the category.
            priorities[(int) c - '1'] = n;

            // Print user input to terminal and move cursor the correct position.
            printf("%c", c);
            if (n == 0) {
                printf("\033[9C");
            } else if (n == 1) {
                printf("\033[10C");
            }

            n++;
        } else if (c == ENTER && n == 3) {
            // Break out of the while loop if the user presses enter.
            break;
        } else if (c == BSP && n > 0) {
            // Delete number in category box in terminal.
            if (n == 1) {
                printf("\033[10D \033[1D");
            } else if (n == 2) {
                printf("\033[11D \033[1D");
            } else if (n == 3) {
                printf("\033[1D \033[1D");
            }

            // "Clear" category from priority array.
            for (int i = 0; i < 3; ++i) {
                if (priorities[i] == n - 1) {
                    priorities[i] = -1;
                }
            }

            n--;
        }
    }

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

    while (1) {
        c = w_getchar();

        if (i > 0 && c == BSP) {
            // Backspace: delete last char.
            printf("\033[1D \033[1D");
            i--;
            input[i] = '\0';
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
            // Ignore the rest
            fflush(stdin);
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

            // If a strings match is found
            if (stringsAmount > 0) {
                set_win_color(wc_gray);

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
            } else {
                autoCompleteSelection = -1;
                autoCompleteString = NULL;

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
        printf("%c", 201);
        utf8_print(L"╡");
        printf(" \033[1m");
        set_win_color(wc_light_blue);
        printf(titles[i]);
        printf("\033[0m");
        set_win_color(wc_gray);
        utf8_print(L" ╞");
        printf("%c", 187);
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

void print_top_of_box(const char title[], const winColor_e titleColor) {
    // Print top left corner and set font to bold for title.
    set_win_color(wc_gray);
    printf("%c", 201);
    utf8_print(L"╡");
    printf(" \033[1m");

    // Set text color and print box title.
    set_win_color(titleColor);
    int i = strlen(title);
    printf(title);

    // Set text back to non-bold and set color.
    printf("\033[0m");
    set_win_color(wc_gray);
    utf8_print(L" ╞");

    // Print the rest of the top part.
    while (i < BOX_WIDTH) {
        printf("%c", 205);
        i++;
    }

    // Print top right corner.
    printf("%c\n", 187);
}

void print_middle_of_box() {
    set_win_color(wc_gray);
    printf("%c", 186);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf(" ");
    }
    printf("%c\n", 186);
}

void print_bottom_of_box() {
    set_win_color(wc_gray);
    printf("%c", 200);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);
}

int length_of_longest_word(const char message[]) {
    int lengthOfLongestWord = 0;
    int n = 0;

    // Look for spaces in message.
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == ' ') {
            if (n > lengthOfLongestWord) {
                // If the distance between to spaces is greater than the previous distance, then update variable.
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
        print_error("Could not getting console mode");
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
        print_error("Could not setting console mode");
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
 * Prints an error message
 * @param msg Error message
 */
void print_error(const char *msg) {
    char title[13] = "Error";

    // Reset loading bar
    loading_bar(0);

    // Create title
    if (errno != 0) {
        sprintf(title, "%s: %d", title, errno);
    }

    // Print error in box
    print_top_of_box(title, wc_red);
    print_middle_of_box();
    printf("\033[1A\033[%dG", 1 + BOX_PADDING);

    set_win_color(wc_light_red);

    if (errno == 0) {
        printf("%s.\n", msg);
    } else {
        perror(msg);
    }

    set_win_color(wc_default);

    print_bottom_of_box();
}

/**
 * Draw loading bar
 * @param mode 0: clear, 1: draw
 */
void loading_bar(const int mode) {
    static int progress = 2;
    static int firstPrint = 1;
    static clock_t lastUpdate = 0;
    const int size = 8;
    const clock_t interval = CLOCKS_PER_SEC / 10;

    if (mode == 0 && !firstPrint) {
        // Reset variables
        progress = 2;
        firstPrint = 1;

        // Delet bar
        printf("\033[1A\033[G");
        for (int i = 0; i < size + 2; ++i) {
            putchar(' ');
        }
        printf("\033[G");
    } else if (mode == 1 && clock() >= lastUpdate + interval) {
        // Save time of drawing
        lastUpdate = clock();

        if (firstPrint) {
            firstPrint = 0;
        } else {
            printf("\033[1A\033[G");
        }

        // Draw loading bar
        putchar('[');
        for (int i = 0; i < size; ++i) {
            if ((progress - i) % size == 0) {
                set_win_color(wc_bright_white);
                putchar('#');
            } else if ((progress - i - 1) % size == 0) {
                set_win_color(wc_white);
                putchar('#');
            } else if ((progress - i - 2) % size == 0) {
                set_win_color(wc_gray);
                putchar('#');
            } else {
                putchar(' ');
            }
        }
        set_win_color(wc_default);
        putchar(']');
        putchar('\n');

        progress = (progress + 1) % size;
    }
}

void utf8_print(const wchar_t *utf8Str) {
    static int firstRunFlag = 1;
    static UINT initialCodeSpace;

    // Save initial terminal mode in first run of function
    if (firstRunFlag) {
        firstRunFlag = 0;
        initialCodeSpace = GetConsoleOutputCP();
    }

    // Force utf-8 mode
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U8TEXT);

    // Print string
    wprintf(utf8Str);

    SetConsoleOutputCP(initialCodeSpace);
    _setmode(_fileno(stdout), _O_TEXT);
}
