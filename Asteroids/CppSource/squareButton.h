#pragma once

#include <vector>
#include "point.h"


class SquareButton
{
public:
    SquareButton();

    void setPosition(const float x, const float y);
    void setPosition(const Point& point);
    void setSize(const float x, const float y);
    void setSize(const Point& point);
    void setBorder(const float value);
    void setIsPress(const bool value);

    const Point& getPosition() const;
    const Point& getSize() const;
    float getBorder() const;
    bool isPress() const;

    bool checkTouch(const Point& touch) const;
    bool checkTouches(const std::vector<Point>& touches) const;

private:
    Point position_;
    Point size_;
    float border_;
    bool isPress_;

};