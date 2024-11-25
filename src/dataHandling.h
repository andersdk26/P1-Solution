#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include <stdio.h>

typedef enum {
    tt_none,
    tt_plane,
    tt_train
} transportType_e;

typedef struct {
    char *origin;
    char *destination;
    char *originName;
    char *destinationName;
    int travelTime; // min
    int emission; // g per passenger
    int price; // kr in 1/100
    int downtime; // min
    transportType_e transportType;
} route_s;

void print_routes(const route_s *list, int length);
FILE* open_file(const char *fileName, const char *mode);
int get_file_lines(FILE *file);
void append_route(route_s **routeList, int *routeListLength, const char *originStr, const char *destinationStr,
                  const char *originNameStr, const char *destinationNameStr, int travelTime, int emission,
                  int price, int downtime, transportType_e transportType);
void remove_route(route_s **routeList, int *routeListLength, int index);
void get_all_routes(const char *fileName, transportType_e transportType, route_s **routes, int *routeAmount);
void append_string(const char *str, char ***stringList, int *stringListLength);
void search_first_column(const char *query, char ***stringList, int *stringListLength,
const route_s *routes, int routeAmount);
void search_second_column(const char *origin, const char *query, char ***stringList, int *stringListLength,
                         const route_s *routes, int routeAmount);
void remove_mismatches(const char *origin, const char *destination, route_s **routes, int *routeAmount);
void free_string_list(char **stringList, int stringListLength, int dynamicList);
void free_route_list(route_s *routeList, int routeListLength, int dynamicList);
int alphabetic_route_compare(const void *vp1, const void *vp2);
#endif //DATAHANDLING_H
