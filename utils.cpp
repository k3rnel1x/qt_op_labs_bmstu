#include "utils.h"

size_t char_count(char* str, char c)
{
    if (!str) return 0;
    size_t count = 0;
    for (char* ptr = str; *ptr; ptr++) if (*ptr == c) count++;
    return count;
}

int fill_double(double* field, char* ptr)
{
    if (!field || !ptr) return 0;

    char* endptr;
    *field = strtod(ptr, &endptr);
    if (endptr == ptr)
        return 0;
    return 1;
}

char* fill_str(char* ptr)
{
    if (!ptr) return NULL;
    size_t str_len = strchr(ptr, ',') - ptr;
    if (str_len == 0)
        return NULL;

    char* str = (char*)calloc(str_len+1, sizeof(char));
    if (!str) return NULL;

    strncpy(str, ptr, str_len);
    return str;
}
