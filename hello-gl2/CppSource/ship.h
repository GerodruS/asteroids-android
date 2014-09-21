#pragma once

#include <vector>
#include "point.h"

class Ship
{
public:
    Ship();
    ~Ship();

    std::vector<float> points;
    Point direction;

    void move(float x, float y);
    Point getCenter();
    Point getBulletStartPosition();
    Point getBulletStartMove();

    void step();
};