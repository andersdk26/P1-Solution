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

void get_train_routes(const char* origin, const char* destination, route_s **routeList, int *routeListLength);
void get_plane_routes(const char* origin, const char* destination, route_s **routeList, int *routeListLength);

#endif //JOURNEY_H
