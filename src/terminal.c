#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

#define BOX_WIDTH 32
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
        // if (message[breakIndex] != ' ') {
        // } else if (message[breakIndex + 1] != ' ') {
        // } else if (breakIndex > 0) {
        //     breakIndex--;
        // }

        // while (message[breakIndex] != ' ' || message[breakIndex + 1] != ' ') {
        //     if (breakIndex > 0) {
        //         breakIndex--;
        //     }
        // }

        while (message[breakIndex] != ' ' && message[breakIndex + 1] != ' ') {
            breakIndex--;
        }

        breaks[n] = breakIndex;
        printf("%d\n", breaks[n]);
        breakIndex += BOX_WIDTH;
    }

    // Top side of text box.
    print_top_of_box(title);

    int n = 0;

    // For every line of text in the box.
    for (int i = 0; i < boxHeight; i++) {
        // For the width of the box.
        for (int j = 0; j < BOX_WIDTH; j++) {
            // If it's the first iteration, print the left side of the box.
            if (j == 0) {
                printf("%c", 186);
                for (int k = 0; k < BOX_PADDING; k++) {
                    printf(" ");
                }
            }

            // Otherwise, print characters from message.
            if (n < messageLength) {
                if (n <= breaks[i]) {
                    //printf("%d", n%16);
                    if (n % BOX_WIDTH == 0 && message[n] == ' ') {
                    } else {
                        printf("%c", message[n]);
                    }
                    n++;
                } else {
                    printf("^");
                    n = breaks[i] + 1;
                }
            } else {
                // If end of message has been reached, then print empty spaces.
                printf(" ");
            }

            // If it's the last iteration, print the right side of the box.
            if (j == BOX_WIDTH - 1) {
                for (int k = 0; k < BOX_PADDING; k++) {
                    printf(" ");
                }
                printf("%c\n", 186);
            }
        }

        // If end of message has been reached, break out of the for-loop.
        if (n == messageLength) {
            break;
        }
    }

    // Bottom side of text box.
    print_bottom_of_box(BOX_WIDTH + 2 * BOX_PADDING);
}

char *box_read(const char title[]) {
    print_top_of_box(title);
    printf("%c", 186);
    for (int i = 0; i < BOX_WIDTH + 2 * BOX_PADDING; i++) {
        printf(" ");
    }
    printf("%c\n", 186);
    print_bottom_of_box();

    printf("\033[2A");
    printf("\033[%dC", 1 + BOX_PADDING);

    hstdin = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    int i = 0;
    char c;
    char *input = malloc(BOX_WIDTH + 1);

    while (1) {
        scanf("%c", &c);

        if (i > 0) {
            if (c == 8) {
                printf("\033[1D");
                printf(" ");
                printf("\033[1D");
                input[i] = "";
                i--;
            } else if (c == 13) {
                break;
            }
        }

        if (i < BOX_WIDTH && c != 8) {
            printf("%c", c);
            input[i] = c;
            i++;
        }
    }
    input[i + 1] = '\0';
    printf("\033[2E");
    return input;
}

void print_top_of_box(const char title[]) {
    printf("%c", 201);
    printf("%c ", 181);
    printf("\033[1m");

    int i = 0;

    for (i = 0; i < BOX_WIDTH; i++) {
        if (i < strlen(title)) {
            printf("%c", title[i]);
        } else {
            break;
        }
    }

    printf("\033[0m");
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
