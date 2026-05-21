//
// Created by k3rnel1x on 02.05.2026.
//

#include "surfacedrawer.h"

void SurfaceDrawer::updateData(PointsArr* arr)
{

    this->arr = arr;

    this->Xangle = 0;
    this->Yangle = 0;

    xOffset = 0.0;
    yOffset = 0.0;
    zOffset = 0.0;

    calcNormPoints();
};

SurfaceDrawer::SurfaceDrawer(AppContext* context)
{
    if(!context)
        throw std::invalid_argument("context is null");

    this->context = context;
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
    normArr.points = NULL;
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

    Vec2DyArr v2arr = getVec2DyArr();
    for (int i = 0; i < normArr.count; i++)
    {
        Vector3 init = {
            .x = normArr.points[i].x,
            .y = normArr.points[i].z,
            .z = normArr.points[i].y
        };

        Vector3 v3 = _rotateX(_rotateY(init, Yangle), Xangle);

        v3.x += xOffset;
        v3.y += yOffset;
        v3.z += zOffset + initZOffset;

        Vector2 protectedPoint = protect(v3);
        Vector2 point = place(protectedPoint.x, protectedPoint.y);

        addVector2(&v2arr, point);
        p.drawEllipse(point.x, point.y, 5, 5);
    }

    QPen pen = p.pen();
    pen.setWidth(2);
    p.setPen(pen);
    fillNeibors(&v2arr, 30);
    drawLines(p, v2arr.data + 5);
    delVec2DyArr(&v2arr);
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

void SurfaceDrawer::calcNormPoints()
{
    if(!arr || !arr->points) return;

    delPointsArr(&normArr);
    normArr = getPointsArr();

    this->minZovoffset = arr->points[0].z;
    this->maxZovoffset = arr->points[0].z;
    int range = context->maxNormalizationRange - context->minNormalizationRange;

    for (size_t i = 0; i < arr->count; ++i)
    {
        // [-1, 1]
        Point p = arr->points[i];
        p.x = -1 + (p.x)/(40)*2;
        p.y = -1 + (p.y)/(40)*2;
        p.z = -1 + (p.z)/(MAXZ)*2;

        addPoint(&normArr, p);

        if(p.z < minZovoffset)
            minZovoffset = p.z;

        if(p.z > maxZovoffset)
            maxZovoffset = p.z;
    }
}

void SurfaceDrawer::drawLines(QPainter& p, Vector2* v2)
{
    if(!v2 || v2->visited)
        return;

    v2->visited = true;

    // qDebug() << "Vector v2 : " << v2; 
    for (int i = 0; i < NEIBORSCOUNT; ++i)
    {
        if(v2->neibors[i] && !v2->neibors[i]->visited)
        {
            p.drawLine(v2->x, v2->y, v2->neibors[i]->x, v2->neibors[i]->y);
            drawLines(p, v2->neibors[i]);
        }
    }
}

void SurfaceDrawer::fillNeibors(Vec2DyArr* arr, size_t matrix_size)
{
    for(int i = 0; i < arr->count; ++i)
    {
        memset(arr->data[i].neibors, 0, sizeof(Point*)*NEIBORSCOUNT);

        if(i - matrix_size >= 0){
            arr->data[i].neibors[0] = arr->data + i - matrix_size;
        }

        if(i + matrix_size < arr->count){
            arr->data[i].neibors[2] = arr->data + i + matrix_size;
        }

        if (i - 1 >= 0){
            arr->data[i].neibors[1] = arr->data + i - 1;
        }

        if (i + 1 < arr->count){
            arr->data[i].neibors[3] = arr->data + i + 1;
        }
        // qDebug() << "Neibors: " << arr->data[i].neibors[0] << ' ' << arr->data[i].neibors[1] << ' '
                                // << arr->data[i].neibors[2] << ' ' << arr->data[i].neibors[3];
    }
}