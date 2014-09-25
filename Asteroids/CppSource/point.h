#pragma once


struct Point
{
    float x;
    float y;
};


static const Point pointZero = { 0.0f, 0.0f };
static const Point pointOne = { 1.0f, 1.0f };


class PointFunctions
{
public:
    static float lengthSqr(const Point& point);

    static float length(const Point& point);

    static float polygonSquare(const Point& a,
                               const Point& b,
                               const Point& c);

    static float polygonSquare(const Point& vectorA, const Point& vectorB);

    static float distance(const Point& pointA, const Point& pointB);

    static void normalize(Point& point, float value = 1.0f);

    static void rotate(Point& point, const Point& center, const float angle);

    static void rotate(float& x, float& y,
                       const float centerX, const float centerY,
                       const float angle);

    static void rotate(Point& point, float angle);

    static void rotate(float& x, float& y, const float angle);

    static bool isPointInsidePolygon(const Point& a,
                                     const Point& b,
                                     const Point& c,
                                     const Point& point,
                                     const float precision = 0.001f);
};