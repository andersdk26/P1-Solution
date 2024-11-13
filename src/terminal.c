#include "terminal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

void box_message(const char message[]) {
    const int messageLength = strlen(message);
    const int messagePadding = 0;
    const int boxWidth = 32;
    const int boxHeight = ceil(messageLength / (double) (boxWidth-2));

    int breaks[boxHeight-1];
    int breakIndex = boxWidth;
    for (int n = 0; n < boxHeight; n++) {
        int x = 0;
        while (message[breakIndex] != ' ') {
            breakIndex--;
            x ++;
        }

        breaks[n] = breakIndex;
        printf("%d\n", breakIndex);
        breakIndex += boxWidth+x;
    }

    // Top side of text box.
    printf("%c", 201);
    for (int i = 0; i < boxWidth+2*messagePadding-2; i++) {
        printf("%c", 205);
    }
    printf("%c\n", 187);

    int n = 0;

    // Print box sides.
    for (int i = 0; i < boxHeight; i++) {
        for (int j = 0; j < boxWidth; j++) {
            if (j == 0) {
                printf("%c", 186);
                for (int k = 0; k < messagePadding; k++) {
                    printf(" ");
                }
            } else if (j == boxWidth - 1) {
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
                        n++;
                   } else {
                        printf(" ");
                   }
                } else {
                    printf(" ");
                }
            }
        }
    }

    // Bottom side of text box.
    printf("%c", 200);
    for (int i = 0; i < boxWidth+2*messagePadding-2; i++) {
        printf("%c", 205);
    }
    printf("%c", 188);
}
