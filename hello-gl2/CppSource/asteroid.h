#pragma once

#include <vector>

class Asteroid
{
public:
    Asteroid(int n);
    ~Asteroid();

    void generate(int n);

    std::vector<float> points;

    void move(float x, float y);
};