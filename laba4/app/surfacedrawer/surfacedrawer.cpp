//
// Created by k3rnel1x on 02.05.2026.
//

#include "surfacedrawer.h"
#include <QPainter>

void SurfaceDrawer::updateData(PointsArr* arr) { this->arr = arr; };

SurfaceDrawer::SurfaceDrawer()
{
    
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

void SurfaceDrawer::paintEvent(QPaintEvent* event)
{
    if(!arr){
        return;
    }

    QPainter p;
    p.begin(this);
    // p.drawText(width()/2, height()/2, QString("Height"));
    angle += 0.0002;
    for (int i = 0; i < 8; i++)
    {
        v[i] = rotateY(v[i], angle);
        Vector2 protectedPoint = protect(v[i]);
        Vector2 point = place(protectedPoint.x, protectedPoint.y);
        qDebug() << "Point: " << point.x << point.y;
        p.drawEllipse(point.x, point.y, 3, 3);
    }
    qDebug() << angle;
    
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

Vector3 SurfaceDrawer::rotateY(Vector3 p, double angle)
{
    return (Vector3){
        .x = p.x*cos(angle) + p.z*sin(angle),
        .y = p.y,
        .z = -p.x*sin(angle) + p.z*cos(angle),
    };
}