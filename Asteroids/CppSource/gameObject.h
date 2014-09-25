#pragma once

#include <vector>

#include "point.h"


class GameObject
{
public:
    GameObject();

    void setPosition(const float x, const float y);
    void setPosition(const Point& value);
    void addPosition(const float dx, const float dy);
    void addPosition(const Point& delta);

    void setVelocity(const float x, const float y);
    void setVelocity(const Point& value);
    void addVelocity(const float dx, const float dy);
    void addVelocity(const Point& delta);

    void setVelocityMax(const float x, const float y);
    void setVelocityMax(const Point& value);

    void setAngularVelocity(const float value);
    void addAngularVelocity(const float delta);

    void setDel(bool value);

    const Point& getPosition() const;
    const Point& getVelocity() const;
    const Point& getVelocityMax() const;
    float getAngularVelocity() const;
    const std::vector<Point>& getPoints() const;
    bool isDel() const;

    virtual void step();

    void rotate(const float angle);

protected:
    std::vector<Point> points_;
    Point velocity_;
    Point velocityMax_;
    float angularVelocity_;
    bool toDel_;

};