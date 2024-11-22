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
    char *inputStart = box_read("Start");
    check_input(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination");
    check_input(inputDestination);

    get_train_routes(inputStart, inputDestination, &routes, &routeQuantity);
    get_plane_routes(inputStart, inputDestination, &routes, &routeQuantity);

    // Free allocated memory.
    free(inputStart);
    free(inputDestination);

    // Print prioritisation instructions.
    box_print(prioritisationInstructions, "Prioritisation");

    // Get user priorities.
    char price = '\0';
    char time = '\0';
    char emission = '\0';
    get_priorities(&price, &time, &emission);

    // int priorities[] = {1, 2, 3};
    // sort_trips(trips, num_trips, priorities);

    // get_journey(&startLocation, &endLocation);

    // get_result();

    // print_table();

    printf("Press any key to exit...");
    char exit = '\0';
    while (exit == '\0') {
        scanf("%c", &exit);
    }

    return EXIT_SUCCESS;
}
