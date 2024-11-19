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

//HANDLE hstdin;
//DWORD mode;

int main(void) {
    location_s startLocation, endLocation;
    preference_e environmentPreference;

    // Print instructions.
    box_print(MESSAGE, "Welcome");

    // Read input.
    char *x = box_read("Input");
    if (x == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    printf(x);
    free(x);

    // get_journey(&startLocation, &endLocation);

    // get_preferences();

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
