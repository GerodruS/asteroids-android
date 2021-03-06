#pragma once

#include <vector>
#include "point.h"

class SquareButton
{
public:
    SquareButton();

    void setIsPress(const bool value);
    bool isPress() const;

    bool check(const std::vector<Point>& touches) const;
    bool check(const Point& touch) const;

    void setPosition(const Point& point);
    void setPosition(const float x, const float y);
    const Point& getPosition() const;

    void setSize(const Point& point);
    void setSize(const float x, const float y);
    const Point& getSize() const;

private:
    bool isPress_;
    Point position_;
    Point size_;

};