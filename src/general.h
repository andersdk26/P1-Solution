#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum {
    wc_black = 0x00,
    wc_blue = 0x01,
    wc_green = 0x02,
    wc_aqua = 0x03,
    wc_red = 0x04,
    wc_purple = 0x05,
    wc_yellow = 0x06,
    wc_white = 0x07,
    wc_default = 0x07,
    wc_gray = 0x08,
    wc_light_blue = 0x09,
    wc_light_green = 0x0A,
    wc_light_aqua = 0x0B,
    wc_light_red = 0x0C,
    wc_light_purple = 0x0D,
    wc_light_yellow = 0x0E,
    wc_bright_white = 0x0F,
    wc_black_hgl = 0x07,
    wc_default_hgl = 0x07,
    wc_blue_hgl = 0x17,
    wc_green_hgl = 0x27,
    wc_aqua_hgl = 0x37,
    wc_red_hgl = 0x47,
    wc_purple_hgl = 0x57,
    wc_yellow_hgl = 0x67,
    wc_white_hgl = 0x70,
    wc_gray_hgl = 0x87,
    wc_light_blue_hgl = 0x97,
    wc_light_green_hgl = 0xA7,
    wc_light_aqua_hgl = 0xB7,
    wc_light_red_hgl = 0xC7,
    wc_light_purple_hgl = 0xD7,
    wc_light_yellow_hgl = 0xE7,
    wc_bright_white_hgl = 0xF7,
} winColor_e;

void set_win_color(winColor_e color);
winColor_e merge_win_color(winColor_e color, winColor_e highlighter);
void cls(void);
void delay(clock_t millis);
void swap_int(int *p1, int *p2);
void print_warning(const char *msg);
#endif //GENERAL_H
