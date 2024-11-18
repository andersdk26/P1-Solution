#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

#define BOX_WIDTH 32
#define BOX_PADDING 2

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
    int breakIndex = BOX_WIDTH;
    for (int n = 0; n < boxHeight; n++) {
        while (message[breakIndex] != ' ' && breakIndex >= 0) {
            breakIndex--;
        }

        breaks[n] = breakIndex;
        //printf("%d\n", breaks[n]);
        breakIndex += BOX_WIDTH;
    }

    // Top side of text box.
    print_top_of_box(BOX_WIDTH + 2 * BOX_PADDING - 3, title);

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
                if (n < breaks[i]) {
                    // printf("%d", n);
                    printf("%c", message[n]);
                    n++;
                } else {
                    printf(" ");
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

void print_top_of_box(const int width, const char title[]) {
    printf("%c", 201);
    printf("%c ", 181);
    printf("\033[1m");

    for (int i = 0; i < width; i++) {
        if (i < strlen(title)) {
            printf("%c", title[i]);
        } else if (i == strlen(title)) {
            printf("\033[0m");
            printf(" %c", 198);
        } else {
            printf("%c", 205);
        }
    }
    printf("%c\n", 187);
}

void print_bottom_of_box(const int width) {
    printf("%c", 200);
    for (int i = 0; i < width; i++) {
        printf("%c", 205);
    }
    printf("%c", 188);
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
