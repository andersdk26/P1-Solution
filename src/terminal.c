#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

#define BOX_WIDTH 16
#define BOX_PADDING 2

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
        printf("%d\n", breakIndex);
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
                            } else {
                                printf("^");
                                m++;
                            }
                        } else {
                            printf("_");
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

char *box_read(const char title[]) {
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

    char *input = malloc(BOX_WIDTH + 1);
    set_win_color(wc_bright_white);
    read_characters(input);

    printf("\033[2E");
    set_win_color(wc_gray);
    return input;
}

void get_priorities(char *price, char *time, char *emission) {
    const char titles[3][10] = {{"Time"}, {"Price"}, {"Emission"}};

    print_top_of_priority_boxes(titles);
    print_middle_of_priority_boxes(titles);
    print_bottom_of_priority_boxes(titles);

    printf("\033[2A");
    printf("\033[3C");
    set_win_color(wc_bright_white);

    char c = '\0';
    while (c < 49 || c > 51) {
        scanf(" %c", &c);
    }

    *price = c;
    printf("%c", c);
    c = '\0';

    printf("\033[9C");

    while (c < 49 || c > 51 || c == *price) {
        scanf(" %c", &c);
    }

    *time = c;
    printf("%c", c);
    c = '\0';

    printf("\033[10C");

    while (c < 49 || c > 51 || c == *price || c == *time) {
        scanf(" %c", &c);
    }

    *emission = c;
    printf("%c", c);

    printf("\033[2B");
    printf("\033[0G");
    set_win_color(wc_gray);
}

void read_characters(char *input) {
    // TODO: Fix warning?
    int i = 0;
    char c;
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
            i++;
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
