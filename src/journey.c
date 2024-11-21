#include "journey.h"
#include "dataHandling.h"

// void get_journey() {
//
// }

route_s *get_train_routes(const char* input_start, const char* input_destination, route_s **routes, int *routeAmount) {

    FILE *file = search_file(input_start, input_destination, TRAIN_ROUTES_CSV_PATH, routes, routeAmount);

    //print_routes(*routes, *routeAmount);

    return *routes;
}

route_s *get_plane_routes(const char* input_start, const char* input_destination, route_s **routes, int *routeAmount) {

    FILE *file = search_file(input_start, input_destination, FLIGHT_CSV_PATH, routes, routeAmount);

    //print_routes(*routes, *routeAmount);

    return *routes;
}