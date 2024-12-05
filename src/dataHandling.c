#include "dataHandling.h"
#include "general.h"
#include <string.h>
#include <math.h>

#include "terminal.h"

/**
 * Prints the given list of routes
 * @param list List of routes
 * @param length Length of list
 */
void print_routes(const route_s* list, const int length) {
    set_win_color(wc_default);
    printf("Printing %d routes:\n", length);

    printf("%-12s" "%-12s" "%7s" "%8s" "%9s" "\n",
            "Origin", "Destination", "Time", "Price", "Emission");

    // Prints all routes
    for (int i = 0; i < length; ++i) {
        printf("%-12s" "%-12s" "  %5d" "  %6.2lf" "   %5d" "\n",
            list[i].origin, list[i].destination, list[i].travelTime, (double)list[i].price / 100, list[i].emission);
    }
}

/**
 * Opens a file
 * @param fileName Path and name of file
 * @param mode R/W mode
 * @return File object
 */
FILE *open_file(const char *fileName, const char *mode) {
    FILE *file = fopen(fileName, mode);

    // Check for errors
    if (file == NULL) {
        print_error("Could not open file");
    }

    return file;
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
void append_route(route_s **routeList, int *routeListLength, const char *originStr, const char *destinationStr,
                  const char *originNameStr, const char *destinationNameStr, const int travelTime, const int emission,
                  const int price, const int downtime, const transportType_e transportType) {
    // Extend route array by 1 object
    (*routeListLength)++;
    *routeList = memory_allocation(*routeList, *routeListLength * sizeof(route_s), 0);

    // Calculates pointer to last (new) object in list
    route_s *object = (*routeList) + *routeListLength - 1;

    // Insert numbers
    object->travelTime = travelTime;
    object->emission = emission;
    object->price = price;
    object->downtime = downtime;
    object->transportType = transportType;

    // Dynamic allocate strings and insert them as pointers
    // Origin
    object->origin = memory_allocation(NULL, strlen(originStr) + 1, 0);
    strcpy(object->origin, originStr);
    object->destination = memory_allocation(NULL, strlen(destinationStr) + 1, 0);
    strcpy(object->destination, destinationStr);
    // Destination
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
void remove_route(route_s **routeList, int *routeListLength, const int index) {
    // Free string memory
    free((*routeList)[index].origin);
    free((*routeList)[index].destination);
    free((*routeList)[index].originName);
    free((*routeList)[index].destinationName);

    // Remove hole in list
    for (int i = index; i < *routeListLength - 1; ++i) {
        (*routeList)[i] = (*routeList)[i + 1];
    }

    // Shrink memory
    (*routeListLength)--;
    *routeList = memory_allocation(*routeList, *routeListLength * sizeof(route_s), 0);
}

/**
 * Searches for specific route in a file and appends resulting object to routes
 * (File should be alphabetically ordered)
 * @param fileName File to search in
 * @param transportType Transportation type in file
 * @param routes Pointer to route list
 * @param routeAmount Pointer to route list length
 */
void get_all_routes(const char *fileName, const transportType_e transportType, route_s **routes, int *routeAmount) {
    FILE *file = open_file(fileName, "r");
    const int lineLength = 200;
    char line[lineLength];

    // Read first irrelevant row
    fgets(line, lineLength, file);

    // Read all lines one by one
    while (fgets(line, lineLength, file) != NULL) {
        // Draw loading bar
        loading_bar(1);

        // Split line in substrings
        char originStr[50], destinationStr[50], originNameStr[50], destinationNameStr[50],
            travelTimeStr[10], emissionStr[10], priceStr[10], downtimeStr[10];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",
               originStr, destinationStr, originNameStr, destinationNameStr,
               travelTimeStr, emissionStr, priceStr, downtimeStr);

        // Parse parameters and catch error
        int errorFlag = 0;
        const int travelTime = strtol_check(travelTimeStr, 10, &errorFlag);
        const int emission = (int) round(strtod_check(emissionStr, &errorFlag)); // Convert decimal to integer
        const int price = (int) round(strtod_check(priceStr, &errorFlag) * 100); // Convert price to 1/100
        const int downTime = strtol_check(downtimeStr, 10, &errorFlag);

        if (errorFlag) {
            continue;
        }

        // Append parameter to routes
        append_route(routes, routeAmount,
                     originStr, destinationStr, originNameStr, destinationNameStr,
                     travelTime, emission, price, downTime, transportType);
    }

    fclose(file);
}

/**
 * Append string to list
 * @param str String to append
 * @param stringList List of strings
 * @param stringListLength Length of list
 */
void append_string(const char *str, char ***stringList, int *stringListLength) {
    // Expand memory
    (*stringListLength)++;
    *stringList = memory_allocation(*stringList, *stringListLength * sizeof(char *), 0);

    // Insert pointer to allocated empty string
    (*stringList)[*stringListLength - 1] = memory_allocation(NULL, strlen(str) + 1, 0);

    // Insert string at pointer address
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
void search_first_column(const char *query, char ***stringList, int *stringListLength,
                         const route_s *routes, const int routeAmount) {
    char query2[50];
    strcpy(query2, query);
    strcat(query2, "~");

    for (int i = 0; i < routeAmount; ++i) {
        // Skip identical strings
        if (i > 0 && stricmp(routes[i - 1].origin, routes[i].origin) == 0) {
            continue;
        }

        // Save matching strings in array
        if (stricmp(query, routes[i].origin) <= 0 && stricmp(query2, routes[i].origin) > 0) {
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
void search_second_column(const char *origin, const char *query, char ***stringList, int *stringListLength,
                          const route_s *routes, const int routeAmount) {
    char query2[50];
    strcpy(query2, query);
    strcat(query2, "~");

    for (int i = 0; i < routeAmount; ++i) {
        // Skip strings that doesnt match origin
        if (stricmp(origin, routes[i].origin) != 0) {
            continue;
        }

        // Skip identical strings
        if (i > 0 && stricmp(routes[i - 1].destination, routes[i].destination) == 0) {
            continue;
        }

        // Save matching strings in array
        if (stricmp(query, routes[i].destination) <= 0 && stricmp(query2, routes[i].destination) > 0) {
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
void remove_mismatches(const char *origin, const char *destination, route_s **routes, int *routeAmount) {
    for (int i = *routeAmount - 1; i >= 0; i--) {
        // Skip routes that match origin and destination
        if (stricmp((*routes)[i].origin, origin) == 0 && stricmp((*routes)[i].destination, destination) == 0) {
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
void free_string_list(char **stringList, const int stringListLength, const int dynamicList) {
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
void free_route_list(route_s *routeList, const int routeListLength, const int dynamicList) {
    // Free strings
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
int alphabetic_route_compare(const void *vp1, const void *vp2) {
    const route_s *route1 = (route_s *) vp1;
    const route_s *route2 = (route_s *) vp2;

    // Compare origin
    const int result = stricmp(route1->origin, route2->origin);
    if (result != 0) {
        return result;
    }

    // Compare destination
    return stricmp(route1->destination, route2->destination);
}

/**
 * Compares two trips based on a prioritized set of criteria
 * @param param Array of priorities to compare by
 * @param a Trip A
 * @param b Trip B
 * @return -1: Trip A < Trip B, 1: Trip A > Trip B, 0: Trip A == Trip B
 */
int compare_trips(void *param, const void *a, const void *b) {
    const route_s *trip_a = (const route_s *) a; // Cast first trip to route_s
    const route_s *trip_b = (const route_s *) b; // Cast second trip to route_s
    const priority_e *priorities = (priority_e *) param; // Cast parameter to priority array

    // Iterate through the priority list
    for (int i = 0; i < 3; ++i) {
        switch (priorities[i]) {
            case p_time: // Compare total travel time including downtime
                if (trip_a->travelTime + trip_a->downtime < trip_b->travelTime + trip_b->downtime) return -1;
                if (trip_a->travelTime + trip_a->downtime > trip_b->travelTime + trip_b->downtime) return 1;
                break;
            case p_price: // Compare price
                if (trip_a->price < trip_b->price) return -1;
                if (trip_a->price > trip_b->price) return 1;
                break;
            case p_emission: // Compare emissions
                if (trip_a->emission < trip_b->emission) return -1;
                if (trip_a->emission > trip_b->emission) return 1;
                break;
            default:
                break;
        }
    }
    return 0;
}

/**
 * Sorts an array of trips based on a prioritized set of criteria
 * @param trips Array of trips to be sorted
 * @param num_trips Number of trips in the array
 * @param priorities Array of priorities to determine the sorting order
 */

void sort_trips(route_s *trips, const size_t num_trips, void *priorities) {
    // Sort the trips array using qsort_s with compare_trips as the comparison function
    qsort_s(trips, num_trips, sizeof(route_s), compare_trips, (void *) priorities);
}
