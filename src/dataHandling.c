#include "dataHandling.h"
#include "general.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void print_routes(const route_s *list, const int length) {
    for (int i = 0; i < length; ++i) {
        printf("%s,%s\n", list[i].origin, list[i].destination);
    }
}

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

FILE *search_file(const char *origin, const char *destination, const char *fileName, route_s **routes, int *routeAmount) {
    FILE* file = open_file(fileName, "r");
    size_t originLength = strlen(origin);
    size_t destinationLength = strlen(destination);
    route_s route = {"",""};
    int originNotMatching = 0;
    int destinationNotMatching = 0;
    const int lineLength = 200;
    char line[lineLength];
    char originStr[100], destinationStr[100];

    fgets(line, lineLength, file);
    while (fgets(line, lineLength, file) != NULL) {
        if (1) {
            (*routeAmount)++;
            *routes = memory_allocation(*routes, *routeAmount * sizeof(route_s),0);
            sscanf(line, "%[^,],%[^,]", originStr, destinationStr);
            (*routes)[*routeAmount-1].origin = memory_allocation(NULL,strlen(originStr)+1,0);
            strcpy((*routes)[*routeAmount-1].origin, originStr);
            (*routes)[*routeAmount-1].destination = memory_allocation(NULL,strlen(destinationStr)+1,0);
            strcpy((*routes)[*routeAmount-1].destination, destinationStr);
        }


        // if (origin != NULL) {
        //     originNotMatching = 0;
        //     for (int i = 0; i < originLength; ++i) {
        //         if (route.origin[i] != origin[i]) {
        //             originNotMatching = 1;
        //             break;
        //         }
        //     }
        // }
        //
        // if (destination != NULL) {
        //     destinationNotMatching = 0;
        //     for (int i = 0; i < destinationLength; ++i) {
        //         if (route.destination[i] != destination[i]) {
        //             destinationNotMatching = 1;
        //             break;
        //         }
        //     }
        // }
        //
        // if (!originNotMatching && !destinationNotMatching) {
        //     return route;
        // }
    }

    fclose(file);
    return NULL;
}

typedef struct Journey {
    char start[50];
    char destination[50];
    float price;
    int duration;
    int environmental_impact;
} Trip;

int compare_trips(const void *a, const void *b, void *param) {
    const Trip *trip_a = (const Trip *)a;
    const Trip *trip_b = (const Trip *)b;
    int *priorities = (int *)param;

    for (int i = 0; i < 3; ++i) {
        switch (priorities[i]) {
            case 1:
                if (trip_a->price < trip_b->price) return -1;
            if (trip_a->price > trip_b->price) return 1;
            break;
            case 2:
                if (trip_a->duration < trip_b->duration) return -1;
            if (trip_a->duration > trip_b->duration) return 1;
            break;
            case 3:
                if (trip_a->environmental_impact > trip_b->environmental_impact) return -1;
            if (trip_a->environmental_impact < trip_b->environmental_impact) return 1;
            break;
        }
    }
    return 0;
}


void sort_trips(Trip *trips, size_t num_trips, int *priorities) {
    qsort_s(trips, num_trips, sizeof(Trip), compare_trips, priorities);
}

