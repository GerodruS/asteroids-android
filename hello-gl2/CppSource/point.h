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

    static void normalize(Point& point, float value = 1.0f)
    {
        float l = length(point);
        point.x *= value / l;
        point.y *= value / l;
    }
};