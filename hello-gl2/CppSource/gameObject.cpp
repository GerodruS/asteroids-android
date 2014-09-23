#include "gameObject.h"


GameObject::GameObject() :
    angularVelocity_(0.0f)
{
}


void GameObject::setPosition(const float x, const float y)
{
    const Point& position = getPosition();
    moveOn(x - position.x, y - position.y);
}


void GameObject::setPosition(const Point& value)
{
    setPosition(value.x, value.y);
}


void GameObject::moveOn(const float dx, const float dy)
{
    const unsigned count = points_.size();
    for (unsigned i = 0; i < count; ++i)
    {
        points_[i].x += dx;
        points_[i].y += dy;
    }
}


void GameObject::moveOn(const Point& delta)
{
    moveOn(delta.x, delta.y);
}


void GameObject::setVelocity(float x, float y)
{
    velocity_.x = x;
    velocity_.y = y;
}


void GameObject::setVelocity(const Point& value)
{
    setVelocity(value.x, value.y);
}


void GameObject::addVelocity(float x, float y)
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


const Point& GameObject::getPosition() const
{
    return pointZero;
}


const Point& GameObject::getVelocity() const
{
    return velocity_;
}


float GameObject::getAngularVelocity() const
{
    return angularVelocity_;
}


void GameObject::step()
{
    moveOn(velocity_);
    rotate(angularVelocity_);
}


void GameObject::rotate(const float angle)
{
    const Point center = getPosition();
    const unsigned count = points_.size();
    for (unsigned i = 0; i < count; ++i)
    {
        PointFunctions::rotate(points_[i], center, angle);
    }
}


bool GameObject::isIntersect(const GameObject& other)
{
    return false;
}