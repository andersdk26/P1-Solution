#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "dataHandling.h"
#include "terminal.h"
#include "journey.h"

void print_introduction(const char message[]);

HANDLE hstdin;
DWORD mode;

int main(void) {
    location_s startLocation, endLocation;
    preference_e environmentPreference;

    // Print instructions.
    box_message(introduction, "Welcome");

    box_message("", "Start");

    hstdin = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    printf("\033[7;3H");

    int i = 0;
    char c;
    while (i <= 32) {
        printf("\033[5;%dH", i + 3);
        printf("");
        scanf("%c", &c);

        if (c == 8) {
            i--;
        } else {
            printf("%c", c);
            i++;
        }
    }
    printf("\033[2E");

    box_message("", "Destination");

    // get_journey(&startLocation, &endLocation);

    // get_preferences();

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
