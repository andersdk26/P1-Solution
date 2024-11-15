#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "dataHandling.h"
#include "general.h"
#include "terminal.h"

void print_introduction(const char message[]);
void get_journey(route_s *start, route_s *end);
void get_preferences(void);
void get_result(void);
void print_table(void);

int main(void) {
    route_s route;
    preference_e enviormentPreference;

    route = search_file("Bravo",NULL,TRAIN_CSV_PATH);
    printf("%s", route.origin);

    // print_introduction();

    // get_journey(&startLocation, &endLocation);

    // get_preferences();
    
    // get_result();
    
    // print_table();

    // while (1) {}

    return EXIT_SUCCESS;
}