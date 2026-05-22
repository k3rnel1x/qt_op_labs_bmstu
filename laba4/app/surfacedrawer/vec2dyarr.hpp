#pragma once

#include <stdlib.h>
#define INITCAPACITY 20
#define SCALECAPACITY 2
#define NEIBORSCOUNT 2

struct Vector2
{
    double x;
    double y;
    Vector2* neibors[NEIBORSCOUNT];
    // int visited = 0;
};

struct Vec2DyArr
{
    Vector2* data;
    size_t   count;     
    size_t   capacity;
};

Vec2DyArr getVec2DyArr();
void delVec2DyArr(Vec2DyArr* arr);
void addVector2(Vec2DyArr* arr, Vector2 v2);