//
// Created by k3rnel1x on 02.05.2026.
//

#ifndef SURFACEDRAWER_H
#define SURFACEDRAWER_H
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "../../businesslogic/pointsarr/pointsarr.h"
#include <cmath>

#define MAXZ 2000
#define MAXY 2000
#define MAXX 2000

#define ROTATIONANGLE 0.05
#define STEP 0.2

#define SIZE 0.4

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
public:

    SurfaceDrawer();
    ~SurfaceDrawer();
    void updateData(PointsArr* arr);
    void paintEvent(QPaintEvent* event);

    void yRotate(double addAngle);
    void xRotate(double addAngle);

    void walkOx(double step);
    void walkOy(double step);
    void walkOz(double step);

private:
    Vector2 protect(Vector3 v);
    Vector3 _rotateY(Vector3 p, double angle);
    Vector3 _rotateX(Vector3 p, double angle);
    Vector2 place(double x, double y);
    void normalizePoints();

    PointsArr* arr;
    PointsArr normArr;
    double Xangle = 0;
    double Yangle = 0;
    Vector3* v;

    const double initZOffset = -2.0;
    double xOffset = 0.0;
    double yOffset = 0.0;
    double zOffset = 0.0;

    int minZovoffset;

};

#endif //SURFACEDRAWER_H
