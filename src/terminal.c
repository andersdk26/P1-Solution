#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

HANDLE hstdin;
DWORD mode;

void box_print(char message[], const char title[]) {
    // TODO: Fix wrap selvom der er plads til ordet.
    // TODO: Fix at sidste ord bliver sat på en ny linje.

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
        while (message[breakIndex] != ' ') {
            breakIndex--;
        }
        if (breakIndex < previousBreakIndex || breakIndex > messageLength) {
            break;
        }
        message[breakIndex] = '#';
        previousBreakIndex = breakIndex;
        // printf("%d\n", breakIndex);
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
                delay(15);
                n++;
            }
        }

        printf("\033[1B" "\033[0G");
    }

    // Bottom side of text box.
    print_bottom_of_box(BOX_WIDTH + 2 * BOX_PADDING);
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

    hstdin = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    char *input = memory_allocation(NULL, BOX_WIDTH + 1, 0);
    set_win_color(wc_bright_white);
    read_characters(input, routes, routeQuantity, searchColumn, firstColumn);

    printf("\033[2E");
    set_win_color(wc_gray);

    check_input(input);
    return input;
}

void print_journey(const route_s journey) {
    print_top_of_box("Best journey");

    for (int i = 0; i < 4; i++) {
        print_left_side_of_box();
        for (int j = 0; j < BOX_WIDTH; j++) {
            printf(" ");
        }
        print_right_side_of_box();
    }

    set_win_color(wc_bright_white);

    printf("\033[4A");

    printf("\033[0G");
    printf("\033[%dC", 1 + BOX_PADDING);
    printf("From %s to %s by %s", journey.origin, journey.destination, journey.transportType == 1 ? "Airplane" : "Train");
    printf("\033[1B");

    printf("\033[0G");
    printf("\033[%dC", 1 + BOX_PADDING);
    printf("Time:\t%d minutes", journey.travelTime);
    printf("\033[1B");

    printf("\033[0G");
    printf("\033[%dC", 1 + BOX_PADDING);
    printf("Price:\t%.2lf EUR", journey.price / 100.0);
    printf("\033[1B");

    printf("\033[0G");
    printf("\033[%dC", 1 + BOX_PADDING);
    printf("Emission:\t%d CO2e", journey.emission);
    printf("\033[1B");

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

    // Get priority for first box.
    char p = '\0';
    while (p < 49 || p > 51) {
        p = getchar();
    }
    priorities[0] = (int) p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor to the second box.
    printf("\033[9C");

    // Get priority for second box.
    while (p < 49 || p > 51 || (int) p - '0' == priorities[0]) {
        p = getchar();
    }
    priorities[1] = (int) p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor to the third box.
    printf("\033[10C");

    // Get priority for third box.
    while (p < 49 || p > 51 || (int) p - '0' == priorities[0] || (int) p - '0' == priorities[1]) {
        p = getchar();
    }
    priorities[2] = (int) p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor out of the box and change text color.
    printf("\033[2B");
    printf("\033[0G");
    set_win_color(wc_gray);
}

void read_characters(char *input, const route_s *routes, const int routeQuantity, const searchInColumn_e searchColumn,
                     const char *firstColumn) {
    int i = 0;
    char c;
    char **strings = NULL;
    int stringsAmount = 0;

    while (1) {
        scanf(" %c", &c);
        if (i > 0) {
            if (c == 8) {
                printf("\033[1D \033[1D");
                input[i] = '\0';
                i--;
            } else if (c == 46) {
                break;
            }
        }

        if (i < BOX_WIDTH && isalpha(c)) {
            printf("%c", c);
            input[i] = c;
            input[i + 1] = '\0';
            i++;
        }

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

            int stringLength = (int) strlen(strings[0]);
            if (stringsAmount > 0 && stringLength > i) {
                printf("%s ", strings[0] + i);
                for (int j = 0; j < stringLength - i + 1; ++j) {
                    printf("\033[1D");
                }
            }

            set_win_color(wc_bright_white);
        }
    }
    input[i] = '\0';
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
