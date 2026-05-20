#ifndef POINT
#define POINT
#define NEIBORSCOUNT 4
struct Point
{
    double x;
    double y;
    double z;
    Point* neibors[4];
    int visited = 0;
};

#endif
