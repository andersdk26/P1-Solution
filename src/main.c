#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"
#include "journey.h"

void print_introduction(const char message[]);

int main(void) {
    route_s *routes = NULL;
    int routeQuanitiy = 0;
    preference_e environmentPreference;

    // Print journey instructions.
    box_print(journeyInstructions, "Journey");

    // Get start location.
    char *inputStart = box_read("Start");
    check_input(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination");
    check_input(inputDestination);

    get_train_routes(inputStart, inputDestination, &routes, &routeQuanitiy);
    get_plane_routes(inputStart, inputDestination, &routes, &routeQuanitiy);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // Print preferences instructions.
    box_print(preferencesInstructions, "Preferences");

    // Get time weight.
    char *timeWeight = box_read("Time");
    check_input(timeWeight);

    // Get price weight.
    char *priceWeight = box_read("Price");
    check_input(priceWeight);

    // Get emission weight.
    char *emissionWeight = box_read("Emission");
    check_input(emissionWeight);

    // Free allocated memory.
    free(timeWeight);
    free(priceWeight);
    free(emissionWeight);

    // get_journey(&startLocation, &endLocation);

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
