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
void get_specific_routes(const char *originQuery, const char *destinationQuery, const char *fileName,
    transportType_e transportType, route_s **routes, int *routeAmount);
void search_column_string(int columnNo, const char *query, const char *fileName, char **stringList,
    int *stringListLength);
void free_string_list(char **stringList, int stringListLength, int dynamicList);
void free_route_list(route_s *routeList, int routeListLength, int dynamicList);
#endif //DATAHANDLING_H
