#include "point.h"

#include <math.h>


float PointFunctions::lengthSqr(const Point& point)
{
    return point.x * point.x + point.y * point.y;
}


float PointFunctions::length(const Point& point)
{
    return sqrtf(lengthSqr(point));
}


float PointFunctions::polygonSquare(const Point& a,
                                    const Point& b,
                                    const Point& c)
{
    const Point vA = { a.x - c.x, a.y - c.y };
    const Point vB = { b.x - c.x, b.y - c.y };
    return polygonSquare(vA, vB);
}


float PointFunctions::polygonSquare(const Point& vectorA, const Point& vectorB)
{
    return fabsf(vectorA.x * vectorB.y - vectorA.y * vectorB.x) / 2.0f;
}


float PointFunctions::distance(const Point& pointA, const Point& pointB)
{
    const Point vector = { pointB.x - pointA.x, pointB.y - pointA.y };
    return length(vector);
}


void PointFunctions::normalize(Point& point, float value)
{
    const float k = value / length(point);
    point.x *= k;
    point.y *= k;
}


void PointFunctions::rotate(Point& point, const Point& center, const float angle)
{
    rotate(point.x, point.y, center.x, center.y, angle);
}


void PointFunctions::rotate(float& x, float& y,
                            const float centerX, const float centerY,
                            const float angle)
{
    x -= centerX;
    y -= centerY;

    rotate(x, y, angle);

    x += centerX;
    y += centerY;
}


void PointFunctions::rotate(Point& point, const float angle)
{
    rotate(point.x, point.y, angle);
}


void PointFunctions::rotate(float& x, float& y, float angle)
{
    angle *= 0.0174532925f;

    const float resX = x * cos(angle) - y * sin(angle);
    const float resY = x * sin(angle) + y * cos(angle);

    x = resX;
    y = resY;
}


bool PointFunctions::isPointInsidePolygon(const Point& a,
                                          const Point& b,
                                          const Point& c,
                                          const Point& point,
                                          const float precision)
{
    const float xMin = fminf(a.x, fminf(b.x, c.x));
    const float yMin = fminf(a.y, fminf(b.y, c.y));
    const float xMax = fmaxf(a.x, fmaxf(b.x, c.x));
    const float yMax = fmaxf(a.y, fmaxf(b.y, c.y));

    if (xMin <= point.x && point.x <= xMax &&
        yMin <= point.y && point.y <= yMax) {

        const float sqrA = polygonSquare(a, b, c);

        float sqrB = 0.0f;
        sqrB += polygonSquare(a, b, point);
        sqrB += polygonSquare(b, c, point);
        sqrB += polygonSquare(c, a, point);

        return fabsf(sqrA - sqrB) < precision;
    }

    return false;
}