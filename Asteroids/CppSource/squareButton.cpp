#include "squareButton.h"


using std::vector;


SquareButton::SquareButton() :
    position_(pointZero),
    size_(pointZero),
    border_(0.0f),
    isPress_(false)
{
}


void SquareButton::setPosition(const float x, const float y)
{
    position_.x = x;
    position_.y = y;
}


void SquareButton::setPosition(const Point& point)
{
    setPosition(point.x, point.y);
}


void SquareButton::setSize(const float x, const float y)
{
    size_.x = x;
    size_.y = y;
}


void SquareButton::setSize(const Point& point)
{
    setSize(point.x, point.y);
}


void SquareButton::setBorder(const float value)
{
    border_ = value;
}


void SquareButton::setIsPress(const bool value)
{
    isPress_ = value;
}


const Point& SquareButton::getPosition() const
{
    return position_;
}


const Point& SquareButton::getSize() const
{
    return size_;
}


float SquareButton::getBorder() const
{
    return border_;
}


bool SquareButton::isPress() const
{
    return isPress_;
}


bool SquareButton::checkTouch(const Point& touch) const
{
    const bool r = position_.x < touch.x && touch.x < position_.x + size_.x &&
                   position_.y < touch.y && touch.y < position_.y + size_.y;
    return r;
}


bool SquareButton::checkTouches(const vector<Point>& touches) const
{
    bool r = false;
    const unsigned count = touches.size();
    for (unsigned i = 0; i < count && !r; ++i) {
        r = checkTouch(touches[i]);
    }
    return r;
}