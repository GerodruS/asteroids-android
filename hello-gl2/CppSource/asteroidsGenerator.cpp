#include "asteroidsGenerator.h"

#include <stdlib.h>

AsteroidsGenerator::AsteroidsGenerator() :
    edgeCountMin_(6),
    edgeCountMax_(12),
    radiusMin_(10),
    radiusMax_(30),
    velocityMin_(1),
    velocityMax_(3)
{
}


void AsteroidsGenerator::setFrame(const float positionX, const float positionY,
                                  const float sizeX,     const float sizeY)
{
    position_.x = positionX;
    position_.y = positionY;

    size_.x = sizeX;
    size_.y = sizeY;
}


void AsteroidsGenerator::generate(Asteroid& asteroid)
{
    asteroid.generate(edgeCountMin_, edgeCountMax_, radiusMin_, radiusMax_);

    Point pointFrom, pointTo;
    getPoints(pointFrom, pointTo);
    
    asteroid.setPosition(pointFrom);

    pointTo.x -= pointFrom.x;
    pointTo.y -= pointFrom.y;

    const float velocity = velocityMin_ + rand() % int(velocityMax_ - velocityMin_);
    PointFunctions::normalize(pointTo, velocity);

    asteroid.setVelocity(pointTo.x, pointTo.y);
}


void AsteroidsGenerator::getPoints(Point& pointFrom, Point& pointTo)
{
    float positionFrom = getPosition();
    float positionTo = getPosition(positionFrom);

    setPointAtPosition(pointFrom, positionFrom);
    setPointAtPosition(pointTo, positionTo);
}


float AsteroidsGenerator::getPosition()
{
    return rand() % int(2.0f * (size_.x + size_.y));
}


float AsteroidsGenerator::getPosition(float from)
{
    float halfPerimeter = size_.x + size_.y;
    float res = from + halfPerimeter + (rand() % int(halfPerimeter) - halfPerimeter / 2.0f);
    while (2.0f * halfPerimeter < res) {
        res -= 2.0f * halfPerimeter;
    }
    return res;
}



void AsteroidsGenerator::setPointAtPosition(Point& point, float position)
{    
    if (0.0f <= position && position < size_.y)
    {
        point.x = size_.x;
        point.y = position;
    }
    else if (size_.y <= position && position < size_.y + size_.x)
    {
        point.x = position - size_.y;
        point.y = 0.0f;
    }
    else if (size_.y + size_.x <= position && position < size_.y + size_.x + size_.y)
    {
        point.x = 0.0f;
        point.y = position - (size_.y + size_.x);
    }
    else // (size_.y + size_.x + size_.y <= p < 2.0f * (size_.x + size_.y))
    {
        point.x = position - (size_.y + size_.x + size_.y);
        point.y = size_.y;
    }

    point.x += position_.x;
    point.y += position_.y;
}