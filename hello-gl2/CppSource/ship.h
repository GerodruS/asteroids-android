#pragma once

#include "gameObject.h"

class Ship : public GameObject
{
public:
    Ship();

    virtual const Point& getPosition() const;

    virtual void step();

    float getFrictionForce() const
    {
        return frictionForce_;
    }

    void setFrictionForce(float value)
    {
        frictionForce_ = value;
    }

    Point getDirection()
    {
        Point p = { points_[2].x - points_[0].x, points_[2].y - points_[0].y };
        PointFunctions::normalize(p);
        return p;
    }

    const Point& getFramePositon() const
    {
        return framePositon_;
    }

    const Point& getFrameSize() const
    {
        return frameSize_;
    }

    const Point& getBulletStartPosition() const
    {
        return points_[2];
    }    

    void setFramePositon(const Point& value)
    {
        setFramePositon(value.x, value.y);
    }

    void setFrameSize(const Point& value)
    {
        setFrameSize(value.x, value.y);
    }

    void setFramePositon(float x, float y)
    {
        framePositon_.x = x;
        framePositon_.y = y;
    }

    void setFrameSize(float x, float y)
    {
        frameSize_.x = x;
        frameSize_.y = y;
    }

private:
    float frictionForce_;
    Point framePositon_;
    Point frameSize_;

};