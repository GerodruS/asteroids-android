#pragma once

#include <vector>

class Asteroid
{
public:
    Asteroid();
    ~Asteroid();

    void generate();

    std::vector<float> points;
};