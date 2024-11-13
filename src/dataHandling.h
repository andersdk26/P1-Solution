#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include <stdio.h>

typedef struct {
    char name[20];
} location_s;

FILE* open_file(const char *fileName, const char *mode);
int get_file_lines(FILE *file);

#endif //DATAHANDLING_H
