#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include <stdio.h>

#define LOCATION_STRING_LENGTH 20

typedef struct {
    char origin[LOCATION_STRING_LENGTH];
    char destination[LOCATION_STRING_LENGTH];
} route_s;

FILE* open_file(const char *fileName, const char *mode);
int get_file_lines(FILE *file);
route_s search_file(const char *origin, const char *destination, const char *fileName);

#endif //DATAHANDLING_H
