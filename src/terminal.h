#ifndef TERMINAL_H
#define TERMINAL_H

void box_print(const char message[], const char title[]);

char *box_read();

void print_top_of_box(int width, const char title[]);

void print_bottom_of_box(int width);

int length_of_longest_word(const char message[]);

int number_of_words(const char message[]);

void print_result();

#endif //TERMINAL_H
