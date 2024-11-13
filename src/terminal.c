#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "general.h"

void box_message(const char message[]) {
    const int messageLength = strlen(message);
    const int messagePadding = 3;
    const int boxWidth = 20;
    const int boxHeight = ceil(messageLength / (double) boxWidth);

    int breaks[boxHeight - 1];
    int breakIndex = boxWidth - 1;
    for (int n = 0; n < boxHeight; n++) {
        while (message[breakIndex] != ' ' && breakIndex >= 0) {
            breakIndex--;
        }

        breaks[n] = breakIndex;
        printf("%d\n", breakIndex);
        breakIndex += boxWidth;
    }

    // Top side of text box.
    printf("%c", 201);
    for (int i = 0; i < boxWidth + 2 * messagePadding; i++) {
        printf("%c", 205);
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
                if (n <= messageLength) {
                    if (n < breaks[i]) {
                        printf("%c", message[n]);
                        n++;
                    } else if (message[n] == ' ') {
                        printf(" ");
                        n++;
                    } else {
                        printf(" ");
                    }
                }
            }
        }
    }

    // Bottom side of text box.
    printf("%c", 200);
    for (int i = 0; i < boxWidth + 2 * messagePadding; i++) {
        printf("%c", 205);
    }
    printf("%c", 188);
}
