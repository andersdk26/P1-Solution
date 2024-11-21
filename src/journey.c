#include "journey.h"
#include "dataHandling.h"

// void get_journey() {
//
// }

route_s *get_train_routes(const char* input_start, const char* input_destination) {
    route_s *routePointer = NULL;

    route_s element = search_file(input_start, input_destination, TRAIN_ROUTES_CSV_PATH);

    printf("%s,%s\n", element.origin, element.destination);

    return routePointer;
}