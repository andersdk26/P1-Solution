#ifndef TERMINAL_H
#define TERMINAL_H

enum inputType {STRING, NUMBER};

void box_print(const char message[], const char title[]);

char *box_read(const char title[]);

void get_priorities(int priorities[3]);

void read_characters(char *input);

void print_top_of_priority_boxes(const char titles[3][10]);

void print_middle_of_priority_boxes(const char titles[3][10]);

void print_bottom_of_priority_boxes(const char titles[3][10]);

void print_top_of_box(const char title[]);

void print_bottom_of_box();

int length_of_longest_word(const char message[]);

int number_of_words(const char message[]);

void print_result();

#endif //TERMINAL_H
