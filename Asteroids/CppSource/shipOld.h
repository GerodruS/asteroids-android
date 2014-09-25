#pragma once

#include <vector>
#include "point.h"

class ShipOld
{
public:
    ShipOld();
    ~ShipOld();

    std::vector<float> points;
    Point direction;

    void move(float x, float y);
    Point getCenter();
    Point getBulletStartPosition();
    Point getBulletStartMove();

    void step();
};