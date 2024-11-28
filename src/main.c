#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"
#include "journey.h"

int main(void) {
    while (1) {
        route_s *routes = NULL;
        int routeQuantity = 0;

        // Load routes from files
        get_all_routes(TRAIN_ROUTES_CSV_PATH, tt_train, &routes, &routeQuantity);
        get_all_routes(FLIGHT_CSV_PATH, tt_plane, &routes, &routeQuantity);
        qsort(routes, routeQuantity, sizeof(route_s), alphabetic_route_compare);
        print_routes(routes, routeQuantity);

        // Print journey instructions.
        box_print(journeyInstructions, "Journey");

        // Get start location.
        char *inputStart = box_read("Start", routes, routeQuantity, sic_first, "");

        // Get destination.
        char *inputDestination = box_read("Destination", routes, routeQuantity, sic_second, inputStart);

        // Remove routes that does not match location and destination
        remove_mismatches(inputStart, inputDestination, &routes, &routeQuantity);

        // Free allocated memory.
        free(inputStart);
        free(inputDestination);

        if (routeQuantity < 1) {
            box_print("No route found between your two locations. Please try again.", "Uh oh");
        } else {
            // Print prioritisation instructions.
            box_print(prioritisationInstructions, "Prioritisation");

            // Get user priorities.
            int priorities[] = {0, 0, 0};
            get_priorities(priorities);

            sort_trips(routes, routeQuantity, priorities);

            print_routes(routes, routeQuantity);

            print_journey(routes[0]);
        }

        printf("Press 'Q' to quit or any other key to continue.");
        char exit = '\0';
        while (exit == '\0') {
            scanf("%c", &exit);
        }
        if (tolower(exit) == 'q') {
            break;
        }
        system("cls");
    }

    return EXIT_SUCCESS;
}
