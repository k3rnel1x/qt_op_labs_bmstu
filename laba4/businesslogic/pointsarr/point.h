#ifndef POINT
#define POINT
#define NEIBORSCOUNT 4
struct Point
{
    double x;
    double y;
    double z;
    Point* neibors[NEIBORSCOUNT];
    int visited = 0;
};

#endif
