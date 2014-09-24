#include "ship.h"

#include <stdlib.h>
#include <algorithm>

using std::vector;
using std::min;


Ship::Ship() :
    frictionForce_(0.9f)
{
    points_.resize(4);

    const float width = 50;
    const float height = 60;

    points_[0].x = 0.0f;   points_[0].y = 0.0f;
    points_[1].x = -width; points_[1].y = -height;
    points_[2].x = 0.0f;   points_[2].y = height;
    points_[3].x = width;  points_[3].y = -height;

    radiusMax_ = sqrtf(width * width + height * height);
    setVelocityMax(5.0f, 5.0f);
}


const Point& Ship::getPosition() const
{
    return points_[0];
}


void Ship::step()
{
    GameObject::step();

    Point velocity = getVelocity();
    float l = PointFunctions::length(velocity);
    /*
    if (l < frictionForce_)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }
    else
    {
        PointFunctions::normalize(velocity, l - frictionForce_);
    }
    */
    setVelocity(velocity.x * frictionForce_, velocity.y * frictionForce_);

    const Point& pos = getPosition();
    if (pos.x < framePositon_.x)
    {
        moveOn(frameSize_.x, 0.0f);
    }
    else if (framePositon_.x + frameSize_.x < pos.x)
    {
        moveOn(-frameSize_.x, 0.0f);
    }

    if (pos.y < framePositon_.y)
    {
        moveOn(0.0f, frameSize_.y);
    }
    else if (framePositon_.y + frameSize_.y < pos.y)
    {
        moveOn(0.0f, -frameSize_.y);
    }
}


bool Ship::isCollisionWithAsteroid(const Asteroid& asteroid) const
{
    const Point& positionShip = getPosition();
    const Point& positionAsteroid = asteroid.getPosition();
    const float d = PointFunctions::distance(positionShip, positionAsteroid);
    if (radiusMax_ + asteroid.getRadiusMax() < d)
    {
        return false;
    }
    else
    {
        bool res = false;
        const unsigned count = points_.size();
        for (unsigned i = 0; i < count && !res; i += 2)
        {
            if (asteroid.polygonsIntersect(points_[i]))
            {
                res = true;
            }
        }
        return res;
    }
}