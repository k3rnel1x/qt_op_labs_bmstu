#include "pointsarr.h"

PointsArr getPointsArr()
{
    Point* data = (Point*)calloc(1, sizeof(PointsArr));

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

    free(pointsArr->points);
    pointsArr->points = NULL;
}

void addPoint(PointsArr* pointsArr, Point point)
{
    if(!pointsArr) return;

    if(pointsArr->capacity == pointsArr->count)
    {
        Point* new_points = (Point*)realloc(pointsArr->points, INITCAPACITY*SCALECAPACITY*sizeof(Point));
        if(!new_points)
            return;

        pointsArr->capacity *= SCALECAPACITY;

        pointsArr->points = new_points;
    }
    pointsArr->points[pointsArr->count++] = point;
}   
