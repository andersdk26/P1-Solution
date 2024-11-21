#include "journey.h"
#include "dataHandling.h"

// void get_journey() {
//
// }

route_s *get_train_routes(const char* input_start, const char* input_destination) {
    int routeAmount = 0;
    route_s *routePointer = NULL;

    FILE *file = search_file(input_start, input_destination, TRAIN_ROUTES_CSV_PATH, &routePointer, &routeAmount);

    fclose(file);

    print_routes(routePointer, routeAmount);

    return routePointer;
}