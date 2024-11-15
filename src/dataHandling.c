#include "dataHandling.h"

#include <ctype.h>
#include <string.h>

FILE* open_file(const char *fileName, const char *mode) {
    FILE* file = fopen(fileName, mode);
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    return file;
}

int get_file_lines(FILE *file) {
    int count = 0;
    char c;
    while ((c = (char)fgetc(file)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    return count + 1;
}

route_s search_file(const char *origin, const char *destination, const char *fileName) {
    FILE* file = open_file(fileName, "r");
    size_t originLength = strlen(origin);
    size_t destinationLength = strlen(destination);
    route_s route = {"",""};
    int originNotMatching = 0;
    int destinationNotMatching = 0;

    while (fscanf(file,"%s,%s\n",route.origin,route.origin)) {
        if (origin != NULL) {
            originNotMatching = 0;
            for (int i = 0; i < originLength; ++i) {
                if (route.origin[i] != origin[i]) {
                    originNotMatching = 1;
                    break;
                }
            }
        }

        if (destination != NULL) {
            destinationNotMatching = 0;
            for (int i = 0; i < destinationLength; ++i) {
                if (route.destination[i] != destination[i]) {
                    destinationNotMatching = 1;
                    break;
                }
            }
        }

        if (!originNotMatching && !destinationNotMatching) {
            return route;
        }
    }

    route_s noRoute = {'\0','\0'};
    return noRoute;
}