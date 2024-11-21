#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"
#include "journey.h"

int main(void) {
    route_s startLocation, endLocation;
    preference_e environmentPreference;

    // Print instructions for choosing journey.
    box_print(journeyInstructions, "Journey");

    // Get start location.
    char *inputStart = box_read("Start");
    check_input(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination");
    check_input(inputDestination);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // Print instructions for weighing preferences.
    box_print(preferencesInstructions, "Preferences");

    // Get time weight.
    char *timeWeight = box_read("Time");
    check_input(timeWeight);

    // Get emission weight.
    char *emissionWeight = box_read("Emission");
    check_input(emissionWeight);

    // Get price weight.
    char *priceWeight = box_read("Price");
    check_input(priceWeight);

    // Free allocated memory.
    free(timeWeight);
    free(emissionWeight);
    free(priceWeight);

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
