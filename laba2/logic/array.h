//
// Created by k3rnel1x on 05.04.2026.
//

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define ARR_INIT_SIZE  100
#define ARR_SIZE_SCALE 2


#define getp(table, index, ptr) (ptr)((table)->data[index])

struct Array {
    size_t capacity;
    size_t count;
    void** data;
};

Array* get_array();
Array* push(Array* arr, void* item);
Array* link(Array* arr);
void delete_arr(Array** arr_ptr);

// get content
char** get_charpp(Array* arr, size_t index);
char*  get_charp(Array* arr, size_t index);

#endif //ARRAY_H
