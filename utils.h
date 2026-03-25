#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>

size_t char_count(char* str, char c);
int fill_size_t(size_t* field, char* ptr);
int fill_double(double* field, char* ptr);
char* fill_str(char* ptr);

#endif // UTILS_H
