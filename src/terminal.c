#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

HANDLE hstdin;
DWORD mode;

void box_print(const char message[], const char title[]) {
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

    int breaks[boxHeight - 1];
    int breakIndex = BOX_WIDTH - 1;
    for (int n = 0; n < boxHeight; n++) {
        while (message[breakIndex] != ' ') {
            breakIndex--;
        }
        breaks[n] = breakIndex;
        // printf("%d\n", breakIndex);
        breakIndex += BOX_WIDTH;
    }

    // Top side of text box.
    print_top_of_box(title);

    int m = 0;
    // For every line of text in the box.
    for (int i = 0; i < boxHeight; i++) {
        // For the left side, middle and right side of the box.
        for (int j = 0; j < 3; j++) {
            switch (j) {
            case 0: // Print left side of the box.
                set_win_color(wc_gray);
                printf("%c", 186);
                for (int k = 0; k < BOX_PADDING; k++) {
                    printf(" ");
                }
                break;
            case 1: // Print line of text.
                set_win_color(wc_bright_white);
                int n = 0;
                while (n < BOX_WIDTH) {
                    if (BOX_WIDTH * i + n - m < messageLength) {
                        if (n <= breaks[i]) {
                            printf("%c", message[BOX_WIDTH * i + n - m]);
                        }
                        else {
                            printf(" ");
                            m++;
                        }
                    }
                    else {
                        printf(" ");
                    }

                    if (BOX_WIDTH * i + n - m >= messageLength) {
                        i = boxHeight - 1;
                    }
                    n++;
                }
                break;
            case 2: // Print right side of the box.
                set_win_color(wc_gray);
                for (int k = 0; k < BOX_PADDING; k++) {
                    printf(" ");
                }
                printf("%c\n", 186);
                break;
            default:
                printf("Error\n");
                exit(EXIT_FAILURE);
            }

            /*
            // Otherwise, print characters from message.
            set_win_color(wc_bright_white);
            if (n < messageLength) {
                if (n <= breaks[i]) {
                    printf("%d", n);
                    if (n % BOX_WIDTH == 0 && message[n] == ' ') {
                    } else {
                        printf("%c", message[n]);
                    }
                    n++;
                } else {
                    printf("^");
                    //n = breaks[i] + 1;
                }
            } else {
                // If end of message has been reached, then print empty spaces.
                printf("_");
            }
            */
        }

        // If end of message has been reached, break out of the for-loop.
        //if (n == messageLength) {
        //    break;
        //}
    }

    // Bottom side of text box.
    print_bottom_of_box(BOX_WIDTH + 2 * BOX_PADDING);
}

char* box_read(const char title[], const route_s* routes, const int routeQuantity, const searchInColumn_e searchColumn,
               const char* firstColumn) {
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

    char* input = memory_allocation(NULL, BOX_WIDTH + 1, 0);
    set_win_color(wc_bright_white);
    read_characters(input, routes, routeQuantity, searchColumn, firstColumn);

    printf("\033[2E");
    set_win_color(wc_gray);

    check_input(input);
    return input;
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
    priorities[0] = (int)p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor to the second box.
    printf("\033[9C");

    // Get priority for second box.
    while (p < 49 || p > 51 || (int)p == priorities[0]) {
        p = getchar();
    }
    priorities[1] = (int)p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor to the third box.
    printf("\033[10C");

    // Get priority for third box.
    while (p < 49 || p > 51 || (int)p == priorities[0] || (int)p == priorities[1]) {
        p = getchar();
    }
    priorities[2] = (int)p - '0';
    printf("%c", p);
    p = '\0';

    // Move cursor out of the box and change text color.
    printf("\033[2B");
    printf("\033[0G");
    set_win_color(wc_gray);
}

void read_characters(char* input, const route_s* routes, const int routeQuantity, const searchInColumn_e searchColumn,
                     const char* firstColumn) {
    int i = 0;
    char c;
    char** strings = NULL;
    int stringsAmount = 0;

    while (1) {
        scanf(" %c", &c);
        if (i > 0) {
            if (c == 8) {
                printf("\033[1D \033[1D");
                input[i] = '\0';
                i--;
            }
            else if (c == 46) {
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
            }
            else {
                search_first_column(input, &strings, &stringsAmount, routes, routeQuantity);
            }

            if (stringsAmount < 1) {
                set_win_color(wc_white);
                continue;
            }

            int stringLength = (int)strlen(strings[0]);
            if (stringsAmount > 0 && stringLength > i) {
                printf("%s ", strings[0] + i);
                for (int j = 0; j < stringLength - i + 1; ++j) {
                    printf("\033[1D");
                }
            }

            set_win_color(wc_white);
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
    printf("%c", 200);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);
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
        }
        else {
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
