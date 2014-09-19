#pragma once

#include <vector>

class Ship
{
public:
    Ship();
    ~Ship();

    std::vector<float> points;

    void move(float x, float y);

};