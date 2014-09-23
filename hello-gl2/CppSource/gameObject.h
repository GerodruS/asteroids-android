#pragma once

#include <vector>

#include "point.h"

class GameObject
{
public:
    GameObject();

    void setPosition(const float x, const float y);
    void setPosition(const Point& value);
    void moveOn(const float dx, const float dy);
    void moveOn(const Point& delta);
    void setVelocity(const float x, const float y);
    void setVelocity(const Point& value);
    void addVelocity(const float dx, const float dy);
    void addVelocity(const Point& delta);
    void setAngularVelocity(const float value);
    void addAngularVelocity(const float delta);

    virtual const Point& getPosition() const;
    const Point& getVelocity() const;
    float getAngularVelocity() const;
    const std::vector<Point>& getPoints() const
    {
        return points_;
    };

    virtual void step();

    void rotate(const float angle);

    bool isIntersect(const GameObject& other);

protected:
    std::vector<Point> points_;
    Point velocity_;
    float angularVelocity_;

};