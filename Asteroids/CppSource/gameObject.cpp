#include "gameObject.h"

#include <math.h>


using std::vector;


GameObject::GameObject() :
    velocityMax_({ 1.0e+10f, 1.0e+10f }),
    angularVelocity_(0.0f),
    toDel_(false)
{
    points_.push_back(pointZero);
}


void GameObject::setPosition(const float x, const float y)
{
    const Point& position = getPosition();
    addPosition(x - position.x, y - position.y);
}


void GameObject::setPosition(const Point& value)
{
    setPosition(value.x, value.y);
}


void GameObject::addPosition(const float dx, const float dy)
{
    const unsigned count = points_.size();
    for (unsigned i = 0; i < count; ++i) {
        points_[i].x += dx;
        points_[i].y += dy;
    }
}


void GameObject::addPosition(const Point& delta)
{
    addPosition(delta.x, delta.y);
}


void GameObject::setVelocity(const float x, const float y)
{
    velocity_.x = fminf(x, velocityMax_.x);
    velocity_.y = fminf(y, velocityMax_.y);
}


void GameObject::setVelocity(const Point& value)
{
    setVelocity(value.x, value.y);
}


void GameObject::setVelocityMax(const float x, const float y)
{
    velocityMax_.x = x;
    velocityMax_.y = y;
}


void GameObject::setVelocityMax(const Point& value)
{
    setVelocityMax(value.x, value.y);
}


void GameObject::addVelocity(const float x, const float y)
{
    velocity_.x += x;
    velocity_.y += y;
}


void GameObject::addVelocity(const Point& delta)
{
    addVelocity(delta.x, delta.y);
}


void GameObject::setAngularVelocity(const float value)
{
    angularVelocity_ = value;
}


void GameObject::addAngularVelocity(const float delta)
{
    angularVelocity_ += delta;
}


void GameObject::setDel(bool value)
{
    toDel_ = value;
}


const Point& GameObject::getPosition() const
{
    return points_[0];
}


const Point& GameObject::getVelocity() const
{
    return velocity_;
}


const Point& GameObject::getVelocityMax() const
{
    return velocityMax_;
}


float GameObject::getAngularVelocity() const
{
    return angularVelocity_;
}


const vector<Point>& GameObject::getPoints() const
{
    return points_;
}


bool GameObject::isDel() const
{
    return toDel_;
}


void GameObject::step()
{
    addPosition(velocity_);
    rotate(angularVelocity_);
}


void GameObject::rotate(const float angle)
{
    const Point& p = getPosition();
    const unsigned count = points_.size();
    for (unsigned i = 0; i < count; ++i) {
        PointFunctions::rotate(points_[i], p, angle);
    }
}