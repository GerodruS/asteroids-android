#pragma once

#include <vector>

#include "point.h"

class Asteroid
{
public:
    Asteroid();
    ~Asteroid();

    void generate(int n);

    std::vector<float> points;
    Point velocity;

    void move(float x, float y);
    void step();

    void setPosition(float x, float y);
    void setVelocity(float x, float y);

    float getPositionX();
    float getPositionY();
};