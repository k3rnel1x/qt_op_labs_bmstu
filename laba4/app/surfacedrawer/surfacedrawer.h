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
#include "../appcontext.h"
#include "vec2dyarr.hpp"
#include <cmath>

#define MAXZ 100
#define MAXY 1000
#define MAXX 1000

#define ROTATIONANGLE 0.05
#define STEP 0.2

#define SIZE 0.4
#define INITZOFFSET -2.0


struct Vector3
{
    double x;
    double y;
    double z;
};

class SurfaceDrawer : public QWidget {
public:

    SurfaceDrawer(AppContext* context);
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
    void calcNormPoints();
    void drawLines(QPainter& p, Vector2* point);
    void fillNeibors(Vec2DyArr* arr, size_t matrix_size);

    PointsArr* arr;
    PointsArr  normArr;
    AppContext* context;
    double Xangle = 0;
    double Yangle = 0;
    Vector3* v;

    const double initZOffset = INITZOFFSET;
    double xOffset = 0.0;
    double yOffset = 0.0;
    double zOffset = 0.0;

    int minZovoffset;
    int maxZovoffset;

};

#endif //SURFACEDRAWER_H
