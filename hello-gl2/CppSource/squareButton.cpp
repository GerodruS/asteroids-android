#include "squareButton.h"


using namespace std;


SquareButton::SquareButton() :
  isPress_(false),
  position_({ 0.0f, 0.0f }),
  size_({ 0.0f, 0.0f })
{
}


void SquareButton::setIsPress(const bool value)
{
    isPress_ = value;
}


bool SquareButton::check(const Touches& touches) const
{
    bool r = false;
    for (Touches::const_iterator it = touches.begin(); it != touches.end() && !r; ++it) {
        r = check(it->second);
    }
    return r;
}


bool SquareButton::check(const Point& touch) const
{
    bool r = position_.x < touch.x && touch.x < position_.x + size_.x &&
             position_.y < touch.y && touch.y < position_.y + size_.y;
    return r;
}


bool SquareButton::isPress() const
{
    return isPress_;
}


void SquareButton::setPosition(const Point& point)
{
    setPosition(point.x, point.y);
}


void SquareButton::setPosition(const float x, const float y)
{
    position_.x = x;
    position_.y = y;
}


const Point& SquareButton::getPosition() const
{
    return position_;
}


void SquareButton::setSize(const Point& point)
{
    setSize(point.x, point.y);
}


void SquareButton::setSize(const float x, const float y)
{
    size_.x = x;
    size_.y = y;
}


const Point& SquareButton::getSize() const
{
    return size_;
}