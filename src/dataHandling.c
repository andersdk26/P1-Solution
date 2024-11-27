#include "dataHandling.h"
#include "general.h"
#include <ctype.h>
#include <string.h>
#include <math.h>

/**
 * Prints the given list of routes
 * @param list List of routes
 * @param length Length of list
 */
void print_routes(const route_s* list, const int length) {
    printf("Printing %d routes:\n", length);
    printf("%-12s" "%-12s" "%7s" "%7s" "%7s" "\n",
            "Origin", "Destination", "Time", "Price", "Emmision");
    for (int i = 0; i < length; ++i) {
        printf("%-12s" "%-12s" "  %5d" "  %6.2lf" "  %5d" "\n",
            list[i].origin, list[i].destination, list[i].travelTime, (double)list[i].price / 100, list[i].emission);
    }
}

/**
 * Opens a file
 * @param fileName Path and name of file
 * @param mode R/W mode
 * @return File object
 */
FILE* open_file(const char* fileName, const char* mode) {
    FILE* file = fopen(fileName, mode);
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    return file;
}

/**
 * Counts lines in a file
 * @param file File object to read
 * @return Amount of lines in the file
 */
int get_file_lines(FILE* file) {
    int count = 0;
    char c;
    while ((c = (char)fgetc(file)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    return count + 1;
}

/**
 * Appends route object at then end of a list
 * @param routeList Pointer to list of routes
 * @param routeListLength length of routeList
 * @param originStr route_s element
 * @param destinationStr route_s element
 * @param originNameStr route_s element
 * @param destinationNameStr route_s element
 * @param travelTime route_s element
 * @param emission route_s element
 * @param price route_s element
 * @param downtime route_s element
 * @param transportType route_s element
 */
void append_route(route_s** routeList, int* routeListLength, const char* originStr, const char* destinationStr,
                  const char* originNameStr, const char* destinationNameStr, const int travelTime, const int emission,
                  const int price, const int downtime, const transportType_e transportType) {
    // Extend route array by 1 object
    (*routeListLength)++;
    *routeList = memory_allocation(*routeList, *routeListLength * sizeof(route_s), 0);

    // Calculates pointer to last (new) object in list
    route_s* object = (*routeList) + *routeListLength - 1;

    // Insert numbers
    object->travelTime = travelTime;
    object->emission = emission;
    object->price = price;
    object->downtime = downtime;
    object->transportType = transportType;

    // Dynamic allocate strings and insert them as pointers
    object->origin = memory_allocation(NULL, strlen(originStr) + 1, 0);
    strcpy(object->origin, originStr);
    object->destination = memory_allocation(NULL, strlen(destinationStr) + 1, 0);
    strcpy(object->destination, destinationStr);

    object->originName = memory_allocation(NULL, strlen(originNameStr) + 1, 0);
    strcpy(object->originName, originNameStr);
    object->destinationName = memory_allocation(NULL, strlen(destinationNameStr) + 1, 0);
    strcpy(object->destinationName, destinationNameStr);
}

/**
 * Removes route from list
 * @param routeList List to remove in
 * @param routeListLength Length of list
 * @param index Index to remove
 */
void remove_route(route_s** routeList, int* routeListLength, const int index) {
    free((*routeList)[index].origin);
    free((*routeList)[index].destination);
    free((*routeList)[index].originName);
    free((*routeList)[index].destinationName);

    for (int i = index; i < *routeListLength - 1; ++i) {
        (*routeList)[i] = (*routeList)[i + 1];
    }

    (*routeListLength)--;
    *routeList = memory_allocation(*routeList, *routeListLength * sizeof(route_s), 0);
}

// todo: error handling
/**
 * Searches for specific route in a file and appends resulting object to routes
 * (File should be alphabetically ordered)
 * @param fileName File to search in
 * @param transportType Transportation type in file
 * @param routes Pointer to route list
 * @param routeAmount Pointer to route list length
 */
void get_all_routes(const char* fileName, const transportType_e transportType, route_s** routes, int* routeAmount) {
    FILE* file = open_file(fileName, "r");

    // Temporary strings
    const int lineLength = 200;
    char line[lineLength];
    char originStr[50], destinationStr[50], originNameStr[50], destinationNameStr[50],
         travelTimeStr[10], emissionStr[10], priceStr[10], downtimeStr[10];

    fgets(line, lineLength, file); // Read first irrelevant row
    while (fgets(line, lineLength, file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
               originStr, destinationStr, originNameStr, destinationNameStr,
               travelTimeStr, emissionStr, priceStr, downtimeStr);

        // parse parameter values and append them to routes
        append_route(routes, routeAmount,
                     originStr, destinationStr, originNameStr, destinationNameStr,
                     strtol(travelTimeStr,NULL, 10),
                     (int)round(strtod(emissionStr,NULL)), // Convert decimal to integer
                     (int)(strtod(emissionStr,NULL) * 100), // Convert price to 1/100
                     strtol(downtimeStr,NULL, 10),
                     transportType);
    }

    fclose(file);
}

/**
 * Append string to list
 * @param str String to append
 * @param stringList List of strings
 * @param stringListLength Length of list
 */
void append_string(const char* str, char*** stringList, int* stringListLength) {
    (*stringListLength)++;
    *stringList = memory_allocation(*stringList, *stringListLength * sizeof(char*), 0);

    (*stringList)[*stringListLength - 1] = memory_allocation(NULL, strlen(str) + 1, 0);

    strcpy((*stringList)[*stringListLength - 1], str);
}

/**
 * Findes all instances (partially) matching query in route array
 * @param query Query to search for
 * @param stringList List to insert matches
 * @param stringListLength Length of list
 * @param routes Array to search in
 * @param routeAmount Length of array
 */
void search_first_column(const char* query, char*** stringList, int* stringListLength,
                         const route_s* routes, const int routeAmount) {
    char query2[50];
    strcpy(query2, query);
    strcat(query2, "~");

    for (int i = 0; i < routeAmount; ++i) {
        if (i > 0 && strcmp(routes[i - 1].origin, routes[i].origin) == 0) {
            continue;
        }
        if (strcmp(query, routes[i].origin) <= 0 && strcmp(query2, routes[i].origin) > 0) {
            append_string(routes[i].origin, stringList, stringListLength);
        }
    }
}


/**
 * Finds all instances (partially) matching query in route array
 * @param origin Origin to match (exactly)
 * @param query Query to search for
 * @param stringList List to insert matches
 * @param stringListLength Length of list
 * @param routes Array to search in
 * @param routeAmount Length of array
 */
void search_second_column(const char* origin, const char* query, char*** stringList, int* stringListLength,
                          const route_s* routes, const int routeAmount) {
    char query2[50];
    strcpy(query2, query);
    strcat(query2, "~");

    for (int i = 0; i < routeAmount; ++i) {
        if (strcmp(origin, routes[i].origin) != 0) {
            continue;
        }
        if (i > 0 && strcmp(routes[i - 1].destination, routes[i].destination) == 0) {
            continue;
        }
        if (strcmp(query, routes[i].destination) <= 0 && strcmp(query2, routes[i].destination) > 0) {
            append_string(routes[i].destination, stringList, stringListLength);
        }
    }
}

/**
 * Removes all routes not matching origin and destination
 * @param origin Origin string
 * @param destination Destination string
 * @param routes Routes to search in
 * @param routeAmount Amount of routes
 */
void remove_mismatches(const char* origin, const char* destination, route_s** routes, int* routeAmount) {
    for (int i = *routeAmount - 1; i >= 0; i--) {
        if (strcmp((*routes)[i].origin, origin) == 0 && strcmp((*routes)[i].destination, destination) == 0) {
            // Route matches origin and destination
            continue;
        }
        remove_route(routes, routeAmount, i);
    }
}

/**
 * Frees string list from heap memory
 * @param stringList List to free
 * @param stringListLength Length of list
 * @param dynamicList True for dynamic lists
 */
void free_string_list(char** stringList, const int stringListLength, const int dynamicList) {
    for (int i = 0; i < stringListLength; ++i) {
        free(stringList[i]);
    }
    if (dynamicList) {
        free(stringList);
    }
}

/**
 * Frees route list from heap memory
 * @param routeList List to free
 * @param routeListLength Length of list
 * @param dynamicList True for dynamic lists
 */
void free_route_list(route_s* routeList, const int routeListLength, const int dynamicList) {
    for (int i = 0; i < routeListLength; ++i) {
        free(routeList[i].origin);
        free(routeList[i].destination);
        free(routeList[i].originName);
        free(routeList[i].destinationName);
    }
    if (dynamicList) {
        free(routeList);
    }
}

/**
 * Compares two routes alphabetically
 * @param vp1 Element 1
 * @param vp2 Element 2
 * @return 1: E1 > E2, -1: E1 > E2, 0: E1 == E2
 */
int alphabetic_route_compare(const void* vp1, const void* vp2) {
    const route_s* route1 = (route_s*)vp1;
    const route_s* route2 = (route_s*)vp2;

    const int result = strcmp(route1->origin, route2->origin);
    if (result != 0) {
        return result;
    }

    return strcmp(route1->destination, route2->destination);
}

int compare_trips(void* param, const void* a, const void* b) {
    const route_s* trip_a = (const route_s*)a;
    const route_s* trip_b = (const route_s*)b;
    int* priorities = (int*)param;

    for (int i = 0; i < 3; ++i) {
        switch (priorities[i]) {
        case 1:
            if (trip_a->price < trip_b->price) return -1;
            if (trip_a->price > trip_b->price) return 1;
            break;
        case 2:
            if (trip_a->travelTime + trip_a->downtime < trip_b->travelTime + trip_b->downtime) return -1;
            if (trip_a->travelTime + trip_a->downtime > trip_b->travelTime + trip_b->downtime) return 1;
            break;
        case 3:
            if (trip_a->emission > trip_b->emission) return -1;
            if (trip_a->emission < trip_b->emission) return 1;
            break;
        default:
            break;
        }
    }
    return 0;
}

void sort_trips(route_s* trips, const size_t num_trips, void* priorities) {
    qsort_s(trips, num_trips, sizeof(route_s), compare_trips, (void*)priorities);
}
