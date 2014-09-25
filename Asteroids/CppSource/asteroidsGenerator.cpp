#include "asteroidsGenerator.h"

#include <stdlib.h>

#include "asteroid.h"


AsteroidsGenerator::AsteroidsGenerator(int edgeCountMin,
                                       int edgeCountMax,
                                       float radiusMin,
                                       float radiusMax,
                                       float velocityMin,
                                       float velocityMax) :
    position_({ 0.0f, 0.0f }),
    size_({ 0.0f, 0.0f }),
    edgeCountMin_(edgeCountMin),
    edgeCountMax_(edgeCountMax),
    radiusMin_(radiusMin),
    radiusMax_(radiusMax),
    velocityMin_(velocityMin),
    velocityMax_(velocityMax)
{
}


void AsteroidsGenerator::generate(Asteroid& asteroid,
                                  const Point* const position) const
{
    Point pointFrom, pointTo;
    getPoints(pointFrom, pointTo);

    if (position != 0) {
        asteroid.generate(edgeCountMin_,
                         edgeCountMax_,
                         radiusMin_ / 2.0f,
                         radiusMax_ / 2.0f);
        pointFrom = *position;
    }
    else {
        asteroid.generate(edgeCountMin_,
                          edgeCountMax_,
                          radiusMin_,
                          radiusMax_);
    }
    
    asteroid.setPosition(pointFrom);

    pointTo.x -= pointFrom.x;
    pointTo.y -= pointFrom.y;

    const float velocity = velocityMin_ + rand() % int(velocityMax_ - velocityMin_);
    const float angularVelocity = velocityMin_ + rand() % int(velocityMax_ - velocityMin_);

    if (position != 0) {
        asteroid.setAngularVelocity(2.0f * angularVelocity);
        asteroid.setGeneration(2);

        PointFunctions::normalize(pointTo, 2.0f * velocity);
    }
    else {
        asteroid.setAngularVelocity(angularVelocity);
        asteroid.setGeneration(1);

        PointFunctions::normalize(pointTo, velocity);
    }

    asteroid.setVelocity(pointTo.x, pointTo.y);
}


void AsteroidsGenerator::setFrame(const float positionX,
                                  const float positionY,
                                  const float sizeX,
                                  const float sizeY)
{
    position_.x = positionX;
    position_.y = positionY;

    size_.x = sizeX;
    size_.y = sizeY;
}


float AsteroidsGenerator::getPosition() const
{
    const float halfPerimeter = size_.x + size_.y + 4.0f * radiusMax_;
    return rand() % int(2.0f * halfPerimeter);
}


float AsteroidsGenerator::getPosition(const float from) const
{
    const float halfPerimeter = size_.x + size_.y + 4.0f * radiusMax_;
    float res = from + halfPerimeter + (rand() % int(halfPerimeter / 2.0f) - halfPerimeter / 4.0f);
    while (2.0f * halfPerimeter < res) {
        res -= 2.0f * halfPerimeter;
    }
    return res;
}



void AsteroidsGenerator::setPointAtPosition(Point& point, const float position) const
{
    const float width = size_.x + 2.0f * radiusMax_;
    const float height = size_.y + 2.0f * radiusMax_;

    if (0.0f <= position && position < height)
    {
        point.x = width;
        point.y = height - position;
    }
    else if (height <= position && position < height + width)
    {
        point.x = width - (position - height);
        point.y = 0.0f;
    }
    else if (height + width <= position && position < height + width + height)
    {
        point.x = 0.0f;
        point.y = position - (height + width);
    }
    else // (size_.y + size_.x + size_.y <= p < 2.0f * (size_.x + size_.y))
    {
        point.x = position - (height + width + height);
        point.y = height;
    }

    point.x += position_.x - radiusMax_;
    point.y += position_.y - radiusMax_;
}


void AsteroidsGenerator::getPoints(Point& pointFrom, Point& pointTo) const
{
    const float positionFrom = getPosition();
    const float positionTo = getPosition(positionFrom);

    setPointAtPosition(pointFrom, positionFrom);
    setPointAtPosition(pointTo, positionTo);
}