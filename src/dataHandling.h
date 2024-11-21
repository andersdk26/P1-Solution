#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include <stdio.h>


typedef struct {
    char *origin;
    char *destination;
} route_s;

void print_routes(const route_s *list, int length);
FILE* open_file(const char *fileName, const char *mode);
int get_file_lines(FILE *file);
FILE *search_file(const char *origin, const char *destination, const char *fileName, route_s **routes, int *routeAmount);

#endif //DATAHANDLING_H
