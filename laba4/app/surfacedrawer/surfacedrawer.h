//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef SURFACEDRAWER_H
#define SURFACEDRAWER_H
#include <QWidget>
#include <QPainter>
#include "../../businesslogic/pointsarr/pointsarr.h"
#include <cmath>

#define MAXZ 2000
#define MAXY 2000
#define MAXX 2000

#define SIZE 0.5

struct Vector2
{
    double x;
    double y;
};

struct Vector3
{
    double x;
    double y;
    double z;
};

class SurfaceDrawer : public QWidget {
private:
    Vector2 protect(Vector3 v);
    Vector3 rotateY(Vector3 p, double angle);
    Vector2 place(double x, double y);

    PointsArr* arr;
    double angle = 0;
    Vector3* v;

public:
    SurfaceDrawer();
    ~SurfaceDrawer();
    void updateData(PointsArr* arr);
    void paintEvent(QPaintEvent* event);

};

#endif //SURFACEDRAWER_H
