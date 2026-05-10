//
// Created by k3rnel1x on 02.05.2026.
//

#include "surfacedrawer.h"

void SurfaceDrawer::updateData(PointsArr* arr) {
    if(this->arr == arr)
        return;
        
    this->arr = arr;

    this->Xangle = 0;
    this->Yangle = 0;

    xOffset = 0.0;
    yOffset = 0.0;
    zOffset = 0.0;

    normalizePoints();
};

SurfaceDrawer::SurfaceDrawer()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SurfaceDrawer::update));
    
    timer->start(3);

    v = new Vector3[8] {
        {-SIZE, -SIZE,  SIZE},
        {-SIZE,  SIZE,  SIZE},
        { SIZE,  SIZE,  SIZE},
        { SIZE, -SIZE,  SIZE},

        {-SIZE, -SIZE, -SIZE},
        {-SIZE,  SIZE, -SIZE},
        { SIZE,  SIZE, -SIZE},
        { SIZE, -SIZE, -SIZE},
    };

}

SurfaceDrawer::~SurfaceDrawer()
{
    delete v;
}

void SurfaceDrawer::yRotate(double addAngle)
{
    Yangle += addAngle;
}

void SurfaceDrawer::xRotate(double addAngle)
{
    Xangle += addAngle;
}

void SurfaceDrawer::walkOx(double step)
{
    xOffset += step;
}

void SurfaceDrawer::walkOy(double step)
{
    yOffset += step;
}

void SurfaceDrawer::walkOz(double step)
{
    zOffset += step;
}

void SurfaceDrawer::paintEvent(QPaintEvent* event)
{
    if(!arr){
        return;
    }

    QPainter p;
    p.begin(this);
    // p.drawText(width()/2, height()/2, QString("Height"));
    // angle += 0.02;

    // for (int i = 0; i < 8; i++)
    // {
    //     Vector3 v3 = _rotateX(_rotateY(v[i], Yangle), Xangle);

    //     v3.x += xOffset;
    //     v3.y += yOffset;
    //     v3.z += zOffset + initZOffset;

    //     Vector2 protectedPoint = protect(v3);
    //     Vector2 point = place(protectedPoint.x, protectedPoint.y);
    //     // qDebug() << "Point: " << point.x << point.y;
    //     p.drawEllipse(point.x, point.y, 5, 5);
    // }

    for (int i = 0; i < arr->count; i++)
    {
        Vector3 init = {
            .x = arr->points[i].x,
            .y = arr->points[i].z,
            .z = arr->points[i].y
        };

        Vector3 v3 = _rotateX(_rotateY(init, Yangle), Xangle);

        v3.x += xOffset;
        v3.y += yOffset;
        v3.z += zOffset + initZOffset;

        Vector2 protectedPoint = protect(v3);
        Vector2 point = place(protectedPoint.x, protectedPoint.y);
        // qDebug() << "Point: " << point.x << point.y;
        p.drawEllipse(point.x, point.y, 5, 5);
    }

    // qDebug() << Xangle << Yangle;
    
    p.end();
}

// -1..1 -> 0..w/h
Vector2 SurfaceDrawer::place(double x, double y)
{
    double newX = (x + 1)/2 * width();
    double newY = (y + 1)/2 * height();

    return (Vector2){ .x = newX, .y = newY };
}

Vector2 SurfaceDrawer::protect(Vector3 v)
{
    return (Vector2){.x = v.x/v.z, .y = v.y/v.z};
}

Vector3 SurfaceDrawer::_rotateY(Vector3 p, double angle)
{
    return (Vector3){
        .x = p.x*cos(angle) + p.z*sin(angle),
        .y = p.y,
        .z = -p.x*sin(angle) + p.z*cos(angle),
    };
}

Vector3 SurfaceDrawer::_rotateX(Vector3 p, double angle)
{
    return (Vector3){
        .x = p.x,
        .y = p.y*cos(angle) - p.z*sin(angle),
        .z = p.y*sinf(angle) + p.z*cos(angle),
    };
}

void SurfaceDrawer::normalizePoints()
{
    if(!arr || !arr->points) return;

    for (size_t i = 0; i < arr->count; i++)
    {
        // [-1, 1]
        Point& p = arr->points[i];
        p.x = -1 + (p.x)/(30)*2;
        p.y = -1 + (p.y)/(30)*2;
        p.z = -1 + (p.z)/(45)*2;

        qDebug() << p.x << ' ' << p.y << ' ' << p.z << ' ';
    }
    
}