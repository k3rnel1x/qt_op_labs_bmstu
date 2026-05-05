#include "point.h"

#ifndef POINTSARR
#define POINTSARR

#include <stdlib.h>
#define INITCAPACITY 20
#define SCALECAPACITY 2

struct PointsArr
{
  Point* points;
  size_t count;     
  size_t capacity;
};

PointsArr getPointsArr();
void delPointsArr(PointsArr* pointsArr);
void addPoint(PointsArr* pointsArr, Point point);

#endif