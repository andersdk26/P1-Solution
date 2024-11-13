#include "dataHandling.h"

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