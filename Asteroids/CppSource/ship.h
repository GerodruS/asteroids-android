#pragma once

#include "gameObject.h"


class Asteroid;


class Ship : public GameObject
{
public:
    Ship(const float width = 50.0f,
         const float height = 60.0f,
         const float frictionForce = 0.01f,
         const float velocityMax = 4.0f);

    float getFrictionForce() const;
    const Point& getFramePositon() const;
    const Point& getFrameSize() const;

    const Point& getBulletStartPosition() const;
    Point getDirection() const;

    void setFrictionForce(const float value);
    void setFramePositon(const float x, const float y);
    void setFramePositon(const Point& value);
    void setFrameSize(const float x, const float y);
    void setFrameSize(const Point& value);

    bool isCollisionWithAsteroid(const Asteroid& asteroid) const;

    virtual void step();
    void reset();

private:
    Point framePositon_;
    Point frameSize_;
    float frictionForce_;
    float radiusMax_;
    float width_;
    float height_;

};