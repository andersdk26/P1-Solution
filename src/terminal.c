#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

void box_message(const char message[], const char title[]) {
    const int messageLength = strlen(message);
    const int messagePadding = 2;
    int boxWidth = 32;
    if (boxWidth < get_length_of_longest_word(message)) {
        printf("WARNING: 'boxWidth' is less than the length of the longest word in the message and has been adjusted.\n");
        boxWidth = get_length_of_longest_word(message);
    }

    // TODO sæt boxHeight til antal ord i beskeden.
    //const int boxHeight = ceil(messageLength / (double) boxWidth);
    const int boxHeight = 1;

    int breaks[boxHeight - 1];
    int breakIndex = boxWidth - 1;
    for (int n = 0; n < boxHeight; n++) {
        while (message[breakIndex] != ' ' && breakIndex >= 0) {
            breakIndex--;
        }

        breaks[n] = breakIndex;
        breakIndex += boxWidth;
    }

    // Top side of text box.
    printf("%c", 201);
    printf("%c ", 181);
    printf("\033[1m");

    for (int i = 0; i < boxWidth + 2 * messagePadding - 3; i++) {
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

    int n = 0;

    // Print box sides.
    for (int i = 0; i < boxHeight; i++) {
        for (int j = -1; j <= boxWidth; j++) {
            if (j == -1) {
                // Print left side of box plus padding.
                printf("%c", 186);
                for (int k = 0; k < messagePadding; k++) {
                    printf(" ");
                }
            } else if (j == boxWidth) {
                // Print padding plus right side of box.
                for (int k = 0; k < messagePadding; k++) {
                    printf(" ");
                }
                printf("%c\n", 186);
            } else {
                if (n - 1 <= messageLength) {
                    if (n < breaks[i]) {
                        printf("%c", message[n]);
                        n++;
                    } else if (message[n] == ' ') {
                        printf(" ");
                        n++;
                    } else {
                        printf(" ");
                    }
                } else {
                    printf(" ");
                }
            }
        }
        if (n > messageLength) {
            i = boxHeight - 1;
        }
    }

    // Bottom side of text box.
    printf("%c", 200);
    for (int i = 0; i < boxWidth + 2 * messagePadding; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 188);
}

int get_length_of_longest_word(const char message[]) {
    int lengthOfLongestWord = 0;
    int n = 0;

    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == ' ') {
            if (n > lengthOfLongestWord) {
                lengthOfLongestWord = n;
            }
            n = -1;
        } else {
            n++;
        }
    }

    return lengthOfLongestWord;
}
