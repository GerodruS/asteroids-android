#pragma once

#include "asteroidNew.h"

class AsteroidsGenerator
{
public:
    AsteroidsGenerator();

    void setFrame(const float positionX, const float positionY,
                  const float sizeX,     const float sizeY);

    void generate(Asteroid& asteroid);

private:
    Point position_;
    Point size_;

    int edgeCountMin_;
    int edgeCountMax_;
    float radiusMin_;
    float radiusMax_;
    float velocityMin_;
    float velocityMax_;

    void getPoints(Point& from, Point& to);
    float getPosition();
    float getPosition(float from);
    void setPointAtPosition(Point& point, float position);

};