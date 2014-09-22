#pragma once

#include <math.h>

struct Point
{
    float x;
    float y;
};

static const Point pointZero = { 0.0f, 0.0f };

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

    static void rotate(Point& point, const Point& center, const float angle)
    {
        rotate(point.x, point.y, center.x, center.y, angle);
    }

    static void rotate(float& x, float& y, float centerX, float centerY, float angle)
    {
        x -= centerX;
        y -= centerY;

        rotate(x, y, angle);

        x += centerX;
        y += centerY;
    }

    static void rotate(Point& point, const float angle)
    {
        rotate(point.x, point.y, angle);
    }

    static void rotate(float& x, float& y, float angle)
    {
        angle *= 0.0174532925f;

        const float resX = x * cos(angle) - y * sin(angle);
        const float resY = x * sin(angle) + y * cos(angle);

        x = resX;
        y = resY;
    }
};