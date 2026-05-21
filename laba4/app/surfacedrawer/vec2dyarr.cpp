#include "vec2dyarr.hpp"

Vec2DyArr getVec2DyArr()
{
    Vector2* data = (Vector2*)calloc(1, INITCAPACITY*sizeof(Vector2));

    Vec2DyArr arr = {
        .data = data,
        .count = 0,
        .capacity = INITCAPACITY,
    };
    return arr; 
}

void delVec2DyArr(Vec2DyArr* arr)
{
    if(!arr) return;

    if(arr->data)
        free(arr->data);
    arr->data = NULL;
}

void addVector2(Vec2DyArr* arr, Vector2 v2)
{
    if(!arr) return;

    if(arr->capacity == arr->count)
    {
        Vector2* new_points = (Vector2*)realloc(arr->data, arr->capacity*SCALECAPACITY*sizeof(Vector2));
        if(!new_points)
            return;

        arr->capacity *= SCALECAPACITY;

        arr->data = new_points;
    }

    arr->data[arr->count++] = v2;
}   
