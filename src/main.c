#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "dataHandling.h"
#include "terminal.h"

void print_introduction(const char message[]);
void get_journey(location_s *start, location_s *end);
void get_preferences(void);
void get_result(void);
void print_table(void);

int main(void) {
    location_s startLocation, endLocation;
    preference_e enviormentPreference;

    FILE *trainTableFile = open_file("../../data/train.csv","r");
    printf("%d", get_file_lines(trainTableFile));

    // print_introduction();

    // get_journey(&startLocation, &endLocation);

    // get_preferences();
    
    // get_result();
    
    // print_table();

    // while (1) {}

    return EXIT_SUCCESS;
}