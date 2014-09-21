#pragma once

#include <math.h>

struct Point
{
    float x;
    float y;
};


struct PointFunctions
{
    static float lengthSqr(const Point& point)
    {
        return point.x * point.x + point.y * point.y;
    }

    static float length(const Point& point)
    {
        return sqrtf(lengthSqr(point));
    }

    static float polygonSquare(const Point& a, const Point& b, const Point& c)
    {
        Point vA, vB;

        vA.x = a.x - c.x;
        vA.y = a.y - c.y;

        vB.x = b.x - c.x;
        vB.y = b.y - c.y;

        return polygonSquare(vA, vB);
    }

    static float polygonSquare(const Point& vectorA, const Point& vectorB)
    {
        return fabsf(vectorA.x * vectorB.y - vectorA.y * vectorB.x) / 2.0f;
    }

    static float distance(const Point& pointA, const Point& pointB)
    {
        Point vector;
        vector.x = pointB.x - pointA.x;
        vector.y = pointB.y - pointA.y;
        return length(vector);
    }

    static void normalize(Point& point, float value = 1.0f)
    {
        float l = length(point);
        point.x *= value / l;
        point.y *= value / l;
    }
};