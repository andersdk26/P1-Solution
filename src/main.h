#ifndef MAIN_H
#define MAIN_H

static const char TRAIN_ROUTES_CSV_PATH[] = "../data/trainroutes.csv";
static const char FLIGHT_CSV_PATH[] = "../data/flights.csv";

const static char instructionsStart[] = "Please enter your starting location. If your location exists in the database, you can press TAB to autocomplete your entry.";
const static char instructionsDestination[] = "Please enter your destination. If your destination does not appear as an autocomplete suggestion, then a journey between your two locations does not exist. If your journey does exist in the database, you can press TAB to autocomplete your entry.";
const static char prioritisationInstructions[] = "Please enter your preferred order of prioritisation. Input a number from 1 to 3 for each of the three journey factors. Two factors can not have the same priority.";
const static char noJourney[] = "No route found between your two locations. Please try again.";
const static char endMessage[] = "Press 'Q' to quit or any other key to restart.";

#endif //MAIN_H