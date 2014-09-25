#include "asteroidsGenerator.h"

#include <stdlib.h>

AsteroidsGenerator::AsteroidsGenerator() :
    edgeCountMin_(6),
    edgeCountMax_(12),
    radiusMin_(25),
    radiusMax_(75),
    velocityMin_(0.5f),
    velocityMax_(3.0f)
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


void AsteroidsGenerator::generate(Asteroid& asteroid, const Point* const position)
{
    if (position != 0) {
        asteroid.generate(edgeCountMin_, edgeCountMax_, radiusMin_ / 2.0f, radiusMax_ / 2.0f);
    }
    else {
        asteroid.generate(edgeCountMin_, edgeCountMax_, radiusMin_, radiusMax_);
    }

    Point pointFrom, pointTo;
    getPoints(pointFrom, pointTo);

    if (position != 0) {
        pointFrom = *position;
    }
    
    asteroid.setPosition(pointFrom);

    pointTo.x -= pointFrom.x;
    pointTo.y -= pointFrom.y;

    const float velocity = velocityMin_ + rand() % int(velocityMax_ - velocityMin_);

    if (position != 0) {
        PointFunctions::normalize(pointTo, 2.0f * velocity);
    }
    else {
        PointFunctions::normalize(pointTo, velocity);
    }


    asteroid.setVelocity(pointTo.x, pointTo.y);

    const float angularVelocity = velocityMin_ + rand() % int(velocityMax_ - velocityMin_);


    if (position != 0) {
        asteroid.setAngularVelocity(2.0f * angularVelocity);
        asteroid.generation = 2;
    }
    else {
        asteroid.setAngularVelocity(angularVelocity);
        asteroid.generation = 1;
    }

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
    float halfPerimeter = size_.x + size_.y + 4.0f * radiusMax_;
    return rand() % int(2.0f * halfPerimeter);
}


float AsteroidsGenerator::getPosition(float from)
{
    float halfPerimeter = size_.x + size_.y + 4.0f * radiusMax_;
    float res = from + halfPerimeter + (rand() % int(halfPerimeter / 2.0f) - halfPerimeter / 4.0f);
    while (2.0f * halfPerimeter < res) {
        res -= 2.0f * halfPerimeter;
    }
    return res;
}



void AsteroidsGenerator::setPointAtPosition(Point& point, float position)
{
    float width = size_.x + 2.0f * radiusMax_;
    float height = size_.y + 2.0f * radiusMax_;
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