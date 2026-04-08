//
// Created by k3rnel1x on 05.04.2026.
//

#include "array.h"
#include <stdlib.h>

Array* get_array()
{
    Array* arr = (Array*)calloc(1, sizeof(Array));
    if (!arr) return NULL;
    arr->data = (void**)calloc(ARR_INIT_SIZE, sizeof(void*));
    if (!arr->data) { free(arr); return NULL; };

    arr->capacity = ARR_INIT_SIZE;
    arr->count = 0;
    return arr;
}

void push(Array* arr, void* item)
{
    if (!arr || !item) return;

    // resize if needed
    if (arr->capacity == arr->count) {
        void** old_data = arr->data;
        arr->data = (void**)realloc(arr->data, (arr->capacity *= ARR_SIZE_SCALE) * sizeof(void*));
        if (!arr->data) {
            arr->data = old_data;
            return;
        }
    }

    // insert item
    arr->data[arr->count++] = item;
}

Array* link(Array* arr)
{
    // validate
    if (!arr) return NULL;
    if (!arr->data) return NULL;

    // alloc
    Array* cpy_arr = (Array*)calloc(1, sizeof(Array));
    if (!cpy_arr) return NULL;
    cpy_arr->data = (void**)calloc(ARR_INIT_SIZE, sizeof(void*));
    if (!cpy_arr->data) return NULL;

    // link
    cpy_arr->capacity = arr->capacity;
    cpy_arr->count    = arr->count;
    cpy_arr->data     = arr->data;

    return cpy_arr;
}

void delete_arr(Array** arr_ptr)
{
    if (!arr_ptr) return;

    Array* arr = *arr_ptr;

    for (int i = 0; i < arr->count; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    free(arr);

    *arr_ptr = NULL;
}

char** get_charpp(Array* arr, size_t index)
{
    if (index >= arr->count) return NULL;
    return (char**)arr->data[index];
}

char* get_charp(Array* arr, size_t index)
{
    if (index >= arr->count) return NULL;
    return (char*)arr->data[index];
}
