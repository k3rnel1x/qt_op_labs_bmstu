#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_CAP   10
#define VEC_SCALE 2
#define FILDS_CNT 7

typedef struct Line {
    double year;
    char*  region;
    double natural_grow;
    double birth_rate;
    double death_rate;
    double general_demographic_weight;
    double urbanization;
    double* by_idx[FILDS_CNT];
} Line;

Line* get_line();

typedef struct Vector {
    int    inited;
    size_t count;
    size_t cap;
    Line** data;
} Vector;

Vector v_init();
void   v_push_ptr(Vector* v, Line* item_ptr);
Line*  v_item(Vector* v, size_t idx);
void   v_destroy(Vector* v);

// Iterator begin(Vector* v);
// Iterator end(Vector* v);
// Line*    get(Iterator* it);
// 
// void next(Iterator* iter);
// void set(Iterator* it, Line* book);
// int  is_end(Iterator* it, Line* line);

#endif // VECTOR_H
