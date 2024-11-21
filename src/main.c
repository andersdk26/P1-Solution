#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"
#include "journey.h"

void print_introduction(const char message[]);

//HANDLE hstdin;
//DWORD mode;

int main(void) {
    route_s startLocation, endLocation;
    preference_e environmentPreference;

    // Print instructions.
    box_print(MESSAGE, "Welcome");

    // Get start location.
    char *inputStart = box_read("Start");
    check_input(inputStart);
    free(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination");
    check_input(inputDestination);

    trainRoutes = get_train_routes(inputStart, inputDestination);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // get_journey(&startLocation, &endLocation);

    // get_preferences();

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
