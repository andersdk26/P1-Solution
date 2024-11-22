#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"
#include "journey.h"

int main(void) {
    route_s *routes = NULL;
    int routeQuantity = 0;
    preference_e environmentPreference;

    // Print journey instructions.
    box_print(journeyInstructions, "Journey");

    // Get start location.
    char *inputStart = box_read("Start", STRING);
    check_input(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination", STRING);
    check_input(inputDestination);

    get_train_routes(inputStart, inputDestination, &routes, &routeQuantity);
    get_plane_routes(inputStart, inputDestination, &routes, &routeQuantity);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // Print prioritisation instructions.
    box_print(prioritisationInstructions, "Prioritisation");

    char a;
    char b;
    char c;

    get_priorities(&a, &b, &c);
    int priorities[] = {1, 2, 3};

    sort_trips(trips, num_trips, priorities);

    /*
    // Get time weight.
    char *timeWeight = box_read("Time", NUMBER);
    check_input(timeWeight);

    // Get price weight.
    char *priceWeight = box_read("Price", NUMBER);
    check_input(priceWeight);

    // Get emission weight.
    char *emissionWeight = box_read("Emission", NUMBER);
    check_input(emissionWeight);

    // Free allocated memory.
    free(timeWeight);
    free(priceWeight);
    free(emissionWeight);

    */

    // get_journey(&startLocation, &endLocation);

    // get_result();

    // print_table();

    while (1) {
    }

    return EXIT_SUCCESS;
}
