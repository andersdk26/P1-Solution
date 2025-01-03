#ifndef TERMINAL_H
#define TERMINAL_H

#include "dataHandling.h"
#include "general.h"

#define ESC 27
#define BSP 127
#define ENTER 13
#define START_BRACKET 91
#define BOX_WIDTH 48
#define BOX_PADDING 2

typedef enum {
    sic_none,
    sic_first,
    sic_second
} searchInColumn_e;


void box_print(const char originalMessage[], const char title[]);

void print_best_journey(route_s journey);

void print_alternative_journeys(route_s journeys[], int numberOfJourneys);

char *box_read(const char title[], const route_s *routes, int routeQuantity, searchInColumn_e searchColumn,
               const char *firstColumn);

void print_top_of_priority_boxes(const char titles[3][10]);

void print_middle_of_priority_boxes(const char titles[3][10]);

void print_bottom_of_priority_boxes(const char titles[3][10]);

void get_priorities(int priorities[3]);

void read_characters(char *input, const route_s *routes, int routeQuantity, searchInColumn_e searchColumn,
                     const char *firstColumn);

void print_top_of_priority_boxes(const char titles[3][10]);

void print_middle_of_priority_boxes(const char titles[3][10]);

void print_bottom_of_priority_boxes(const char titles[3][10]);

void print_top_of_box(const char title[], winColor_e titleColor);

void print_middle_of_box();

void print_bottom_of_box();

int length_of_longest_word(const char message[]);

int number_of_words(const char message[]);

void print_result();

void set_terminal_mode(DWORD setValues, DWORD clearValues);

int w_getchar();

void print_error(const char *msg);

void loading_bar(int mode);

void utf8_print(const wchar_t* utf8Str);

#endif //TERMINAL_H
