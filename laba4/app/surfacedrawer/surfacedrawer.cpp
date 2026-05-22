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
    
    timer->start(1);

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
    if (fabs(xOffset + step) < ABSMAXX)
    {
        xOffset += step;
    }
}

void SurfaceDrawer::walkOy(double step)
{
    if (fabs(yOffset + step) < ABSMAXY)
    {
        yOffset += step;
    }

}

void SurfaceDrawer::walkOz(double step)
{
    if (fabs(zOffset + step) < ABSMAXZ)
    {
        zOffset += step;
    }
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

        qDebug("(%lf, %lf, %lf)", v3.x, v3.y, v3.z);

        Vector2 protectedPoint = protect(v3);
        Vector2 point = place(protectedPoint.x, protectedPoint.y);

        addVector2(&v2arr, point);
        // p.drawEllipse(point.x, point.y, 5, 5);
    }

    QPen pen = p.pen();
    pen.setWidth(2);
    p.setPen(pen);
    fillNeibors(&v2arr);
    drawLines(p, v2arr);
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

    double oneStep = 1.0 / double(context->renderStep);
    for (int i = 0; i < arr->count; ++i)
    {
        // [-1, 1]
        Point p = arr->points[i];
        p.x = -1 + (p.x)/double(context->matrixSize)*2;
        p.y = -1 + (p.y)/double(context->matrixSize)*2;
        p.z = -1 + (p.z)/(VDMAXZ)*2;

        // if (p.z > 1.0)
            // p.z = 1.0;

        addPoint(&normArr, p);

        if(p.z < minZovoffset)
            minZovoffset = p.z;

        if(p.z > maxZovoffset)
            maxZovoffset = p.z;

        // qDebug("%lf %lf %lf\n", p.x, p.y, p.z);
    }
}

void SurfaceDrawer::drawLines(QPainter& p, Vec2DyArr& arr)
{
    for (int i = 0; i < arr.count; ++i)
    {
        Vector2& v1 = arr.data[i];
        for (int k = 0; k < NEIBORSCOUNT; ++k)
        {
            if (v1.neibors[k])
            {
                p.drawLine(v1.x, v1.y, v1.neibors[k]->x, v1.neibors[k]->y);
            }
        }
    }
}

void SurfaceDrawer::fillNeibors(Vec2DyArr* arr)
{
    int matrix_size = int(context->matrixSize);
    // qDebug() << matrix_size;
    for(int i = 0; i < arr->count; ++i)
    {
        memset(arr->data[i].neibors, 0, sizeof(Vector2*)*NEIBORSCOUNT);

        if (i + 1 < arr->count && (i+1) % matrix_size != 0) {
            arr->data[i].neibors[0] = arr->data + i + 1;
            // qDebug("i = %i; i + 1 = %i; i+1 % matrix_size = %i", i, i+1, (i+1) % matrix_size);
        }

        if(i + matrix_size < arr->count){
            // qDebug() << "i - matrix_size = " << i - matrix_size;
            arr->data[i].neibors[1] = arr->data + i + matrix_size;
        }
        // qDebug() << "i = " << i << "Neibors: " << arr->data[i].neibors[0] << ' ' << arr->data[i].neibors[1];
    }
}
// 4
// 0,0,0,0
// 0,0,0,0
// 0,0,0,0