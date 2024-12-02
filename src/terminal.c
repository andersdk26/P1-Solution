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

    const int boxHeight = number_of_words(message);

    //int breaks[boxHeight - 1];
    int previousBreakIndex = 0;
    int breakIndex = BOX_WIDTH - 1;
    for (int n = 0; n < boxHeight - 1; n++) {
        if (breakIndex >= messageLength) {
            break;
        }
        while (message[breakIndex] != ' ') {
            breakIndex--;
        }
        if (breakIndex < previousBreakIndex) {
            break;
        }
        message[breakIndex] = '#';
        previousBreakIndex = breakIndex;
        breakIndex += BOX_WIDTH + 1;
    }

    // Top side of text box.
    print_top_of_box(title);

    int n = 0;
    while (n < messageLength) {
        print_left_side_of_box();
        for (int i = 0; i < BOX_WIDTH; i++) {
            printf(" ");
        }
        print_right_side_of_box();

        set_win_color(wc_bright_white);
        printf("\033[1A" "\033[0G" "\033[%dC", 1 + BOX_PADDING);

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

        printf("\033[1B" "\033[0G");
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

    char *input = memory_allocation(NULL, BOX_WIDTH + 1, 0);
    set_win_color(wc_bright_white);
    read_characters(input, routes, routeQuantity, searchColumn, firstColumn);

    printf("\033[2E");
    set_win_color(wc_gray);

    check_input(input);
    return input;
}

void print_journey(const route_s journey) {
    char title[BOX_WIDTH];
    sprintf(title, "Your best journey is by %s", journey.transportType == 1 ? "airplane" : "train");
    print_top_of_box(title);

    for (int i = 0; i < 5; i++) {
        print_left_side_of_box();
        for (int j = 0; j < BOX_WIDTH; j++) {
            printf(" ");
        }
        print_right_side_of_box();
    }

    set_win_color(wc_bright_white);

    printf("\033[5A");

    for (int i = 0; i < 5; i++) {
        printf("\033[0G");
        printf("\033[%dC", 1 + BOX_PADDING);
        switch (i) {
            case 0:
                printf("From:\t\t%s (%s)", journey.origin, journey.originName);
                break;
            case 1:
                printf("To:\t\t\t%s (%s)", journey.destination, journey.destinationName);
                break;
            case 2:
                printf("Est. travel time:\t%d minutes", journey.travelTime + journey.downtime);
                break;
            case 3:
                printf("Price:\t\t%.2lf EUR", journey.price / 100.0);
                break;
            case 4:
                printf("Emission:\t\t%d kg CO2e per passenger.", journey.emission);
                break;
            default:
                printf("Unknown 'print_journey' case!");
                exit(EXIT_FAILURE);
        }
        printf("\033[1B");
    }

    printf("\033[0G");
    print_bottom_of_box();
}

void get_priorities(int priorities[3]) {
    const char titles[3][10] = {{"Time"}, {"Price"}, {"Emission"}};

    // Print input boxes in terminal.
    print_top_of_priority_boxes(titles);
    print_middle_of_priority_boxes(titles);
    print_bottom_of_priority_boxes(titles);

    // Set cursor inside the first box.
    printf("\033[2A");
    printf("\033[3C");
    set_win_color(wc_bright_white);

    set_terminal_mode(ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT
                      ,ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);

    // TODO: Fix this. Gør så man kan slette sine inputs.

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
    unsigned int autoCompleteSelection = 0;

    // Prepare terminal for autocomplete
    set_terminal_mode(ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT
                      ,ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);

    while (1) {
        c = w_getchar();
        if (i > 0) {
            if (c == BSP) {
                printf("\033[1D \033[1D");
                input[i] = '\0';
                i--;
            } else if (c == '.' || c == ENTER) {
                break;
            } else if (c == '\t' && autoCompleteString != NULL) {
                // Put the suggested autocompletion into the char pointer.
                strcpy(input, autoCompleteString);
                printf("\033[%dD", i);
                printf(input);
                i = strlen(input);
                continue;
            } else if (c == ESC) {
                if (w_getchar() == START_BRACKET) {
                    const char c2 = w_getchar();
                    switch (c2) {
                        case 'A':
                        case 'D':
                            autoCompleteSelection--;
                            break;
                        case 'B':
                        case 'C':
                            autoCompleteSelection++;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if (i < BOX_WIDTH && isalpha(c)) {
            printf("%c", c);
            input[i] = c;
            input[i + 1] = '\0';
            i++;
        }

        // Autocomplete
        if (searchColumn != sic_none) {
            set_win_color(wc_gray);

            free_string_list(strings, stringsAmount, 1);
            strings = NULL;
            stringsAmount = 0;

            if (searchColumn == sic_second) {
                search_second_column(firstColumn, input, &strings, &stringsAmount, routes, routeQuantity);
            } else {
                search_first_column(input, &strings, &stringsAmount, routes, routeQuantity);
            }

            if (stringsAmount < 1) {
                set_win_color(wc_bright_white);
                continue;
            }
            if (autoCompleteSelection >= stringsAmount) {
                autoCompleteSelection = 0;
            }

            int stringLength = (int) strlen(strings[autoCompleteSelection]);
            autoCompleteString = strings[autoCompleteSelection];

            if (stringsAmount > 0 && stringLength > i) {
                printf("%s ", strings[autoCompleteSelection] + i);
                for (int j = 0; j < stringLength - i + 1; ++j) {
                    printf("\033[1D");
                }
            }

            set_win_color(wc_bright_white);
        }
    }
    input[i] = '\0';

    // Reset terminal mode
    set_terminal_mode(0, 0);
}

void print_top_of_priority_boxes(const char titles[3][10]) {
    set_win_color(wc_gray);

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
    set_win_color(wc_gray);

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
    set_win_color(wc_gray);
    printf("%c%c \033[1m", 201, 181);
    set_win_color(wc_light_blue);
    int i = strlen(title);
    printf(title);
    printf("\033[0m");
    set_win_color(wc_gray);
    printf(" %c", 198);

    while (i < BOX_WIDTH) {
        printf("%c", 205);
        i++;
    }

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

    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == ' ') {
            if (n > lengthOfLongestWord) {
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
    if (strlen(message) == 0) {
        return 0;
    }

    int number_of_words = 1;

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

// todo comments
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
