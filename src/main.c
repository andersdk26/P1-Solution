#include <stdio.h>
#include <stdlib.h>

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

    get_all_routes(TRAIN_ROUTES_CSV_PATH, tt_train, &routes, &routeQuantity);
    get_all_routes(FLIGHT_CSV_PATH, tt_plane, &routes, &routeQuantity);
    qsort(routes, routeQuantity, sizeof(route_s), alphabetic_route_compare);

    print_routes(routes, routeQuantity);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // Print prioritisation instructions.
    box_print(prioritisationInstructions, "Prioritisation");

    char a;
    char b;
    char c;

    get_priorities(&a, &b, &c);

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
