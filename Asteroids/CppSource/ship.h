#pragma once

#include "gameObject.h"


class Asteroid;


class Ship : public GameObject
{
public:
    Ship();

    void init(const float width,
              const float height,
              const float frictionForce,
              const float velocityMax);

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