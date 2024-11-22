#include "journey.h"
#include "dataHandling.h"

// void get_journey() {
//
// }

/**
 * Appends train routes to list
 * @param origin Origin of route
 * @param destination Destination of route
 * @param routeList List to append objects to
 * @param routeListLength Length of list
 */
void get_train_routes(const char* origin, const char* destination, route_s **routeList, int *routeListLength) {
    get_specific_routes(origin, destination, TRAIN_ROUTES_CSV_PATH, tt_train, routeList, routeListLength);
}

/**
 * Appends flights to list
 * @param origin Origin of route
 * @param destination Destination of route
 * @param routeList List to append objects to
 * @param routeListLength Length of list
 */
void get_plane_routes(const char* origin, const char* destination, route_s **routeList, int *routeListLength) {
    get_specific_routes(origin, destination, FLIGHT_CSV_PATH, tt_plane, routeList, routeListLength);
}