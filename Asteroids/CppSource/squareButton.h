#pragma once

#include <vector>
#include "point.h"


class SquareButton
{
public:
    SquareButton();

    void setIsPress(const bool value);
    void setSize(const float x, const float y);
    void setSize(const Point& point);
    void setPosition(const float x, const float y);
    void setPosition(const Point& point);

    bool isPress() const;
    const Point& getPosition() const;
    const Point& getSize() const;

    bool checkTouch(const Point& touch) const;
    bool checkTouches(const std::vector<Point>& touches) const;

private:
    Point position_;
    Point size_;
    bool isPress_;

};