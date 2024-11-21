#ifndef JOURNEY_H
#define JOURNEY_H

#include "terminal.h"
#include "general.h"
#include "dataHandling.h"
#include "main.h"

// void get_journey(route_s *start, route_s *end);
//
// void get_preferences(void);
//
// void get_result(void);

route_s *get_train_routes(const char* input_start, const char* input_destination, route_s **routeList, int *routeListLength);
route_s *get_plane_routes(const char* input_start, const char* input_destination, route_s **routes, int *routeAmount);

#endif //JOURNEY_H
