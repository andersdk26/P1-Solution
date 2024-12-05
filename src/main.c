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

        // Load routes from files and sort alphabetically
        get_all_routes(TRAIN_ROUTES_CSV_PATH, tt_train, &routes, &routeQuantity);
        get_all_routes(FLIGHT_CSV_PATH, tt_plane, &routes, &routeQuantity);
        //get_all_routes("../data/test.csv", tt_plane, &routes, &routeQuantity);
        qsort(routes, routeQuantity, sizeof(route_s), alphabetic_route_compare);

        // Print start location instructions.
        box_print(instructionsStart, "Journey");

        // Get start location.
        char *inputStart = box_read("Start", routes, routeQuantity, sic_first, "");

        // Print destination instructions.
        box_print(instructionsDestination, "Journey");

        // Get destination.
        char *inputDestination = box_read("Destination", routes, routeQuantity, sic_second, inputStart);

        // Remove routes that does not match location and destination
        remove_mismatches(inputStart, inputDestination, &routes, &routeQuantity);

        // Free allocated memory.
        free(inputStart);
        free(inputDestination);

        if (routeQuantity < 1) {
            box_print(noJourney, "Uh oh");
        } else {
            // Print prioritisation instructions.
            box_print(prioritisationInstructions, "Prioritisation");

            // Get user priorities.
            int priorities[] = {-1, -1, -1};
            get_priorities(priorities);

            sort_trips(routes, routeQuantity, priorities);

            //print_routes(routes, routeQuantity);

            // TODO: Print tabel over alternative rejser.

            print_journey(routes[0]);
        }

        set_terminal_mode(ENABLE_WINDOW_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT
                          ,ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
        delay(3000);
        box_print(endMessage, "End");
        char exit = '\0';
        while (exit == '\0') {
            scanf("%c", &exit);
        }
        if (tolower(exit) == 'q') {
            break;
        }
        system("cls");
        set_terminal_mode(0, 0);
    }
    set_terminal_mode(0, 0);

    return EXIT_SUCCESS;
}
