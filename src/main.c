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

    char **str = NULL;
    int strLength = 0;

    // Print journey instructions.
    box_print(journeyInstructions, "Journey");

    // Get start location.
    char *inputStart = box_read("Start");
    check_input(inputStart);

    // Get destination.
    char *inputDestination = box_read("Destination");
    check_input(inputDestination);

    get_all_routes(TRAIN_ROUTES_CSV_PATH, tt_train, &routes, &routeQuantity);
    get_all_routes(FLIGHT_CSV_PATH, tt_plane, &routes, &routeQuantity);
    qsort(routes, routeQuantity, sizeof(route_s), alphabetic_route_compare);

    // // test search
    // search_first_column("",&str,&strLength,routes,routeQuantity);
    // printf("%d strings:\n", strLength);
    // for (int i = 0; i < strLength; ++i) {
    //     printf("%s\n", str[i]);
    // }
    //
    // free_string_list(str,strLength,1);
    // str = NULL;
    // strLength = 0;
    //
    // search_second_column("Frankfurt","",&str,&strLength,routes,routeQuantity);
    // printf("%d strings:\n", strLength);
    // for (int i = 0; i < strLength; ++i) {
    //     printf("%s\n", str[i]);
    // }

    // remove_mismatches("Frankfurt", "Hamburg", &routes, &routeQuantity);
    // print_routes(routes, routeQuantity);

    //print_routes(routes, routeQuantity);

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
