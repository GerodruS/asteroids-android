#include "ship.h"

#include "asteroid.h"


Ship::Ship(const float width,
           const float height,
           const float frictionForce,
           const float velocityMax) :
    framePositon_(pointZero),
    frameSize_(pointZero),
    frictionForce_(frictionForce),
    radiusMax_(height),
    width_(width),
    height_(height)
{
    reset();
    setVelocityMax(velocityMax, velocityMax);
}


float Ship::getFrictionForce() const
{
    return frictionForce_;
}


const Point& Ship::getFramePositon() const
{
    return framePositon_;
}


const Point& Ship::getFrameSize() const
{
    return frameSize_;
}


const Point& Ship::getBulletStartPosition() const
{
    return points_[2];
}


Point Ship::getDirection() const
{
    Point p = { points_[2].x - points_[0].x, points_[2].y - points_[0].y };
    PointFunctions::normalize(p);
    return p;
}


void Ship::setFrictionForce(const float value)
{
    frictionForce_ = value;
}


void Ship::setFramePositon(const float x, const float y)
{
    framePositon_.x = x;
    framePositon_.y = y;
}


void Ship::setFramePositon(const Point& value)
{
    setFramePositon(value.x, value.y);
}


void Ship::setFrameSize(const float x, const float y)
{
    frameSize_.x = x;
    frameSize_.y = y;
}


void Ship::setFrameSize(const Point& value)
{
    setFrameSize(value.x, value.y);
}


bool Ship::isCollisionWithAsteroid(const Asteroid& asteroid) const
{
    const float d = PointFunctions::distance(getPosition(), asteroid.getPosition());
    if (radiusMax_ + asteroid.getRadiusMax() < d) {
        return false;
    }
    else {
        const bool res = asteroid.isPointInsidePolygons(points_[0]) ||
                         asteroid.isPointInsidePolygons(points_[2]);
        return res;
    }
}


void Ship::step()
{
    GameObject::step();

    //  сила трения
    const Point& velocity = getVelocity();
    setVelocity(velocity.x * (1.0f - frictionForce_),
        velocity.y * (1.0f - frictionForce_));

    //  свёртка пространства
    const Point& pos = getPosition();

    if (pos.x < framePositon_.x - radiusMax_) {
        addPosition(frameSize_.x + 2.0f * radiusMax_, 0.0f);
    }
    else if (framePositon_.x + frameSize_.x + radiusMax_ < pos.x) {
        addPosition(-(frameSize_.x + 2.0f * radiusMax_), 0.0f);
    }

    if (pos.y < framePositon_.y - radiusMax_) {
        addPosition(0.0f, frameSize_.y + 2.0f * radiusMax_);
    }
    else if (framePositon_.y + frameSize_.y + radiusMax_ < pos.y) {
        addPosition(0.0f, -(frameSize_.y + 2.0f * radiusMax_));
    }
}


void Ship::reset()
{
    setDel(false);

    points_.resize(4);

    points_[0].x = 0.0f;    points_[0].y = 0.0f;
    points_[1].x = -width_; points_[1].y = -height_;
    points_[2].x = 0.0f;    points_[2].y = height_;
    points_[3].x = width_;  points_[3].y = -height_;

    setVelocity(0.0f, 0.0f);
    setAngularVelocity(0.0f);
}