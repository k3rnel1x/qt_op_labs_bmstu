#include "pointsarr.h"

PointsArr getPointsArr()
{
    Point* data = (Point*)calloc(1, INITCAPACITY*sizeof(Point));

    PointsArr arr = {
        .points = data,
        .count = 0,
        .capacity = INITCAPACITY,
    };

    return arr; 
}

void delPointsArr(PointsArr* pointsArr)
{
    if(!pointsArr) return;

    if(pointsArr->points)
        free(pointsArr->points);
    pointsArr->points = NULL;
}

void addPoint(PointsArr* pointsArr, Point point)
{
    if(!pointsArr) return;

    if(pointsArr->capacity == pointsArr->count)
    {
        Point* new_points = (Point*)realloc(pointsArr->points, pointsArr->capacity*SCALECAPACITY*sizeof(point));
        if(!new_points)
            return;

        pointsArr->capacity *= SCALECAPACITY;

        pointsArr->points = new_points;
    }

    pointsArr->points[pointsArr->count++] = point;
}   
