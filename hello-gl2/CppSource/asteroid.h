#pragma once

#include <vector>

#include "point.h"
#include "bullet.h"

class AsteroidOld
{
public:
    AsteroidOld();
    ~AsteroidOld();

    void generate(int n);

    float angularVelocity;
    std::vector<float> points;
    Point velocity;

    void move(float x, float y);
    void step();

    void setPosition(float x, float y);
    void setVelocity(float x, float y);

    bool asteroidIntersect(AsteroidOld& asteroid);
    bool pointIntersect(const Point& point);
    bool polygonsIntersect(const Point& point);
    bool polygonIntersect(const Point& a, const Point& b, const Point& c, const Point& point);
    bool bulletIntersect(Bullet& bullet);

    float getPositionX();
    float getPositionY();
    void getCenter(Point& point);

    float radiusMax;
    float radiusMin;

    void rotate(float  x_in, float  y_in,
        float& x_out, float& y_out,
        float  angle);

    void rotate(float  x_in, float  y_in,
        float  x_center, float  y_center,
        float& x_out, float& y_out,
        float  angle);

    void rotate(float angle);
};